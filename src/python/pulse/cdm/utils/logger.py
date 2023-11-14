# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from typing import List, NamedTuple, Set
import logging
import re

from pulse.cdm.engine import SEAction


_pulse_logger = logging.getLogger('pulse')


def get_severity_str(severity: float) -> str:
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


def break_camel_case(string: str):
    # https://stackoverflow.com/a/9283563
    camel_case_regex = r"""
        (            # start the group
            # alternative 1
        (?<=[a-z])  # current position is preceded by a lower char
                    # (positive lookbehind: does not consume any char)
        [A-Z]       # an upper char
                    #
        |   # or
            # alternative 2
        (?<!\A)     # current position is not at the beginning of the string
                    # (negative lookbehind: does not consume any char)
        [A-Z]       # an upper char
        (?=[a-z])   # matches if next char is a lower char
                    # lookahead assertion: does not consume any char
        )           # end the group"""

    return re.sub(camel_case_regex, r' \1', string, flags=re.VERBOSE)

class ePrettyPrintType(Enum):
    Action = 0
    Condition = 1
def pretty_print(string: str, print_type: ePrettyPrintType, preserve_camel_case: bool = False):
    ret = ""
    string = string.replace('"', '')
    string = string.replace('{', '')
    string = string.replace('}', '')
    string = string.replace(',', '')
    string = string.replace('[', '')
    string = string.replace(']', '')

    lines = [s.rstrip() for s in string.splitlines() if s]

    idx = 0
    while idx < len(lines):
        line = lines[idx].rstrip()

        if len(line) == 0:
            idx += 1
            continue
        if print_type.name in line:
            idx += 1
            continue
        if "ReadOnly" in line:
            idx += 1
            continue
        if line.endswith("Comment:"):
            idx += 1
            continue

        found_unit = False
        if idx+1 < len(lines):
            peek = lines[idx+1]
            if "Scalar" in peek:
                peek = lines[idx+2]
                line = ''.join([line, peek[(peek.find("Value")+5):].rstrip()])
                if (idx+3) < len(lines) and "Unit:" in lines[idx+3]:
                    idx += 3
                    peek = lines[idx]
                    line = ''.join([line, peek[(peek.find("Unit:")+5):].rstrip()])
                    found_unit = True
                else:
                    idx += 2

        if not preserve_camel_case and "Comment:" not in line:
            # Don't change files or units
            if found_unit or "Unit:" in line or "File" in line:
                colon = line.rfind(":")
                if colon != -1:
                    line = break_camel_case(line[:colon]) + line[colon:]
            else:
                line = break_camel_case(line)

        ret = ''.join([ret, line, "\n"])
        idx += 1

    ret = ret.replace('::', ':')

    return ret

class eActionEventCategory(Enum):
    ACTION = 0
    EVENT = 1
class LogActionEvent(NamedTuple):
    time: float
    text: str
    category: eActionEventCategory
def parse_events(log_file: str, omit: List[str] = []):
    event_tag = "[Event]"
    events = []
    with open(log_file) as f:
        lines = f.readlines()
        for line in lines:
            if len(line) == 0:
                continue
            event_idx = line.find(event_tag)
            if event_idx == -1:
                continue
            else:
                event_text = line[(event_idx+len(event_tag)):].strip()
                end_time_idx = event_text.find("(s)")
                if end_time_idx == -1:
                    end_time_idx = event_text.find(",")
                event_time = float(event_text[:end_time_idx].strip())
                event_text = event_text[(event_text.find(",")+1):].strip()

                # Check to see if it should be omitted
                keep_event = True
                for o in omit:
                    if o in event_text:
                        keep_event = False
                        break
                if not keep_event:
                    continue

                events.append(LogActionEvent(event_time, event_text, eActionEventCategory.EVENT))

    return events
def parse_actions(log_file: str, omit: List[str] = []):
    advTime = "AdvanceTime"
    omit.append(advTime)
    action_tag = "[Action]"
    actions = []
    with open(log_file) as f:
        lines = f.readlines()
        idx = 0
        while idx < len(lines):
            line = lines[idx]
            if len(line) == 0:
                idx += 1
                continue
            action_idx = line.find(action_tag)
            if action_idx == -1:
                idx += 1
                continue
            elif advTime in line:
                idx += 1
                continue
            else:
                action_text = line
                # Group 0: Entire match
                # Group 1: Time
                match = re.search(r'\[(\d*\.?\d*)\(.*\)\]', action_text)
                if match is None:
                    _pulse_logger.error("Could not parse actions from " + str(log_file))
                    return actions
                action_time = float(match.group(1))
                action_text = action_text[(action_idx+len(action_tag)):].lstrip()

                # Find blank line at end of action
                while (idx + 1) < len(lines) and len(lines[idx+1].strip()) != 0:
                    idx += 1
                    line = lines[idx]
                    action_text = ''.join([action_text, line])

                # Check to see if it should be omitted
                keep_action = True
                for o in omit:
                    if o in action_text:
                        keep_action = False
                        break
                if not keep_action:
                    idx += 1
                    continue

                action_text = pretty_print(action_text, ePrettyPrintType.Action)

                # Remove leading spaces on each line
                action_text = '\n'.join([s.strip() for s in action_text.splitlines()])

                actions.append(LogActionEvent(action_time, action_text, eActionEventCategory.ACTION))

            idx += 1

    return actions
