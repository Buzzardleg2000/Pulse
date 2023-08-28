# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import re
import sys
import json
import logging
import numpy as np
import pandas as pd
from enum import Enum
from pathlib import Path
from typing import Dict, List, Optional

import PyPulse
from pulse.cdm.patient import eSex
from pulse.cdm.scalars import TimeUnit
from pulse.cdm.scenario import SEScenarioLog
from pulse.cdm.utils.csv_utils import read_csv_into_df
from pulse.cdm.utils.logger import break_camel_case


_pulse_logger = logging.getLogger('pulse')


class ITMScenarioReport(SEScenarioLog):
    __slots__ = ("_csv_file", "_df", "_observation_frequency_s", "_vitals_headers", "_start_headers")

    def __init__(
        self,
        log_file: Path,
        csv_file: Path,
        observation_frequency_min: float=3,
        headers: Optional[Dict[str, str]] = None,
        actions_filter: Optional[List[str]] = None,
        events_filter: Optional[List[str]] = None
    ):
        """
        Generates ITM report from given results and log files.

        :param log_file: Path to log file
        :param csv_file: Path to csv file containing scenario results
        :param observation_frequency_s: Frequency to include observations in report
        :param headers: Headers to include in each observation, if not using defaults
        :param actions_filter: Exclude actions containing any of these strings
        :param events_filter: Exclude events containing any of these strings
        """
        super().__init__(
            log_file=log_file,
            actions_filter=actions_filter,
            events_filter=events_filter
        )

        # Always filter advance time and serialize actions
        adv_time = "AdvanceTime"
        if self._actions_filter is None or adv_time not in self._actions_filter:
            if not self._actions_filter:
                self._actions_filter = list()
            self._actions_filter.append(adv_time)
        serialize = "Serialize"
        if serialize not in self._actions_filter:
            self._actions_filter.append(serialize)

        if observation_frequency_min <= 0:
            raise ValueError(f"Observation frequency must be positive, got {observation_frequency_min}")
        self._observation_frequency_s = observation_frequency_min * 60

        if not csv_file.is_file():
            raise ValueError(f"CSV file ({csv_file}) does not exist/is not a file")
        self._csv_file = csv_file

        # Headers to be included in report with display names
        self._vitals_headers = headers
        if self._vitals_headers is None:
            self._vitals_headers = {
                "HeartRate(1/min)" : "HR",
                "RespirationRate(1/min)" : "RR",
                "OxygenSaturation": "SpO2",
                "SystolicArterialPressure(mmHg)": "Systolic",
                "DiastolicArterialPressure(mmHg)": "Diastolic",
            }

        # Headers needed for START algorithm
        self._start_headers = [
            "RespirationRate(1/min)",
            "SystolicArterialPressure(mmHg)",
            "RightArmVasculature-InFlow(mL/s)",
            "BrainVasculature-Oxygen-PartialPressure(mmHg)",
            "OxygenSaturation"
        ]

        self._df = None
        self._prepare_df()

    def _prepare_df(self) -> None:
        """
        Loads dataframe from CSV and filters down to only needed columns,
        converting units where needed.
        """
        # Determine set of needed headers for this report
        all_headers = set(self._start_headers)
        all_headers.update(self._vitals_headers.keys())
        df = read_csv_into_df(self._csv_file)
        time_header = "Time(s)"
        all_headers.add(time_header)  # Make sure we keep time (as seconds)

        # Filter dataframe to only needed headers
        self._df = pd.DataFrame()
        for header in all_headers:
            if header not in df.columns:
                # Attempt to locate header with different unit and convert
                resolved = False
                paren_idx = header.find("(")
                if paren_idx != -1:
                    unitless_header = header[:(paren_idx+1)]
                    desired_unit = header[(paren_idx+1):-1].replace("_", " ")
                    for h in df.columns:
                        if h.startswith(unitless_header):
                            results_paren_idx = h.find("(")
                            if results_paren_idx != -1:
                                results_unit = h[(results_paren_idx+1):-1].replace("_", " ")
                                self._df[header] = df[h].map(lambda x: PyPulse.convert(x, results_unit, desired_unit))
                                resolved = True
                if not resolved:
                    raise ValueError(f"Missing required and/or requested header: {header}")
            else:
                self._df[header] = df[header]

        # Set time as df index col
        self._df = self._df.set_index(time_header)

    def _START(self, data: pd.Series) -> str:
        """
        Computes START Triage category.

        :param data: Data series containing the necessary headers to compute START result

        :return: START Triage category
        """
        triage_category = Enum(
            'TriageCategory',
            names={
                "EXPECTANT": "Black",
                "IMMEDIATE": "Red",
                "DELAYED": "Yellow",
                "MINOR": "Green"},
            type=str
        )

        if data["RespirationRate(1/min)"] == 0:
            return triage_category.EXPECTANT
        elif data["RespirationRate(1/min)"] > 30:
            return triage_category.IMMEDIATE
        elif data["SystolicArterialPressure(mmHg)"] < 85.1:
            return triage_category.IMMEDIATE
        # TODO: Need CRT
        #elif data["RightArmVasculature-InFlow(mL/s)"] < CRT:
        #    return triage_category.IMMEDIATE
        elif data["BrainVasculature-Oxygen-PartialPressure(mmHg)"] < 19:
            return triage_category.IMMEDIATE
        elif data["OxygenSaturation"] < .92:
            return triage_category.DELAYED
        else:
            return triage_category.MINOR

    def generate(self) -> Dict:
        """
        Generates ITM report content.

        :return: Dict representing report
        """
        ABNORMALITY = "Abnormality"
        INSULTS = "Insults"
        INTERVENTIONS = "Interventions"
        PATIENT_ACTION = "PatientAction"

        out = dict()

        # Parse patient, actions, and events from log
        self._process_log()

        out["PatientAge_yr"] = self._patient.get_age().get_value(TimeUnit.yr)
        out["PatientSex"] = "Male" if self._patient.get_sex() == eSex.Male else "Female"

        # Determine time step
        # Note: Assumes time step is not variable
        times = self._df.index.values
        time_step_s = 0.02
        if len(times) > 1:
            time_step_s = times[1] - times[0]

        # Add set of observations per closest time step to requested observation frequency
        observations = list()
        observation_times= np.arange(
            start=times[0],
            stop=times[-1] + self._observation_frequency_s, # TODO: Do we need to add observation frequency here?
            step=self._observation_frequency_s
        )
        for requested_time_s in observation_times:
            observation = {}

            # Determine closest time to requested observation time in case time step doesn't line up with frequency
            time_s = requested_time_s if time_step_s == 0 else np.floor( (requested_time_s / time_step_s) + 0.5 ) * time_step_s
            if time_s < times[0]:
                continue
            if time_s > times[-1]:
                break

            data = self._df.loc[time_s]
            observation["SimTime_min"] = float(time_s) / 60

            # Add requested CSV data
            for header, display in self._vitals_headers.items():
                observation[display] = float(data[header])

            observation["START"] = self._START(data)

            # Add active abnormalities (events)
            observation[ABNORMALITY] = list()
            for event_time, event in self._events:
                if event_time.get_value(TimeUnit.s) <= time_s:
                    # Note: using re.match here instead of re.search because we expect the match at index 0
                    # Add it to the list of abnormalities if active
                    match = re.match(r'\[Event\s*(?P<abnormality>\D*)(?P<active>\d)\]', event, re.IGNORECASE)
                    if match is None:
                        _pulse_logger.warning(f"Could not identify if event is active, ignoring: {event}")
                        continue
                    event_name = match["abnormality"].strip()
                    event_active = int(match["active"])
                    if event_active:
                        observation[ABNORMALITY].append(event_name)
                    else:
                        if event_name in observation[ABNORMALITY]:
                            observation[ABNORMALITY].remove(event_name)
                        else:
                            _pulse_logger.warning(f"Could not find corresponding abnormality: {event}")

            # Add insults and interventions from actions
            def _get_severity_str(severity: float) -> str:
                if severity == 0:
                    raise ValueError("Severity string undefined for 0 severity")
                elif severity <= 0.3:
                    severity_str = "Mild"
                elif severity <= 0.6:
                    severity_str = "Moderate"
                else:
                    severity_str = "Severe"

                return severity_str
            observation[INSULTS] = list()
            observation[INTERVENTIONS] = list()
            hemorrhage = -1
            for action_time, action in self._actions:
                if action_time.get_value(TimeUnit.s) <= time_s:
                    action_data = json.loads(action)

                    if PATIENT_ACTION in action_data:
                        action_name = list(action_data[PATIENT_ACTION].keys())[0]

                        severity_str = ""
                        severity = 0
                        insult = False
                        if "Severity" in action_data[PATIENT_ACTION][action_name]:
                            insult = True
                            severity = action_data[PATIENT_ACTION][action_name]["Severity"]["Scalar0To1"]["Value"]

                            # Add side and type to pneumothorax actions
                            if action_name.endswith("Pneumothorax"):
                                action_name = f'{action_data[PATIENT_ACTION][action_name]["Side"]}' \
                                              f'{action_data[PATIENT_ACTION][action_name]["Type"]}' \
                                              f'{action_name}'

                            if severity == 0:  # Severity 0 indicates intervention
                                insult = False
                                if action_name == "Hemorrhage":
                                    action_name = "Tourniquet"
                                elif action_name == "AirwayObstruction":
                                    action_name = "SecuredAirway"
                                elif action_name.endswith("Pneumothorax"):
                                    action_name = f"{action_name}WasCorrected"
                                else:
                                    _pulse_logger.warning(f"Ignoring zero-severity action: {action_data}")
                                    continue
                            else:  # Insult
                                severity_str = _get_severity_str(severity)

                        action_out = f"{severity_str} {break_camel_case(action_name)}".strip()
                        if "Hemorrhage" in action_name:  # Only adding max severity hemorrhage
                            if severity > hemorrhage:
                                hemorrhage = severity
                        elif insult:
                            observation[INSULTS].append(action_out)
                        else:
                            observation[INTERVENTIONS].append(action_out)
                    else:
                        raise ValueError(f"Unsupported action: {action_data}")

            # Add max severity hemorrhage, if one exists
            if hemorrhage > 0:
                observation[INSULTS].append(f"{_get_severity_str(hemorrhage)} Hemorrhage")

            observations.append(observation)

        out["Observations"] = observations
        return out

    def write(self, out_file: Path) -> None:
        """
        Generates and writes ITM report.

        :param out_file: JSON filepath
        """
        out_file.parent.mkdir(parents=True, exist_ok=True)

        with open(out_file, "w") as f:
            f.write(json.dumps(self.generate(), indent=2))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    if len(sys.argv) < 4:
        _pulse_logger.error("Expected inputs : <log file> <csv results file> <out file> [observation_frequency_min]")
        sys.exit(1)

    log_file = Path(sys.argv[1])
    csv_file = Path(sys.argv[2])
    out_file = Path(sys.argv[3])
    observation_frequency_min = 3

    if not log_file.is_file():
        _pulse_logger.error("Please provide a valid log file")
        sys.exit(1)

    if not csv_file.is_file():
        _pulse_logger.error("Please provide a valid CSV file")
        sys.exit(1)

    if len(sys.argv) > 4:
        observation_frequency_min = float(sys.argv[4])

    itm = ITMScenarioReport(
        log_file=log_file,
        csv_file=csv_file,
        observation_frequency_min=observation_frequency_min
    )
    itm.write(out_file)
