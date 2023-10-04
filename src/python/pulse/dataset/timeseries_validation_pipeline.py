# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
import argparse
from pathlib import Path
from collections import namedtuple
from typing import Dict, List, Optional, Tuple, Union

from pulse.cdm.engine import SETimeSeriesValidationTarget, SEPatientTimeSeriesValidation
from pulse.cdm.utils.file_utils import get_validation_dir
from pulse.cdm.scalars import get_unit
from pulse.dataset.timeseries_dataset_reader import generate_validation_targets, extract_patient
from pulse.dataset.timeseries_validation import validate, generate_validation_tables
from pulse.cdm.io.engine import serialize_patient_time_series_validation_list_to_file


_pulse_logger = logging.getLogger('pulse')


def gen_system_targets(
    log_file: Path,
    patient_validation: SEPatientTimeSeriesValidation,
    output_file: Optional[Path]=None
) -> bool:
    """
    Generate system validation timeseries validation targets.

    :param log_file: Path to log file. Could alternatively be the relevant
        patient file.
    :param patient_validation: Where generated targets will be stored.
    :param output_file: (Optional) If provided, generated targets will be
        serialized here.

    :return: Whether or not validation target generation was successful.
    """
    xls_file = Path(get_validation_dir() + "/SystemValidationData.xlsx")

    return generate_validation_targets(
        xls_file=xls_file,
        patient_file=log_file,
        patient_validation=patient_validation,
        output_file=output_file
    )


def gen_patient_targets(
    log_file: Path,
    patient_validation: SEPatientTimeSeriesValidation,
    output_file: Optional[Path]=None
) -> bool:
    """
    Generate patient validation timeseries validation targets.

    :param log_file: Path to log file. Could alternatively be the relevant
        patient file.
    :param patient_validation: Where generated targets will be stored.
    :param output_file: (Optional) If provided, generated targets will be
        serialized here.

    :return: Whether or not validation target generation was successful.
    """
    # Get patient, extracting from log if not already done
    if not patient_validation.has_patient():
        p = patient_validation.get_patient()
        p.copy(extract_patient(patient_file=patient_file))
    else:
        p = patient_validation.get_patient()

    table_name = "Patient"
    if table_name not in patient_validation.get_targets():
        patient_validation.get_targets()[table_name] = list()
    patient_val_tgts = patient_validation.get_targets()[table_name]

    PatientValidation = namedtuple('PatientValidation', ["header", "unit", "table_precision", "patient_attr"])
    validated_headers = [
        PatientValidation(
            header="TotalMetabolicRate",
            unit="kcal/day",
            table_precision=".1f",
            patient_attr="_basal_metabolic_rate"
        ), PatientValidation(
            header="BloodVolume",
            unit="mL",
            table_precision=".1f",
            patient_attr="_blood_volume_baseline"
        ), PatientValidation(
            header="DiastolicArterialPressure",
            unit="mmHg",
            table_precision=".1f",
            patient_attr="_diastolic_arterial_pressure_baseline"
        ), PatientValidation(
            header="HeartRate",
            unit="1/min",
            table_precision=".0f",
            patient_attr="_heart_rate_baseline"
        ), PatientValidation(
            header="MeanArterialPressure",
            unit="mmHg",
            table_precision=".1f",
            patient_attr="_mean_arterial_pressure_baseline"
        ), PatientValidation(
            header="RespirationRate",
            unit="1/min",
            table_precision=".0f",
            patient_attr="_respiration_rate_baseline"
        ), PatientValidation(
            header="SystolicArterialPressure",
            unit="mmHg",
            table_precision=".1f",
            patient_attr="_systolic_arterial_pressure_baseline"
        ), PatientValidation(
            header="TidalVolume",
            unit="mL",
            table_precision=".1f",
            patient_attr="_tidal_volume_baseline"
        ), PatientValidation(
            header="Patient-ExpiratoryReserveVolume",
            unit="mL",
            table_precision=".1f",
            patient_attr="_expiratory_reserve_volume"
        ), PatientValidation(
            header = "Patient-FunctionalResidualCapacity",
            unit="mL",
            table_precision=".1f",
            patient_attr="_functional_residual_capacity"
        ), PatientValidation(
            header="Patient-InspiratoryCapacity",
            unit="mL",
            table_precision=".1f",
            patient_attr="_inspiratory_capacity"
        ), PatientValidation(
            header="Patient-InspiratoryReserveVolume",
            unit="mL",
            table_precision=".1f",
            patient_attr="_inspiratory_reserve_volume"
        ), PatientValidation(
            header="Patient-ResidualVolume",
            unit="mL",
            table_precision=".1f",
            patient_attr="_residual_volume"
        ), PatientValidation(
            header="Patient-TotalLungCapacity",
            unit="mL",
            table_precision=".1f",
            patient_attr="_total_lung_capacity"
        ), PatientValidation(
            header="Patient-VitalCapacity",
            unit="mL",
            table_precision=".1f",
            patient_attr="_vital_capacity"
        )
    ]

    for header, unit, table_precision, patient_attr in validated_headers:
        tgt = SETimeSeriesValidationTarget()
        tgt.set_header(f"{header}({unit})")
        tgt.set_patient_specific_setting(True)
        tgt.set_table_formatting(table_precision)
        algo = SETimeSeriesValidationTarget.eTargetType.Mean

        # Only create target if patient has this attribute
        if getattr(p, f"has{patient_attr}")():
            ref_val = getattr(p, f"get{patient_attr}")().get_value(units=get_unit(unit))
            tgt.set_equal_to(ref_val, algo)
            patient_val_tgts.append(tgt)

    return True


def timeseries_validation_pipeline(
    log_file: Path,
    csv_file: Path,
    table_dir: Optional[Path]=None,
    output_file: Optional[Path]=None
) -> SEPatientTimeSeriesValidation:
    """
    Processes given log and csv file through the timeseries validation
    pipeline.

    :param log_file: Path to log file.
    :param csv_file: Path to csv results file.
    :param table_dir: (Optional) Path to table output directory. If None,
        tables will not be generated.
    :param output_file: (Optional) If provided, generated targets will be
        serialized here.

    :return: Evaluated targets
    """
    val_tgts = SEPatientTimeSeriesValidation()
    if not gen_system_targets(log_file=log_file, patient_validation=val_tgts, output_file=None):
        _pulse_logger.error("Unable to generate system targets")
        return False

    if not gen_patient_targets(log_file=log_file, patient_validation=val_tgts, output_file=None):
        _pulse_logger.error("Unable to generate patient targets")
        return False

    validate(patient_validation=val_tgts, csv_filename=csv_file, output_file=output_file)

    # Generate Tables (Optional)
    if table_dir is not None:
        generate_validation_tables(target_map=val_tgts, table_dir=table_dir)

    return val_tgts


def bulk_timeseries_validation_pipeline(
    filename_base_paths: List[Path],
    output_file: Path,
    table_dir: Optional[Union[Path, List[Optional[Path]]]]=None,
    serialize_per_file: bool=False
) -> None:
    """
    Processes bulk list of files through timeseries validation pipeline to
    one output file, optionally saving individual validation tables. If an
    exception is raised during execution, an attempt will be made to serialize
    the current list of targets.

    :param filename_base_paths: List of file base paths to process. ".log" and
        ".csv" will be appended to these paths to locate log and results files.
    :param output_file: Generated targets will be serialized to this file.
    :param table_dir: (Optional) Path or list of paths indicating where tables
        should be generated for the corresponding filename_base_path. If only
        one path is provided, all tables will be generated in that directory.
        If None is provided (single or as list item), tables will not be
        generated.
    :param serialize_per_file: If true, each individual file's validation targets
        will be serialized to its own json file, in addition to the bulk file.
    """
    all_tgts = list()

    # If only one table dir provided, use the same table dir for every input file
    if not isinstance(table_dir, List):
        table_dirs = [table_dir] * len(filename_base_paths)
    else:
        table_dirs = table_dir

    try:
        # Run the pipeline on each input file
        for filename_base, t_dir in zip(filename_base_paths, table_dirs):
            # Check if the csv file should have *Results
            log_file = filename_base.parent / f"{filename_base.stem}.log"
            csv_file = filename_base.parent / f"{filename_base.stem}.csv"
            out_file = filename_base.parent / f"{filename_base.stem}.json" if serialize_per_file else None
            if not csv_file.exists():
                csv_file = filename_base.parent / f"{filename_base.stem}Results.csv"
                if not csv_file.exists():
                    _pulse_logger.error(f'CSV file does not exist for {filename_base_paths}')
                    continue
            tgts = timeseries_validation_pipeline(
                log_file=log_file,
                csv_file=csv_file,
                table_dir=t_dir,
                output_file=out_file
            )
            all_tgts.append(tgts)
    except:
        raise
    finally:
        # Save current target map, even if something goes wrong
        output_file.parent.mkdir(parents=True, exist_ok=True)

        _pulse_logger.info(f"Writing {output_file}")
        serialize_patient_time_series_validation_list_to_file(all_tgts, output_file)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    parser = argparse.ArgumentParser(description="Process the full pipeline for timeseries validation")
    parser.add_argument(
        "filename_base_paths",
        nargs='+',
        type=Path,
        help="base path(s) to files to process during this pipeline run." \
             "E.g. path/to/file1 indicates path/to/file1.csv and path/to/file1.log should be processed."
    )
    parser.add_argument(
        "-o", "--output-file",
        type=Path,
        default=None,
        help="Where generated targets should be serialized."
    )
    parser.add_argument(
        "-t", "--table-dir",
        type=Path,
        default=None,
        help="If provided, generate tables and write them in this directory."
    )
    parser.add_argument(
        "-s", "--serialize-per-file",
        action='store_true',
        help="Serialize generated targets into individual files (in addition to bulk serialization)."
    )
    opts = parser.parse_args()

    output_file = opts.output_file
    if len(opts.filename_base_paths) == 1 and output_file is None:
        base_path = opts.filename_base_paths[0]
        output_file = base_path.parent / f"{base_path.stem}ValidationTargets.json"
    elif output_file is None:
        _pulse_logger.error("You must provide an output file when multiple basenames are provided")
        sys.exit(1)

    bulk_timeseries_validation_pipeline(
        filename_base_paths=opts.filename_base_paths,
        output_file=output_file,
        table_dir=opts.table_dir,
        serialize_per_file=opts.serialize_per_file
    )
