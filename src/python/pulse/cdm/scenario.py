# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import re
import logging
from typing import List, Optional, Tuple
from pathlib import Path

from pulse.cdm.engine import eSerializationFormat, eSwitch, SEAction, SEDataRequestManager
from pulse.cdm.patient import eSex, SEPatient, SEPatientConfiguration
from pulse.cdm.scalars import SEScalarTime, get_unit
from pulse.cdm.io.patient import serialize_patient_from_string


_pulse_logger = logging.getLogger('pulse')


class SEScenario:
    __slots__ = ["_name", "_description", "_params", "_engine_state_file", "_dr_mgr", "_dr_files", "_actions"]

    def __init__(self):
        self._dr_mgr = SEDataRequestManager()
        self.clear()

    def __repr__(self):
        return f'SEScenario({self._name}, {self._description}, {self._params}, {self._engine_state_file}, ' \
            f'{self._dr_mgr}, {self._dr_files}, {self._actions})'

    def __str__(self):
        return f'SEScenario:\n\tName: {self._name}\n\tDescription: {self._description}\n\tPatient Configuration: ' \
            f'{self._params}\n\tEngine State File: {self._engine_state_file}\n\tData Request Manager: {self._dr_mgr}\n\t' \
            f'Data Request Files: {self._dr_files}\n\tActions: {self._actions}'

    def clear(self) -> None:
        self._name = ""
        self._description = ""
        self._params = None
        self._engine_state_file = None
        self._dr_mgr.clear()
        self._dr_files = []
        self._actions = []

    def is_valid(self) -> bool:
        if len(self._actions) == 0:
            return False
        if not self.has_patient_configuration() and not self.has_engine_state():
            return False
        return True

    def get_name(self) -> str:
        return self._name
    def set_name(self, name: str) -> None:
        self._name = name
    def has_name(self) -> bool:
        return len(self._name) > 0
    def invalidate_name(self) -> None:
        self._name = ""

    def get_description(self) -> str:
        return self._description
    def set_description(self, description: str) -> None:
        self._description = description
    def has_description(self) -> bool:
        return len(self._description) > 0
    def invalidate_description(self) -> None:
        self._description = ""

    def get_engine_state(self) -> str:
        return self._engine_state_file
    def set_engine_state(self, state_file: str) -> None:
        self._engine_state_file = state_file
    def has_engine_state(self) -> bool:
        if self.has_patient_configuration():
            return False
        return self._engine_state_file is not None and len(self._engine_state_file) > 0
    def invalidate_engine_state(self) -> None:
        self._engine_state_file = None

    def get_patient_configuration(self) -> SEPatientConfiguration:
        if self._params is None:
            self._params = SEPatientConfiguration()
        return self._params
    def has_patient_configuration(self) -> bool:
        return self._params is not None and self._params.is_valid()
    def invalidate_patient_configuration(self) -> None:
        self._params = None

    def get_actions(self) -> List[SEAction]:
        return self._actions

    def get_data_request_manager(self) -> SEDataRequestManager:
        return self._dr_mgr

    def get_data_request_files(self) -> List[str]:
        return self._dr_files


class SEScenarioExec:
    __slots__ = ["_log_to_console", "_data_root_directory", "_output_root_directory", "_organize_output_directory",
                 "_auto_serialize_after_actions", "_auto_serialize_period_s", "_time_stamp_serialized_states",
                 "_engine_configuration_content", "_engine_configuration_filename", "_scenario_content", "_scenario_filename",
                 "_scenario_directory", "_scenario_log_filename", "_scenario_log_directory","_data_request_files_search",
                 "_content_format", "_thread_count"]

    def __init__(self):
        self.clear()

    def clear(self) -> None:
        self._log_to_console = eSwitch.On
        self._data_root_directory = "./"
        self._output_root_directory = "./"
        self._organize_output_directory = eSwitch.Off

        self._auto_serialize_after_actions = eSwitch.Off
        self._auto_serialize_period_s = 0
        self._time_stamp_serialized_states = eSwitch.On

        self._engine_configuration_content = ""
        self._engine_configuration_filename = ""

        self._scenario_content = ""
        self._scenario_filename = ""
        self._scenario_directory = ""
        self._scenario_log_filename = ""
        self._scenario_log_directory = ""

        self._data_request_files_search = []

        self._content_format = eSerializationFormat.JSON
        self._thread_count = -1

    def get_log_to_console(self) -> eSwitch:
        return self._log_to_console
    def set_log_to_console(self, s: eSwitch) -> None:
        self._log_to_console = s

    def get_data_root_directory(self) -> str:
        return self._data_root_directory
    def set_data_root_directory(self, s: str) -> None:
        self._data_root_directory = s

    def get_output_root_directory(self) -> str:
        return self._output_root_directory
    def set_output_root_directory(self, s: str) -> None:
        self._output_root_directory = s

    def get_organize_output_directory(self) -> eSwitch:
        return self._organize_output_directory
    def set_organize_output_directory(self, s: eSwitch) -> None:
        self._organize_output_directory = same

    def get_auto_serialize_after_actions(self) -> eSwitch:
        return self._auto_serialize_after_actions
    def set_auto_serialize_after_actions(self, s: eSwitch) -> None:
        self._auto_serialize_after_actions = s

    def get_auto_serialize_period_s(self) -> float:
        return self._auto_serialize_period_s
    def set_auto_serialize_period_s(self, p: float) -> None:
        self._auto_serialize_period_s = p

    def get_time_stamp_serialized_states(self) -> eSwitch:
        return self._time_stamp_serialized_states
    def set_time_stamp_serialized_states(self, s: eSwitch) -> None:
        self._time_stamp_serialized_states = s

    def get_engine_configuration_content(self) -> str:
        return self._engine_configuration_content
    def set_engine_configuration_content(self, s: str) -> None:
        self._engine_configuration_content = s
        self._engine_configuration_filename = ""

    def get_engine_configuration_filename(self) -> str:
        return self._engine_configuration_filename
    def set_engine_configuration_filename(self, s: str) -> None:
        self._engine_configuration_content = ""
        self._engine_configuration_filename = s

    def get_scenario_content(self) -> str:
        return self._scenario_content
    def set_scenario_content(self, s: str) -> None:
        self._scenario_content = s
        self._scenario_filename = ""
        self._scenario_directory = ""
        self._scenario_log_filename = ""
        self._scenario_log_directory = ""

    def get_scenario_filename(self) -> str:
        return self._scenario_filename
    def set_scenario_filename(self, s: str) -> None:
        self._scenario_content = ""
        self._scenario_filename = s
        self._scenario_directory = ""
        self._scenario_log_filename = ""
        self._scenario_log_directory = ""

    def get_scenario_directory(self) -> str:
        return self._scenario_directory
    def set_scenario_directory(self, s: str) -> None:
        self._scenario_content = ""
        self._scenario_filename = ""
        self._scenario_directory = s
        self._scenario_log_filename = ""
        self._scenario_log_directory = ""

    def get_scenario_log_filename(self) -> str:
        return self._scenario_log_filename
    def set_scenario_log_filename(self, s: str) -> None:
        self._scenario_content = ""
        self._scenario_filename = ""
        self._scenario_directory = ""
        self._scenario_log_filename = s
        self._scenario_log_directory = ""

    def get_scenario_log_directory(self) -> str:
        return self._scenario_log_directory
    def set_scenario_log_directory(self, s: str) -> None:
        self._scenario_content = ""
        self._scenario_filename = ""
        self._scenario_directory = ""
        self._scenario_log_filename = ""
        self._scenario_log_directory = s

    def get_data_request_files_search(self) -> List[str]:
        return self._data_request_files_search

    def get_content_format(self) -> eSerializationFormat:
        return self._content_format
    def set_content_format(self, s: eSerializationFormat) -> None:
        self._content_format = s

    def get_thread_count(self) -> int:
        return self._thread_count
    def set_thread_count(self, c: int) -> None:
        self._thread_count = c


class SEScenarioLog:
    __slots__ = ("_log_file", "_patient", "_actions", "_actions_filter", "_events", "_events_filter")

    def __init__(
        self,
        log_file: Path,
        actions_filter: Optional[List[str]] = None,
        events_filter: Optional[List[str]] = None
    ):
        """
        Base class to parse log content. Inheriting classes should implement method(s) to
        use this extracted info.

        :param log_file: Path to log file
        :param actions_filter: Exclude actions containing any of these strings
        :param events_filter: Exclude events containing any of these strings
        """
        if not log_file.is_file():
            raise ValueError(f"Log file ({log_file}) does not exist/is not a file")
        self._log_file = log_file
        self._actions_filter = actions_filter
        self._events_filter = events_filter
        self._patient = SEPatient()
        self._actions = list()
        self._events = list()

    def _process_log(self) -> None:
        """
        Extracts patient, actions, and events from log
        """
        def _parse_dict(
            tag: str,
            lines: List[str],
            matches_out: List[Tuple[SEScalarTime, str]],
            max_matches: Optional[int]=None
        ) -> bool:
            """
            Parses dict-like structures following given tag

            :param tag: Tag identifying content to extract
            :param lines: Log text
            :param matches_out: List where matches will be added
            :param max_matches: If provided, limit the number of matches to this value

            :return: Whether parsing was successful
            """
            idx = 0
            while idx < len(lines):
                line = lines[idx]
                tag_idx = line.find(tag)
                if tag_idx != -1:
                    tag_text = line
                    # Extract time
                    match = re.search(r'\[(?P<time>\d*\.?\d*)\((?P<time_unit>.*)\)\]', tag_text)
                    if match is None:
                        return False
                    tag_time = float(match['time'])
                    time_unit = get_unit(match['time_unit'])
                    tag_text = tag_text[(tag_idx+len(tag)):].lstrip()

                    # Find blank line indicating end of tag match
                    while (idx + 1) < len(lines) and len(lines[idx+1].strip()) != 0:
                        idx += 1
                        line = lines[idx]
                        tag_text += line

                    matches_out.append((SEScalarTime(tag_time, time_unit), tag_text))
                    if max_matches is not None and len(matches_out) >= max_matches:
                        break
                idx += 1
            return True

        def _parse_line(
            tag: str,
            lines: List[str],
            matches_out: List[Tuple[SEScalarTime, str]],
            max_matches: Optional[int]=None
        ):
            """
            Parses single line containing given tag

            :param tag: Tag identifying content to extract
            :param lines: Log text
            :param matches_out: List where matches will be added
            :param max_matches: If provided, limit the number of matches to this value

            :return: Whether parsing was successful
            """
            for line in lines:
                tag_idx = line.find(tag)
                if tag_idx != -1:
                    tag_text = line
                    # Get time and find beginning of actual tag text content
                    match = re.search(
                        r'\[(?P<time>\d*\.?\d*)\((?P<time_unit>.*)\)\]\s*(?P<tag>' +
                            re.escape(tag) +
                            r'\s*(?:\d*\.?\d*\(s\),)?)',
                        tag_text
                    )
                    if match is None:
                        return False
                    tag_time = float(match['time'])
                    time_unit = get_unit(match['time_unit'])
                    tag_text = tag_text[(tag_idx + len(match['tag'])):].strip()

                    matches_out.append((SEScalarTime(tag_time, time_unit), tag_text))
                    if max_matches is not None and len(matches_out) >= max_matches:
                        break

            return True

        def _filter(item: str, filter_out: List[str]) -> bool:
            """
            :param item: Value to check
            :param filter_out: Values that given item shoud not contain

            :return: Whether given item passes given filters
            """
            for filter in filter_out:
                if filter in item:
                    return False
            return True

        with open(self._log_file) as f:
            lines = f.readlines()

            # Get patient
            patient_text = list()
            if not _parse_dict(tag="[Patient]", lines=lines, max_matches=1, matches_out=patient_text) or not patient_text:
                _pulse_logger.warning(f"Could not extract patient info: {self._log_file}")
            else:
                serialize_patient_from_string(patient_text[0][1], self._patient, eSerializationFormat.TEXT)

            # Get events
            if not _parse_line(tag="[Event]", lines=lines, matches_out=self._events):
                _pulse_logger.warning(f"Could not extract events: {self._log_file}")
            elif self._events_filter:
                self._events = [
                    e
                    for e in self._events if _filter(item=e[1], filter_out=self._events_filter)
                ]

            # Get actions
            if not _parse_dict(tag="[Action]", lines=lines, matches_out=self._actions):
                _pulse_logger.warning(f"Could not extract actions: {self._log_file}")
            elif self._actions_filter:
                self._actions = [
                    a
                    for a in self._actions if _filter(item=a[1], filter_out=self._actions_filter)
                ]
