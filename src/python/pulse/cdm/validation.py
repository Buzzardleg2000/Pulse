# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import numpy as np
from enum import Enum
from typing import Dict, List, Optional

from pulse.cdm.patient import SEPatient

class SEValidationTarget:
    __slots__ = ["_header", "_reference", "_notes", "_table_formatting",
                 "_target", "_target_min", "_target_max"]

    def __init__(self):
        self.clear()

    def __repr__(self) -> str:
        return f'SEValidationTarget({self._header}, {self._reference}, {self._notes}, ' \
               f'{self._table_formatting}, {self._target}, {self._target_min}, {self._target_max})'

    def __str__(self) -> str:
        return f'SEValidationTarget:' \
                f'\n\tHeader: {self._header}' \
                f'\n\tReference: {self._reference}' \
                f'\n\tNotes: {self._notes}' \
                f'\n\tTable Formatting: {self._table_formatting}' \
                f'\n\tTarget: {self._target}' \
                f'\n\tTarget Range: [{self._target_min}, {self._target_max}]'

    def is_valid(self) -> bool:
        if np.isnan(self._target) and np.isnan(self._target_max):
            return False
        return True

    def clear(self) -> None:
        self._header = ""
        self._reference = ""
        self._notes = ""
        self._table_formatting = None
        self._target = np.nan
        self._target_min = np.nan
        self._target_max = np.nan

    def get_header(self) -> str:
        return self._header
    def set_header(self, h: str):
        self._header = h

    def has_reference(self) -> bool:
        return self._reference != ""
    def get_reference(self) -> str:
        return self._reference
    def set_reference(self, r: str):
        self._reference = r

    def get_notes(self) -> str:
        return self._notes
    def set_notes(self, n: str):
        self._notes = n

    def has_table_formatting(self) -> bool:
        return self._table_formatting is not None
    def get_table_formatting(self) -> Optional[str]:
        return self._table_formatting
    def set_table_formatting(self, specification: str) -> None:
        self._table_formatting = specification
    def invalidate_table_formatting(self) -> None:
        self._table_formatting = None

    def get_target_maximum(self) -> float:
        return self._target_max
    def get_target_minimum(self) -> float:
        return self._target_min
    def get_target(self) -> float:
        return self._target


class SESegmentValidationTarget(SEValidationTarget):
    __slots__ = ["_comparison_type", "_target_segment", "_computed_value", "_error_value"]

    class eComparisonType(Enum):
        NotValidating = 0
        EqualToValue = 1
        EqualToSegment = 2
        GreaterThanValue = 3
        GreaterThanSegment = 4
        LessThanValue = 5
        LessThanSegment = 6
        TrendsToValue = 7
        TrendsToSegment = 8
        Range = 9

    def __init__(self):
        super().__init__()
        self.clear()

    def __repr__(self):
        return f'SESegmentValidationTarget({super().__repr__()}, {self._segment}, {self._comparison_type}' \
               f'{self._computed_value}, {self._error_value})'

    def clear(self):
        super().clear()
        self._comparison_type = self.eComparisonType.NotValidating
        self._target_segment = np.nan
        self._computed_value = None
        self._error_value = None

    def get_comparison_type(self) -> eComparisonType:
        return self._comparison_type
    def get_target_segment(self) -> int:
        return self._target_segment

    def set_equal_to_segment(self, segment: int):
        self._comparison_type = self.eComparisonType.EqualToSegment
        self._target = np.nan
        self._target_max = np.nan
        self._target_min = np.nan
        self._target_segment = segment
    def set_equal_to_value(self, d: float):
        self._comparison_type = self.eComparisonType.EqualToValue
        self._target = d
        self._target_max = d
        self._target_min = d
        self._target_segment = 0
    def set_greater_than_segment(self, segment: int):
        self._comparison_type = self.eComparisonType.GreaterThanSegment
        self._target = np.nan
        self._target_max = np.nan
        self._target_min = np.nan
        self._target_segment = segment
    def set_greater_than_value(self, d: float):
        self._comparison_type = self.eComparisonType.GreaterThanValue
        self._target = d
        self._target_max = d
        self._target_min = d
        self._target_segment = 0
    def set_less_than_segment(self, segment: int):
        self._comparison_type = self.eComparisonType.LessThanSegment
        self._target = np.nan
        self._target_max = np.nan
        self._target_min = np.nan
        self._target_segment = segment
    def set_less_than_value(self, d: float):
        self._comparison_type = self.eComparisonType.LessThanValue
        self._target = d
        self._target_max = d
        self._target_min = d
        self._target_segment = 0
    def set_trends_to_segment(self, segment: int):
        self._comparison_type = self.eComparisonType.TrendsToSegment
        self._target = np.nan
        self._target_max = np.nan
        self._target_min = np.nan
        self._target_segment = segment
    def set_trends_to_value(self, d: float):
        self._comparison_type = self.eComparisonType.TrendsToValue
        self._target = d
        self._target_max = d
        self._target_min = d
        self._target_segment = 0
    def set_range(self, min: float, max: float):
        self._comparison_type = self.eComparisonType.Range
        self._target = np.nan
        self._target_max = max
        self._target_min = min
        self._target_segment = 0

    def has_computed_value(self) -> bool:
        return self._computed_value is not None
    def get_computed_value(self) -> Optional[float]:
        return self._computed_value
    def set_computed_value(self, val: float) -> None:
        self._computed_value = val
    def invalidate_computed_value(self) -> None:
        self._computed_value = None

    def has_error_value(self) -> bool:
        return self._error_value is not None
    def get_error_value(self) -> Optional[float]:
        return self._error_value
    def set_error_value(self, err: float) -> None:
        self._error_value = err
    def invalidate_error_value(self) -> None:
        self._error_value = None


class SESegmentValidationSegment:
    __slots__ = ["_segment_id", "_notes", "_validation_targets", "_actions"]

    def __init__(self):
        self._segment_id = 0
        self._notes = ""
        self._validation_targets = []

        # Not serializing
        self._actions = []

    def clear(self) -> None:
        self._notes = ""
        self._validation_targets = []

        self._actions = []

    def get_segment_id(self) -> int:
        return self._segment_id
    def set_segment_id(self, id: int) -> None:
        self._segment_id = id

    def has_notes(self) -> bool:
        return len(self._notes) > 0
    def get_notes(self) -> str:
        return self._notes
    def set_notes(self, notes: str) -> None:
        self._notes = notes
    def invalidate_notes(self) -> None:
        self._notes = ""

    def has_validation_targets(self) -> bool:
        return len(self._validation_targets) > 0
    def get_validation_targets(self) -> List[SESegmentValidationTarget]:
        return self._validation_targets
    def add_validation_target(self, tgt: SESegmentValidationTarget) -> None:
        self._validation_targets.append(tgt)
    def set_validation_targets(self, tgts: List[SESegmentValidationTarget]) -> None:
        self._validation_targets = tgts
    def invalidate_validation_targets(self) -> None:
        self._validation_targets = []

    def has_actions(self) -> bool:
        return len(self._actions) > 0
    def get_actions(self) -> List[str]:
        return self._actions
    def set_actions(self, actions: List[str]) -> None:
        self._actions = actions
    def invalidate_actions(self) -> None:
        self._actions = []

class SESegmentValidationPipelineConfig:
    __slots__ = ["_plotters"]
    def __init__(self):
        self.clear()

    def clear(self) -> None:
        self._plotters = []

    def get_plotters(self) -> []:
        return self._plotters


class SETimeSeriesValidationTarget(SEValidationTarget):
    __slots__ = ["_target_type", "_comparison_type", "_patient_specific",
                 "_computed_value", "_error_value"]

    class eComparisonType(Enum):
        NotValidating = 0
        EqualToValue = 1
        Range = 5

    class eTargetType(Enum):
        Mean = 0
        Minimum = 1
        Maximum = 2
        MeanPerIdealWeight_kg = 3
        MinPerIdealWeight_kg = 4
        MaxPerIdealWeight_kg = 5

    def __init__(self):
        super().__init__()
        self.clear()

    def clear(self):
        super().clear()
        self._comparison_type = self.eComparisonType.NotValidating
        self._target_type = self.eTargetType.Mean
        self._computed_value = None
        self._error_value = None
        self._patient_specific = None

    def is_evaluated(self) -> bool:
        return self.has_computed_value() and self.has_error_value()

    def get_comparison_type(self) -> eComparisonType:
        return self._comparison_type
    def get_target_type(self) -> eTargetType:
        return self._target_type

    def set_equal_to(self, d: float, t: eTargetType):
        self._comparison_type = self.eComparisonType.EqualToValue
        self._target_type = t
        self._target = d
        self._target_max = d
        self._target_min = d
    def set_range(self, min: float, max: float, t: eTargetType):
        self._comparison_type = self.eComparisonType.Range
        self._target_type = t
        self._target = np.nan
        self._target_max = max
        self._target_min = min

    def has_computed_value(self) -> bool:
        return self._computed_value is not None
    def get_computed_value(self) -> Optional[float]:
        return self._computed_value
    def set_computed_value(self, val: float) -> None:
        self._computed_value = val
    def invalidate_computed_value(self) -> None:
        self._computed_value = None

    def has_error_value(self) -> bool:
        return self._error_value is not None
    def get_error_value(self) -> Optional[float]:
        return self._error_value
    def set_error_value(self, err: float) -> None:
        self._error_value = err
    def invalidate_error_value(self) -> None:
        self._error_value = None

    def has_patient_specific_setting(self) -> bool:
        return self._patient_specific is not None
    def is_patient_specific(self) -> Optional[bool]:
        return self._patient_specific
    def set_patient_specific_setting(self, patient_specific: bool) -> None:
        self._patient_specific = patient_specific
    def invalidate_patient_specific_setting(self) -> None:
        self._patient_specific = None


class SEPatientTimeSeriesValidation:
    __slots__ = ["_patient", "_targets"]

    def __init__(self):
        self.clear()

    def clear(self) -> None:
        self._patient = None
        self._targets = dict()

    def get_targets(self) -> Dict[str, List[SETimeSeriesValidationTarget]]:
        return self._targets
    def invalidate_targets(self) -> None:
        self._targets = dict()

    def has_patient(self) -> bool:
        return self._patient is not None
    def get_patient(self) -> SEPatient:
        from pulse.cdm.patient import SEPatient
        if self._patient is None:
            self._patient = SEPatient()
        return self._patient
    def invalidate_patient(self) -> None:
        self._patient = None

