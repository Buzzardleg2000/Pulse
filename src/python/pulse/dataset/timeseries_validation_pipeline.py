# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Union

from pulse.cdm.engine import SEPatientTimeSeriesValidation
from pulse.cdm.utils.file_utils import get_validation_dir
from pulse.dataset.timeseries_dataset_reader import generate_validation_targets
from pulse.dataset.timeseries_validation import validate, generate_validation_tables
from pulse.cdm.io.engine import serialize_patient_time_series_validation_list_to_file


_pulse_logger = logging.getLogger('pulse')


def gen_system_targets(log_file: Path, output_file: Path) -> Optional[SEPatientTimeSeriesValidation]:
    """
    Generate system validation timeseries validation targets.

    :param log_file: Path to log file. Could alternatively be the relevant
        patient file.
    :param output_file: (Optional) If provided, generated targets will be
        serialized here.

    :return: Generated system validation targets (not evaluated).
    """
    xls_file = Path(get_validation_dir() + "/SystemValidationData.xlsx")

    return generate_validation_targets(
        xls_file=xls_file,
        patient_file=log_file,
        output_file=output_file
    )


def gen_patient_targets(log_file: Path, output_file: Optional[Path]=None) -> Optional[SEPatientTimeSeriesValidation]:
    """
    Generate patient validation timeseries validation targets.

    :param log_file: Path to log file. Could alternatively be the relevant
        patient file.
    :param output_file: (Optional) If provided, generated targets will be
        serialized here.

    :return: Generated patient validation targets (not evaluated).
    """
    xls_file = Path(get_validation_dir() + "/PatientValidationData.xlsx")

    # TODO: Actually generate targets
    return SEPatientTimeSeriesValidation()


def timeseries_validation_pipeline(
    log_file: Path,
    csv_file: Path,
    table_dir: Optional[Path]=None
) -> Tuple[SEPatientTimeSeriesValidation, SEPatientTimeSeriesValidation]:
    """
    Processes given log and csv file through the timeseries validation
    pipeline.

    :param log_file: Path to log file.
    :param csv_file: Path to csv results file.
    :param table_dir: (Optional) Path to table output directory. If None,
        tables will not be generated.

    :return: Evaluated system targets
    :return: Evaluated patient targets.
    """
    sys_tgts = gen_system_targets(log_file=log_file, output_file=None)
    if sys_tgts is None:
        _pulse_logger.error("Unable to generate system targets")
        return False

    patient_tgts = gen_patient_targets(log_file=log_file, output_file=None)
    if patient_tgts is None:
        _pulse_logger.error("Unable to generate patient targets")
        return False

    validate(patient_val=sys_tgts, csv_filename=csv_file, output_file=None)
    # TODO: Repeat with patient targets

    # Generate Tables (Optional)
    if table_dir is not None:
        generate_validation_tables(target_map=sys_tgts, table_dir=table_dir)
        # TODO: Name collisions between patient and system target tables?
        #generate_validation_tables(target_map=sys_tgts, table_dir=table_dir)

    return sys_tgts, patient_tgts


def bulk_timeseries_validation_pipeline(
    filename_base_paths: List[Path],
    output_basename: Path,
    table_dir: Optional[Union[Path, List[Optional[Path]]]]=None
) -> None:
    """
    Processes bulk list of files through timeseries validation pipeline to
    one output file, optionally saving individual validation tables. If an
    exception is raised during execution, an attempt will be made to serialize
    the current list of targets.

    :param filename_base_paths: List of file base paths to process. ".log" and
        ".csv" will be appended to these paths to locate log and results files.
    :param output_basename: The output filepath basename. One file each for
        system targets and patient targets will be generated.
    :param table_dir: (Optional) Path or list of paths indicating where tables
        should be generated for the corresponding filename_base_path. If only
        one path is provided, all tables will be generated in that directory.
        If None is provided (single or as list item), tables will not be
        generated.
    """
    all_sys_tgts = list()
    all_patient_tgts = list()

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
            if not csv_file.exists():
                csv_file = filename_base.parent / f"{filename_base.stem}Results.csv"
                if not csv_file.exists():
                    _pulse_logger.error(f'CSV file does not exist for {filename_base_paths}')
                    continue
            sys_tgts, patient_tgts = timeseries_validation_pipeline(
                log_file=log_file,
                csv_file=csv_file,
                table_dir=t_dir
            )
            all_sys_tgts.append(sys_tgts)
            all_patient_tgts.append(patient_tgts)
    except:
        raise
    finally:
        # Save current target maps, even if something goes wrong
        output_basename.parent.mkdir(parents=True, exist_ok=True)
        sys_out_file = output_basename.parent / f"{output_basename.stem}SystemTargets.json"
        patient_out_file = output_basename.parent / f"{output_basename.stem}PatientTargets.json"

        _pulse_logger.info(f"Writing {sys_out_file}")
        serialize_patient_time_series_validation_list_to_file(all_sys_tgts, sys_out_file)

        _pulse_logger.info(f"Writing {patient_out_file}")
        serialize_patient_time_series_validation_list_to_file(all_patient_tgts, patient_out_file)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    if len(sys.argv) < 2:
        _pulse_logger.error(
            "Expected inputs : <filename base path where there is a csv and log file by the name>"
            " <output basename> [table dir]"
        )
        # For example path/to/scenario/results/Male_44yr_180.34cm_23.71bmi_0.21bff_72bpm_87mmHg_40.5mmHg_12bpm
        sys.exit(1)

    table_dir = None
    if len(sys.argv) > 2:
        table_dir = Path(sys.argv[2])

    bulk_timeseries_validation_pipeline(
        filename_base_paths=[Path(sys.argv[1])],
        output_basename=Path(sys.argv[1]),
        table_dir=table_dir
    )
