# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import re
import sys
import json
import logging
from enum import Enum
from pathlib import Path
from typing import Any, Dict, Hashable, Iterable, List, NamedTuple, Optional, Tuple

from pulse.cdm.engine import eEvent, SEEventChange
from pulse.cdm.patient import eSex
from pulse.cdm.scalars import TimeUnit, FrequencyUnit, SEScalarFrequency
from pulse.cdm.scenario import SEScenarioReport, SEObservationReportModule, SETimestepReportModule
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
    RR_Per_min = "RespirationRate(1/min)"
    SAP_mmHg = "SystolicArterialPressure(mmHg)"
    PPI = "PeripheralPerfusionIndex"
    BRAIN_VASC_O2PP_mmHg = "BrainVasculature-Oxygen-PartialPressure(mmHg)"
    SPO2 = "OxygenSaturation"

    def __init__(self):
        super().__init__()
        self._headers = [
            self.RR_Per_min,
            self.SAP_mmHg,
            self.PPI,
            self.BRAIN_VASC_O2PP_mmHg,
            self.SPO2
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
        if data_slice[slice_idx[self.RR_Per_min]] == 0:
            triage_status = triage_category.EXPECTANT
        elif data_slice[slice_idx[self.RR_Per_min]] > 30:
            triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx[self.SAP_mmHg]] < 85.1:
            triage_status = triage_category.IMMEDIATE
        # PPI is serving as a replacement for capillary refill time (CRT)
        elif data_slice[slice_idx[self.PPI]] < 0.003:
            triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx[self.BRAIN_VASC_O2PP_mmHg]] < 19:
            triage_status = triage_category.IMMEDIATE
        elif data_slice[slice_idx[self.SPO2]] < .92:
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

    def handle_event(self, change: SEEventChange) -> None:
        """
        Add/remove event from active events.
        """
        if change.active:
            self._active_events.append(change.event.name)
        else:
            if change.event.name in self._active_events:
                self._active_events.remove(change.event.name)
            else:
                _pulse_logger.warning(f"Could not find corresponding active abnormality: {change.event.name}")

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


class TCCCDeathCheckModule(SETimestepReportModule):
    """
    Reports if patient survives. Raises StopIteration on update if patient death detected.
    """

    __slots__ = ("_irreversible_state", "_cardiovascular_collapse", "_brain_O2_deficit",
                 "_start_brain_O2_deficit_s", "_myocardium_O2_deficit", "_start_myocardium_O2_deficit_s",
                 "_spO2_deficit", "_start_spO2_deficit_s", "_max_hr_bpm", "_survived")

    TIME_s = "Time(s)"
    HR_bpm = "HeartRate(1/min)"
    SPO2 = "OxygenSaturation"
    SAP_mmHg = "SystolicArterialPressure(mmHg)"

    def __init__(self):
        super().__init__()
        self._headers = [
            self.TIME_s,
            self.HR_bpm,
            self.SPO2,
            self.SAP_mmHg
        ]
        self._irreversible_state = False
        self._cardiovascular_collapse = False
        self._brain_O2_deficit = False
        self._start_brain_O2_deficit_s = 0.
        self._myocardium_O2_deficit = False
        self._start_myocardium_O2_deficit_s = 0.
        self._spO2_deficit = False
        self._start_spO2_deficit_s = 0.
        self._max_hr_bpm = None
        self._survived = True

    def set_max_hr(self, max_hr: SEScalarFrequency) -> None:
        self._max_hr_bpm = max_hr.get_value(FrequencyUnit.Per_min)

    def handle_event(self, change: SEEventChange) -> None:
        """
        Check for death indicators.
        """
        if change.event == eEvent.IrreversibleState:
            self._irreversible_state = change.active

        if change.event == eEvent.CardiovascularCollapse:
            self._cardiovascular_collapse = change.active

        if change.event == eEvent.BrainOxygenDeficit:
            if change.active:
                if not self._brain_O2_deficit:
                    self._brain_O2_deficit = True
                    self._start_brain_O2_deficit_s = change.sim_time.get_value(TimeUnit.s)
            else:
                self._brain_O2_deficit = False
                self._start_brain_O2_deficit_s = 0

        if change.event == eEvent.MyocardiumOxygenDeficit:
            if change.active:
                if not self._myocardium_O2_deficit:
                    self._myocardium_O2_deficit = True
                    self._start_myocardium_O2_deficit_s = change.sim_time.get_value(TimeUnit.s)
            else:
                self._myocardium_O2_deficit = False
                self._start_myocardium_O2_deficit_s = 0

    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> None:
        """
        Determine if death is indicated, if so raise StopIteration.
        """
        curr_time_s = data_slice[slice_idx[self.TIME_s]]

        if self._irreversible_state:
            self._survived = False
            raise StopIteration(f"Patient died from irreversible state @{curr_time_s}s")

        if self._cardiovascular_collapse:
            self._survived = False
            raise StopIteration(f"Patient died from cardiovascular collapse @{curr_time_s}s")

        if self._max_hr_bpm is None:
            raise ValueError("Max HR BPM is not set in death check")
        elif data_slice[slice_idx[self.HR_bpm]] >= self._max_hr_bpm:
            self._survived = False
            raise StopIteration(f"Patient died from reaching max hr of {self._max_hr_bpm} @{curr_time_s}s")

        if self._brain_O2_deficit and (curr_time_s - self._start_brain_O2_deficit_s) > 180:
            self._survived = False
            raise StopIteration(f"Patient died from brain O2 deficit of 180s @{curr_time_s}s")

        if self._myocardium_O2_deficit and (curr_time_s - self._start_myocardium_O2_deficit_s) > 180:
            self._survived = False
            raise StopIteration(f"Patient died from myocardium O2 deficit of 180s @{curr_time_s}s")

        if data_slice[slice_idx[self.SPO2]] < 0.85:
            if not self._spO2_deficit:
                self._spO2_deficit = True
                self._start_spO2_deficit_s = curr_time_s
            elif (curr_time_s - self._start_spO2_deficit_s) > 140:
                self._survived = False
                raise StopIteration(f"Patient died from SpO2 < 85 for 140s @{curr_time_s}s")
        else:
            self._spO2_deficit = False

        if data_slice[slice_idx[self.SAP_mmHg]] < 60:
            self._survived = False
            raise StopIteration(f"Patient died from SBP < 60 @{curr_time_s}s")

    def report(self) -> Iterable[Tuple[Hashable, Any]]:
        """
        Report survival status.
        """
        return [
            ("Survives", self._survived),
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
        death_check_module = TCCCDeathCheckModule()

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

        # After super init so that we can use the parsed patient
        if self._patient.has_heart_rate_maximum():
            death_check_module.set_max_hr(self._patient.get_heart_rate_maximum())
        else:
            raise ValueError("Could not determine max heart rate")

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

    if len(sys.argv) <= 3:
        log_file = Path(sys.argv[1]+".log")
        csv_file = Path(sys.argv[1]+".csv")
        out_file = Path(sys.argv[1]+".obs.json")
        if len(sys.argv) == 3:
            observation_frequency_min = sys.argv[2]
        else:
            observation_frequency_min = 3
    else:
        _pulse_logger.error("Expected inputs : <log/csv file path w/o extension> [observation_frequency_min]")
        sys.exit(1)

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
