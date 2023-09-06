# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import shutil
import logging
import numbers
import tempfile
import numpy as np
from pathlib import Path
from dataclasses import dataclass
from openpyxl import load_workbook
from openpyxl.worksheet.worksheet import Worksheet
from openpyxl.utils.cell import get_column_letter
from typing import Optional, Union
from pycel import ExcelCompiler

from pulse.cdm.engine import SETimeSeriesValidationTarget
from pulse.cdm.patient import SEPatient, eSex
from pulse.cdm.scalars import LengthUnit, MassUnit
from pulse.cdm.utils.file_utils import get_data_dir, get_validation_dir
from pulse.cdm.io.engine import (
    serialize_data_request_list_to_file,
    serialize_time_series_validation_target_list_to_file
)
from pulse.cdm.io.patient import serialize_patient_from_file
from pulse.dataset.utils import generate_data_request

_pulse_logger = logging.getLogger('pulse')

def load_data(xls_file: Path):
    # Remove and recreate directory
    output_dir = Path("./validation/")
    xls_basename = "".join(xls_file.name.rsplit("".join(xls_file.suffixes), 1))
    xls_basename_out = xls_basename[:-4] if xls_basename.lower().endswith("data") else xls_basename
    output_dir = output_dir / xls_basename_out
    try:
        if output_dir.is_dir():
            shutil.rmtree(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
    except OSError as e:
        _pulse_logger.error(f"Unable to clean directories")
        return

    if not xls_file.is_file():
        _pulse_logger.error(f"Could not find xls file {xls_file}")
        return

    _pulse_logger.info(f"Generating data from {xls_file}")

    # Temp file to preserve original state through patient updates
    xls_edit = tempfile.NamedTemporaryFile(suffix=".xlsx")
    xls_edit_path = Path(xls_edit.name)

    # xlsx sheets to skip when generating targets and requests
    ignore_sheets = ["Patient"]

    # Iterate through patients, generating targets for each
    patient_dir = Path("./patients/")
    try:
        if patient_dir.is_dir():
            for patient_file in patient_dir.glob("*.json"):
                full_output_path = output_dir / patient_file.stem
                full_output_path.mkdir(parents=True, exist_ok=True)

                # Update patient sheet so formulas can be re-evaluated with correct parameters
                update_patient(patient_file, xls_file, xls_edit_path)

                workbook = load_workbook(filename=xls_edit_path, data_only=False)
                evaluator = ExcelCompiler(filename=xls_edit_path)
                for system in workbook.sheetnames:
                    if system in ignore_sheets:
                        continue
                    if not generate_targets(
                        sheet=workbook[system],
                        evaluator=evaluator,
                        output_dir=full_output_path
                    ):
                        _pulse_logger.error(f"Unable to generate targets for {system} sheet")
        else:
            _pulse_logger.error("Unable to location patient directory")
    except Exception as e:
        raise
    finally:  # Always clean up temp file
        xls_edit.close()




def update_patient(patient_file: Path, xls_file: Path, new_file: Optional[Path]=None) -> None:
    p = SEPatient()
    serialize_patient_from_file(patient_file, p)

    workbook = load_workbook(filename=xls_file)
    sheet = workbook["Patient"]
    sheet["C2"] = "Male" if p.get_sex() == eSex.Male else "Female"
    if p.has_height():
        sheet["C3"] = p.get_height().get_value(units=LengthUnit.cm)
    if p.has_weight():
        sheet["C4"] = p.get_weight().get_value(units=MassUnit.kg)
    if p.has_right_lung_ratio():
        sheet["C9"] = p.get_right_lung_ratio().get_value()
    if p.has_body_fat_fraction():
        sheet["C12"] = p.get_body_fat_fraction().get_value()

    if not new_file:
        new_file = xls_file
    workbook.save(filename=new_file)


def generate_data_requests(xls_file: Path, output_dir: Path):
    try:
        if output_dir.is_dir():
            shutil.rmtree(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
    except OSError as e:
        _pulse_logger.error(f"Unable to clean directories")
        return
    # Generate a data request file for each target file
    ignore_sheets = ["Patient"]
    workbook = load_workbook(filename=xls_file, data_only=True)
    for system in workbook.sheetnames:
        if system in ignore_sheets:
            continue
        if not generate_sheet_requests(
                sheet=workbook[system],
                output_dir=output_dir
        ):
            _pulse_logger.error(f"Unable to generate requests for {system} sheet")


def generate_sheet_requests(sheet: Worksheet, output_dir: Path) -> bool:
    system = sheet.title.replace(" ", "")
    dr_dict = dict()

    # Get header to dataclass mapping
    ws_headers = [cell.value for cell in sheet[1]]
    try:
        DRB_HEADER = ws_headers.index('Output')
        DRB_UNITS = ws_headers.index('Units')
        DRB_REF_CELL = ws_headers.index('Reference Values')
        DRB_TABLE = ws_headers.index('Table')
        DRB_REQUEST_TYPE = ws_headers.index('Request Type')
        DRB_REQUEST_PRECISION = ws_headers.index('Request Precision')
    except ValueError as e:
        _pulse_logger.error(f"Missing required header {str(e)[:str(e).find(' is not in list')]}")
        return False

    @dataclass
    class DataRequestBuilder:
        header: str
        units: str
        ref_cell: Union[str, numbers.Number]
        dr_file: str
        request_type: str
        precision: Union[str, numbers.Number]

    for row_num, r in enumerate(sheet.iter_rows(min_row=2, values_only=True)):
        drb = DataRequestBuilder(
            header=r[DRB_HEADER],
            units=r[DRB_UNITS] if r[DRB_UNITS] else "unitless",
            ref_cell=r[DRB_REF_CELL],
            dr_file=r[DRB_TABLE] if r[DRB_TABLE] else "Orphaned",
            request_type=r[DRB_REQUEST_TYPE],
            precision=r[DRB_REQUEST_PRECISION]
        )
        if not drb.header:
            continue

        # Skip header rows
        if drb.dr_file == "Table":
            continue

        # Nothing to validate to
        if '*' in drb.header:
            # _pulse_logger.info(f"Ignoring request {drb.header} (has asterisks)")
            continue

        if drb.ref_cell is None:
            _pulse_logger.info(f"Not generating request {drb.header} (no reference value)")
            continue

        if not drb.request_type:
            _pulse_logger.info(f"Not generating request {drb.header} (no DR type)")
            continue

        if "@" in drb.request_type:
            # Currently ignoring assessment types
            continue

        if drb.dr_file not in dr_dict:
            dr_dict[drb.dr_file] = []
        drs = dr_dict[drb.dr_file]

        dr = generate_data_request(
            request_type=drb.request_type,
            property_name=drb.header.replace("*", ""),
            unit_str=drb.units.strip(),
            precision=int(drb.precision) if drb.precision else None
        )

        drs.append(dr)

    for dr_file, drs in dr_dict.items():
        filename = output_dir / f"{dr_file}.json"
        _pulse_logger.info(f"Writing {filename}")
        serialize_data_request_list_to_file(drs, filename)

    return True


def generate_targets(sheet: Worksheet, evaluator: ExcelCompiler, output_dir: Path) -> bool:
    system = sheet.title
    targets = dict()

    # Get header to dataclass mapping
    ws_headers = [cell.value for cell in sheet[1]]
    try:
        VTB_HEADER = ws_headers.index('Output')
        VTB_UNITS = ws_headers.index('Units')
        VTB_ALGO = ws_headers.index('Algorithm')
        VTB_REF_CELL = ws_headers.index('Reference Values')
        VTB_TGT_FILE = ws_headers.index('ValidationTargetFile')
        VTB_REQUEST_TYPE = ws_headers.index('Request Type')
    except ValueError as e:
        _pulse_logger.error(f"Missing required header {str(e)[:str(e).find(' is not in list')]}")
        return False

    @dataclass
    class ValidationTargetBuilder():
        header: str
        units: str
        algorithm: str
        ref_cell: Union[str, numbers.Number]
        tgt_file: str
        request_type: str

    for row_num, r in enumerate(sheet.iter_rows(min_row=2, values_only=True)):
        vtb = ValidationTargetBuilder(
            header=r[VTB_HEADER],
            units=r[VTB_UNITS] if r[VTB_UNITS] else "unitless",
            algorithm=r[VTB_ALGO],
            ref_cell=r[VTB_REF_CELL],
            tgt_file=r[VTB_TGT_FILE] if r[VTB_TGT_FILE] else "",
            request_type=r[VTB_REQUEST_TYPE]
        )
        if not vtb.header:
            continue

        # Skip header
        if vtb.tgt_file == "ValidationTargetFile":
            continue

        # Nothing to validate to
        if vtb.ref_cell is None:
            _pulse_logger.info(f"Not validating {vtb.header} (no reference value)")
            continue

        # Only generate targets with provided DR type
        if not vtb.request_type:
            _pulse_logger.info(f"Not validating {vtb.header} (no DR type)")
            continue

        if vtb.tgt_file not in targets:
            targets[vtb.tgt_file] = list()
        vts = targets[vtb.tgt_file]

        prop_split = [s.strip() for s in vtb.header.split("-")]
        dr = generate_data_request(
            request_type=vtb.request_type,
            property_name=vtb.header.replace("*", ""),
            unit_str=vtb.units.strip(),
            precision=None
        )
        # TODO: References and notes
        tgt = SETimeSeriesValidationTarget()
        tgt.set_header(dr.to_string())

        ref_val = vtb.ref_cell

        algo = vtb.algorithm
        if algo == "Max":
            algo = "Maximum"
        elif algo == "Min":
            algo = "Minimum"
        elif algo.endswith("(kg)"):
            algo = algo.replace("(kg)", "_kg")
        try:
            algo = SETimeSeriesValidationTarget.eTargetType[algo]
        except:
            _pulse_logger.error(f"Unknown validation target type: {algo}. Skipping validation target for {vtb.header} in {system}.")
            continue

        # Evaluate if needed
        if isinstance(ref_val, str) and ref_val.startswith("="):
            if ref_val.startswith("="):
                cell_loc = f"{system}!{get_column_letter(VTB_REF_CELL+1)}{row_num+2}"
                ref_val = evaluator.evaluate(cell_loc)

        # TODO: Support other comparison types
        if isinstance(ref_val, str):
            s_vals = ref_val.strip()
            s_vals = s_vals.replace('[', ' ')
            s_vals = s_vals.replace(']', ' ')
            vals = [float(s) for s in s_vals.split(',')]
            tgt.set_range(min(vals), max(vals), algo)
        elif isinstance(ref_val, numbers.Number):
            val = float(ref_val)
            tgt.set_equal_to(val, algo)
        else:
            _pulse_logger.warning(f"Unknown reference value type {ref_val}")
            tgt.set_range_min(np.nan)
            tgt.set_range_max(np.nan)

        vts.append(tgt)

    for target, val_tgts in targets.items():
        filename = output_dir / f"{system.replace(' ', '')}{'-' if target else ''}{target}.json"
        _pulse_logger.info(f"Writing {filename}")
        serialize_time_series_validation_target_list_to_file(val_tgts, filename)

    return True


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    if len(sys.argv) < 2:
        _pulse_logger.error("Expected inputs : <filename base path where there is a csv and log file by the name>")
        # For example path/to/scenario/results/Male_44yr_180.34cm_23.71bmi_0.21bff_72bpm_87mmHg_40.5mmHg_12bpm
        sys.exit(1)

    xls_file = Path(get_validation_dir() + "/SystemValidationData.xlsx")
    csv_file = Path(sys.argv[1] + ".csv")
    log_file = Path(sys.argv[1] + ".log")

    if not xls_file.is_file():
        _pulse_logger.error("Please provide a valid xls file")
        sys.exit(1)
    if not csv_file.is_file():
        _pulse_logger.error("Could not find csv file: "+str(csv_file))
        sys.exit(1)
    if not log_file.is_file():
        _pulse_logger.error("Could not find log file: "+str(log_file))
        sys.exit(1)

    gen_targets(xls_file, log_file)
