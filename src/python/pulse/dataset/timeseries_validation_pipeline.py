# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
from pathlib import Path
from typing import Dict, List, Optional, Union

from pulse.cdm.engine import SETimeSeriesValidationTarget
from pulse.cdm.utils.file_utils import get_validation_dir
from pulse.dataset.timeseries_dataset_reader import generate_validation_targets
from pulse.dataset.timeseries_validation import validate


_pulse_logger = logging.getLogger('pulse')


def gen_system_targets(log_file: Path, output_file: Path) -> bool:
    xls_file = Path(get_validation_dir() + "/SystemValidationData.xlsx")

    return generate_validation_targets(
        xls_file=xls_file,
        patient_file=log_file,
        output_file=output_file
    )


def gen_patient_targets(log_file: Path, output_file: Optional[Path]=None) -> bool:
    xls_file = Path(get_validation_dir() + "/PatientValidationData.xlsx")

    # TODO: Actually generate targets
    return True


def timeseries_validation_pipeline(log_file: Path, csv_file: Path, output_dir: Optional[Path]=None) -> bool:
    if output_dir is None:
        output_dir = log_file.parent
    sys_tgts_output_file = output_dir / f"{log_file.stem}_SystemTargets.json"
    patient_tgts_output_file = output_dir / f"{log_file.stem}_PatientTargets.json"

    if not gen_system_targets(log_file=log_file, output_file=sys_tgts_output_file):
        _pulse_logger.error("Unable to generate system targets")
        return False
    if not gen_patient_targets(log_file=log_file, output_file=patient_tgts_output_file):
        _pulse_logger.error("Unable to generate patient targets")
        return False

    table_dir = output_dir / f"{log_file.stem}_ValidationTables"
    validate(targets_filename=sys_tgts_output_file, csv_filename=csv_file, table_dir=table_dir)

    # sys_tgts is a SETimeSeriesValidationTargetMap
    #run_validation(csv_file, sys_tgts) # -> SEPropertyValidationMap
    # Generate Tables (Optional)
    # run_validation(csv_file, patient_tgts)

    return True


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    if len(sys.argv) < 2:
        _pulse_logger.error("Expected inputs : <filename base path where there is a csv and log file by the name>")
        # For example path/to/scenario/results/Male_44yr_180.34cm_23.71bmi_0.21bff_72bpm_87mmHg_40.5mmHg_12bpm
        sys.exit(1)

    csv_file = Path(sys.argv[1] + ".csv")
    log_file = Path(sys.argv[1] + ".log")

    if not csv_file.is_file():
        _pulse_logger.error(f"Could not find csv file: {csv_file}")
        sys.exit(1)
    if not log_file.is_file():
        _pulse_logger.error(f"Could not find log file: {log_file}")
        sys.exit(1)

    timeseries_validation_pipeline(log_file=log_file, csv_file=csv_file)
