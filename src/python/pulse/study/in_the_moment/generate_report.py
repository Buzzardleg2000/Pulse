# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import re
import sys
import json
import logging
from enum import Enum
from pathlib import Path
from typing import Any, Dict, Hashable, Iterable, List, NamedTuple, Optional, Tuple

from pulse.cdm.engine import SEEventChange
from pulse.cdm.patient import eSex
from pulse.cdm.scalars import TimeUnit
from pulse.cdm.scenario import SEScenarioReport, SEObservationReportModule
from pulse.cdm.utils.logger import break_camel_case


_pulse_logger = logging.getLogger('pulse')


class VitalsObservationModule(SEObservationReportModule):
    """
    Reports requested vitals at the observation timestep.
    """

    __slots__ = ("_vitals")

    def __init__(self, vitals: Optional[Dict[str, str]]):
        """
        :param vitals: Mapping of desired vital headers to their display name in the report.
        """
        super().__init__()
        self._headers = vitals.keys() if vitals is not None else list()
        self._vitals = vitals if vitals is not None else dict()

    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> Iterable[Tuple[Hashable, Any]]:
        """
        Returns requested vitals.
        """
        vitals_out = list()
        for vital, display_vital in self._vitals.items():
            vitals_out.append((display_vital, data_slice[slice_idx[vital]]))
        return vitals_out


class STARTObservationModule(SEObservationReportModule):
    """
    Computes tag color indicated by the START algorithm at the observation timestep.
    """

    def __init__(self):
        super().__init__()
        self._headers = [
            "RespirationRate(1/min)",
            "SystolicArterialPressure(mmHg)",
            "RightArmVasculature-InFlow(mL/s)",
            "BrainVasculature-Oxygen-PartialPressure(mmHg)",
            "OxygenSaturation"
        ]

    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> Iterable[Tuple[Hashable, Any]]:
        """
        Computes START Triage category.
        """
        triage_category = Enum(
            "TriageCategory",
            names={
                "EXPECTANT": "Black",
                "IMMEDIATE": "Red",
                "DELAYED": "Yellow",
                "MINOR": "Green"},
            type=str
        )

        triage_status = None
        if data_slice[slice_idx["RespirationRate(1/min)"]] == 0:
            triage_status = triage_category.EXPECTANT
        elif data_slice[slice_idx["RespirationRate(1/min)"]] > 30:
            triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx["SystolicArterialPressure(mmHg)"]] < 85.1:
            triage_status = triage_category.IMMEDIATE
        # TODO: Need CRT
        #elif data_slice[slice_idx["RightArmVasculature-InFlow(mL/s)"]] < CRT:
        #    triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx["BrainVasculature-Oxygen-PartialPressure(mmHg)"]] < 19:
            triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx["OxygenSaturation"]] < .92:
            triage_status = triage_category.DELAYED
        else:
            triage_status = triage_category.MINOR

        return [("START", triage_status)]


class ClinicalAbnormalityObservationModule(SEObservationReportModule):
    """
    Reports active clinical abnormalities at the observation timestep.
    """

    __slots__ = ("_active_events")

    def __init__(self):
        super().__init__()
        self._active_events = list()

    def handle_event(self, event: SEEventChange) -> None:
        """
        Add/remove event from active events.
        """
        if event.active:
            self._active_events.append(event.event)
        else:
            if event.event in self._active_events:
                self._active_events.remove(event.event)
            else:
                _pulse_logger.warning(f"Could not find corresponding active abnormality: {event}")

    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> Iterable[Tuple[Hashable, Any]]:
        """
        Return list of active events.
        """
        return [("Abnormality", self._active_events[:])]  # Need to return a copy of list so it doesn't keep updating


class TCCCActionsObservationModule(SEObservationReportModule):
    """
    Reports insults and interventions applied from the beginning of the
    scenario until this observation timestep.
    """

    __slots__ = ("_insults", "_interventions", "_max_hemorrhage")

    def __init__(self):
        super().__init__()
        self._insults = list()
        self._interventions = list()
        self._max_hemorrhage = -1

    @staticmethod
    def _get_severity_str(severity: float) -> str:
        """
        :param severity: The severity value.

        :return: Severity as a qualitative string.
        """
        if severity == 0:
            raise ValueError("Severity string undefined for 0 severity")
        elif severity <= 0.3:
            severity_str = "Mild"
        elif severity <= 0.6:
            severity_str = "Moderate"
        else:
            severity_str = "Severe"

        return severity_str

    def handle_action(self, action: str) -> None:
        """
        Determine if action is insult or intervention.
        """
        # Load into dict direct from JSON because we can't serialize actions from bind
        action_data = json.loads(action)

        # TCCC handles a restricted set of actions:
        # Hemorrhage, AirwayObstruction, TensionPneumothorax, NeedleDecompression, ChestOcclusiveDressing
        # Certain actions with 0 severity are being treated as interventions (such as hemorrhage -> tourniquet)
        PATIENT_ACTION = "PatientAction"
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
                        return
                else:  # Insult
                    severity_str = TCCCActionsObservationModule._get_severity_str(severity)

            action_out = f"{severity_str} {break_camel_case(action_name)}".strip()

            if "Hemorrhage" in action_name:  # Only adding max severity hemorrhage
                if severity > self._max_hemorrhage:
                    self._max_hemorrhage = severity
            elif insult:
                self._insults.append(action_out)
            else:
                self._interventions.append(action_out)
        else:
            raise ValueError(f"Unsupported action: {action_data}")


    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> Iterable[Tuple[Hashable, Any]]:
        """
        Return insults and interventions that have occured up to this point.
        """
        # Add max severity hemorrhage, if one exists (want only one hemorrhage in results)
        insults = self._insults[:]
        if self._max_hemorrhage > 0:
            insults.append(f"{TCCCActionsObservationModule._get_severity_str( self._max_hemorrhage)} Hemorrhage")

        return [
            ("Insults", insults),
            ("Interventions", self._interventions[:])  # Need to return a copy of list so it doesn't keep updating
        ]


class ITMScenarioReport(SEScenarioReport):
    def __init__(
        self,
        log_file: Path,
        csv_file: Path,
        observation_frequency_min: float=3,
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
        # Always filter advance time and serialize actions
        adv_time = "AdvanceTime"
        if actions_filter is None or adv_time not in actions_filter:
            if not actions_filter:
                actions_filter = list()
            actions_filter.append(adv_time)
        serialize = "Serialize"
        if serialize not in actions_filter:
            actions_filter.append(serialize)

        # Desired vitals and display names
        vitals = {
            "HeartRate(1/min)" : "HR",
            "RespirationRate(1/min)" : "RR",
            "OxygenSaturation": "SpO2",
            "SystolicArterialPressure(mmHg)": "Systolic",
            "DiastolicArterialPressure(mmHg)": "Diastolic",
        }

        # Report modules
        observation_modules = [
            VitalsObservationModule(vitals=vitals),
            STARTObservationModule(),
            ClinicalAbnormalityObservationModule(),
            TCCCActionsObservationModule()
        ]
        timestep_modules = None
        death_check_module = None

        super().__init__(
            log_file=log_file,
            csv_file=csv_file,
            observation_frequency_min=observation_frequency_min,
            observation_modules=observation_modules,
            timestep_modules=timestep_modules,
            death_check_module=death_check_module,
            actions_filter=actions_filter,
            events_filter=events_filter
        )

    def _initialize_report(self) -> Dict:
        """
        Initialize the report dict with any initial datapoints.

        :return: Initialized report dict
        """
        init = super()._initialize_report()
        init["PatientAge_yr"] = self._patient.get_age().get_value(TimeUnit.yr)
        init["PatientSex"] = "Male" if self._patient.get_sex() == eSex.Male else "Female"

        return init


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
