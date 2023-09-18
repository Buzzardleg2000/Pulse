# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import logging
import numpy as np
import pandas as pd
from pathlib import Path
from typing import List, Optional

import PyPulse
from pulse.cdm.engine import SETimeSeriesValidationTarget
from pulse.cdm.utils.csv_utils import read_csv_into_df
from pulse.cdm.utils.markdown import table
from pulse.cdm.utils.math_utils import generate_percentage_span, percent_tolerance
from pulse.cdm.io.engine import serialize_time_series_validation_target_map_from_file


_pulse_logger = logging.getLogger('pulse')


def validate(targets_filename: Path, csv_filename: Path, table_dir: Path) -> None:
    _pulse_logger.info(f"Validating {csv_filename} against {targets_filename}")
    targets = serialize_time_series_validation_target_map_from_file(targets_filename)
    df = read_csv_into_df(csv_filename)

    headers = ["Property Name", "Expected Value", "Engine Value", "Percent Error", "Notes"]
    fields = list(range(len(headers)))
    align = [('<', '<')] * len(headers)

    table_dir.mkdir(parents=True, exist_ok=True)
    for tgt_table, tgts in targets.items():
        # No validation targets for this table
        if not tgts:
            continue

        # Evaluate results against targets
        table_data = []
        for tgt in tgts:
            tgt_eval = evaluate(tgt, df)
            if tgt_eval:
                table_data.append(tgt_eval)

        # Write out table
        md_filename = table_dir / f"{tgt_table}ValidationTable.md"
        with open(md_filename, "w") as md_file:
            _pulse_logger.info(f"Writing {md_filename}")
            table(md_file, table_data, fields, headers, align)


def evaluate(tgt: SETimeSeriesValidationTarget, results: pd.DataFrame) -> List[str]:
    tgt_header = tgt.get_header()
    compare_type = tgt.get_comparison_type()
    target_type = tgt.get_target_type()

    if compare_type == SETimeSeriesValidationTarget.eComparisonType.NotValidating:
        return list()

    epsilon = 1E-9
    value_precision = 4
    percent_precision = 1
    err_str = ""

    def _get_header(header: str) -> Optional[pd.Series]:
        series = None
        if header not in results.columns:
            # Attempt to locate header with different unit and convert
            paren_idx = header.find("(")
            if paren_idx != -1:
                unitless_header = header[:(paren_idx+1)]
                desired_unit = header[(paren_idx+1):-1].replace("_", " ")
                for h in results.columns:
                    if h.startswith(unitless_header):
                        results_paren_idx = h.find("(")
                        if results_paren_idx != -1:
                            results_unit = h[(results_paren_idx+1):-1].replace("_", " ")
                            series = results[h].map(lambda x: PyPulse.convert(x, results_unit, desired_unit)).squeeze()
                            break
            if series is None:
                raise ValueError(f"Missing header: {header}")
        else:
            series = results[header].squeeze()

        return series

    header_series = _get_header(tgt_header)

    # Normalize series based on target type
    if (
       target_type == SETimeSeriesValidationTarget.eTargetType.MeanPerIdealWeight_kg or
       target_type == SETimeSeriesValidationTarget.eTargetType.MinPerIdealWeight_kg or
       target_type == SETimeSeriesValidationTarget.eTargetType.MaxPerIdealWeight_kg
    ):
        ideal_weight_kg_series = _get_header("Patient-IdealBodyWeight(kg)")
        header_series = header_series.div(ideal_weight_kg_series)

    # Compute requested comparison value
    if (
        target_type == SETimeSeriesValidationTarget.eTargetType.Minimum or
        target_type == SETimeSeriesValidationTarget.eTargetType.MinPerIdealWeight_kg
    ):
        comparison_value = header_series.min()
    elif (
        target_type == SETimeSeriesValidationTarget.eTargetType.Maximum or
        target_type == SETimeSeriesValidationTarget.eTargetType.MaxPerIdealWeight_kg
    ):
        comparison_value = header_series.max()
    elif (
        target_type == SETimeSeriesValidationTarget.eTargetType.Mean or
        target_type == SETimeSeriesValidationTarget.eTargetType.MeanPerIdealWeight_kg
    ):
        comparison_value = header_series.mean() if not header_series.empty else np.nan
    else:
        raise ValueError(f"Unknown target type: {target_type}")
    engine_val_str = f"{target_type.name.replace('_kg', '(kg)')} of {comparison_value:.{value_precision}G}"

    # Compute error
    tgt_min = tgt.get_target_minimum()
    tgt_max = tgt.get_target_maximum()
    min_error = percent_tolerance(tgt_min, comparison_value, epsilon)
    max_error = percent_tolerance(tgt_max, comparison_value, epsilon)

    # No error if we are in range
    error = None
    if comparison_value >= tgt_min and comparison_value <= tgt_max:
        error = 0.
    elif comparison_value > tgt_max:
        error = max_error
    elif comparison_value < tgt_min:
        error = min_error
    elif np.isnan(comparison_value):
        error = np.nan
    else:
        raise ValueError("Unable to determine error")

    # Close enough
    if abs(error) < 1E-15:
        error = 0.

    # Generate expected value string
    if compare_type == SETimeSeriesValidationTarget.eComparisonType.EqualToValue:
        expected_str = f"{tgt.get_target():.{value_precision}G}"
    elif compare_type == SETimeSeriesValidationTarget.eComparisonType.Range:
        expected_str = f"[{tgt_min:.{value_precision}G},{tgt_max:.{value_precision}G}]"
    else:
        raise ValueError(f"Unknown comparison type: {compare_type}")

    if tgt.get_reference():
        references = [ref.strip() for ref in tgt.get_reference().replace("\n", "").split(",")]
        for ref in references:
            expected_str += f" @cite {ref}"

    err_str = generate_percentage_span(error, percent_precision)

    return [
        tgt_header,
        expected_str if expected_str else "&nbsp;",
        engine_val_str if engine_val_str else "&nbsp;",
        err_str if err_str else "&nbsp;",
        tgt.get_notes() if tgt.get_notes() else "&nbsp;",
    ]
