# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import json
import random
import logging
from string import Template
from typing import Any, Dict, Hashable, Iterable, List, NamedTuple, Optional, Tuple

from pulse.cdm.engine import SEEventChange
from pulse.cdm.scalars import TimeUnit, SEScalarTime
from pulse.cdm.scenario import SEObservationReportModule


_pulse_logger = logging.getLogger('pulse')


class TCCCUnstructuredText(SEObservationReportModule):
    __slots__ = ("_seed", "_injury_time", "_elapsed_time")

    TIME_s = "Time(s)"

    def __init__(self, seed: Optional[int]=None):
        self._headers = [
            self.TIME_s
        ]

        self._seed = seed
        if self._seed is not None:
            random.seed(self._seed)

        self._injury_time = None

        self._init_corpus()

    def _init_corpus(self):
        """
        Initialize corpus of template phrases.
        """
        self._elapsed_time = [
            Template("The patient was injured $elapsed_time_m ago."),
            Template("It's been $elapsed_time_m since the incident."),
            Template("$elapsed_time_m ago, the patient was injured."),
            Template("The incident occurred $elapsed_time_m earlier."),
            Template("$elapsed_time_m $elapsed_time_verb elapsed since injury."),
            Template("$elapsed_time_m $elapsed_time_verb passed since the event.")
        ]

    def _add_phrase(self, choices: List[Template], out_phrases: List[Template]) -> None:
        """
        Add a random phrase from given choices to list of phrases.
        """
        out_phrases.append(random.choice(choices))

    def handle_event(self, change: SEEventChange) -> None:
        """ Process given event change """
        # TODO: Handle events

    def handle_action(self, action: str, action_time: SEScalarTime) -> None:
        """ Process action """
        # TODO: Don't assume first action is injury
        if self._injury_time is None:
            self._injury_time = action_time

    def update(self, data_slice: NamedTuple, slice_idx: Dict[str, int]) -> Iterable[Tuple[Hashable, Any]]:
        """
        Generate unstructured text from current vitals and handled actions and events.
        """
        phrases = list()
        template_vals = dict()

        if self._injury_time is not None:
            time_elapsed_s = data_slice[slice_idx[self.TIME_s]] - self._injury_time.get_value(TimeUnit.s)
            time_elapsed_min = int(round(SEScalarTime(time_elapsed_s, TimeUnit.s).get_value(TimeUnit.min)))

            template_vals["elapsed_time_m"] = f"{time_elapsed_min} " \
                                              f"{'minutes' if time_elapsed_min != 1 else 'minute'}"
            template_vals["elapsed_time_verb"] = "have" if time_elapsed_min != 1 else "has"

            self._add_phrase(choices=self._elapsed_time, out_phrases=phrases)

        # TODO: Add other phrases

        # Remove empty phrases, substitute template values, and shuffle
        phrases = [phrase.safe_substitute(template_vals) for phrase in phrases if phrase]
        random.shuffle(phrases)

        return [
            ("UnstructuredText", " ".join(phrases))
        ]
