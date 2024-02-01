# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
import argparse
from pathlib import Path
from typing import List, Optional, Union

from pulse.cdm.validation import SEPatientTimeSeriesValidation
from pulse.pipelines.dataset.timeseries_dataset_reader import gen_patient_targets
from pulse.pipelines.validation.timeseries_validation import (
    validate, generate_validation_tables,
    gen_expected_str, gen_engine_val_str)
from pulse.cdm.io.scenario import serialize_scenario_exec_status_list_from_file
from pulse.cdm.io.validation import (
    serialize_patient_time_series_validation_to_file,
    serialize_patient_time_series_validation_list_to_file)


_pulse_logger = logging.getLogger('pulse')


def timeseries_validation_pipeline(
        log_file: Path,
        csv_file: Path,
        table_dir: Optional[Path] = None,
        json_file: Optional[Path] = None
) -> SEPatientTimeSeriesValidation:
    """
    Processes given log and csv file through the timeseries validation
    pipeline.

    :param log_file: Path to log file.
    :param csv_file: Path to csv results file.
    :param table_dir: (Optional) Path to table output directory.
    :param json_file: (Optional) Generated targets will be serialized here.

    :return: Evaluated targets
    """

    patient_validation = gen_patient_targets(log_file=log_file)
    if patient_validation is None:
        _pulse_logger.error("Unable to generate patient targets")
        return False

    validate(patient_validation=patient_validation, csv_filename=csv_file, output_file=json_file)

    # Generate Tables (Optional)
    if table_dir is not None:
        generate_validation_tables(target_map=patient_validation, table_dir=table_dir)

    # Write the evaluated validation out (Optional)
    if json_file is not None:
        _pulse_logger.info(f"Writing {json_file}")
        json_file.parent.mkdir(parents=True, exist_ok=True)
        serialize_patient_time_series_validation_to_file(patient_validation, json_file)

    return patient_validation


def bulk_timeseries_validation_pipeline(
        json_file: Path,
        filename_base_paths: Optional[List[Path]] = None,
        exec_status_file: Optional[Path] = None,
        table_dir: Optional[Union[Path, List[Optional[Path]]]] = None,
        serialize_per_file: bool = False
) -> list:
    """
    Processes bulk list of files through timeseries validation pipeline to
    one output file, optionally saving individual validation tables. If an
    exception is raised during execution, an attempt will be made to serialize
    the current list of targets.

    :param filename_base_paths: List of file base paths to process. ".log" and
        ".csv" will be appended to these paths to locate log and results files.
    :param json_file: Generated targets will be serialized to this file.
    :param table_dir: (Optional) Path or list of paths indicating where tables
        should be generated for the corresponding filename_base_path. If only
        one path is provided, all tables will be generated in that directory.
        If None is provided (single or as list item), tables will not be
        generated.
    :param serialize_per_file: If true, each individual file's validation targets
        will be serialized to its own json file, in addition to the bulk file.
    """
    all_tgts = list()

    if not filename_base_paths and exec_status_file is None:
        _pulse_logger.warning("No files to process for timeseries validation.")
        return all_tgts

    if exec_status_file is not None:
        if filename_base_paths is None:
            filename_base_paths = list()
        statuses = list()
        serialize_scenario_exec_status_list_from_file(exec_status_file, statuses)
        for status in statuses:
            if (
                status.get_initialization_state() != eEngineInitializationState.Initialized or
                status.get_scenario_execution_state() != eScenarioExecutionState.Complete
            ):
                _pulse_logger.warning(f"{status.get_scenario_filename()} results are incomplete. Skipping validation.")

            filename_base_paths.append(Path(status.get_csv_filename()).with_suffix(""))

        for category, value in SEScenarioExecStatus.summarize_exec_status_list(statuses).items():
            _pulse_logger.info(f"{category} Runs: {value}")

    # If only one table dir provided, use the same table dir for every input file
    if not isinstance(table_dir, List):
        table_dirs = [table_dir] * len(filename_base_paths)
    else:
        table_dirs = table_dir

    try:
        # Run the pipeline on each input file
        for filename_base, t_dir in zip(filename_base_paths, table_dirs):
            # Check if the csv file should have *Results
            log_file = filename_base.parent / f"{filename_base.name}.log"
            csv_file = filename_base.parent / f"{filename_base.name}.csv"
            out_file = filename_base.parent / f"{filename_base.name}.json" if serialize_per_file else None
            if not csv_file.exists():
                csv_file = filename_base.parent / f"{filename_base.name}Results.csv"
                if not csv_file.exists():
                    _pulse_logger.error(f'CSV file does not exist for {filename_base}')
                    continue
            tgts = timeseries_validation_pipeline(
                log_file=log_file,
                csv_file=csv_file,
                table_dir=t_dir,
                json_file=out_file
            )
            all_tgts.append(tgts)
    except:
        raise
    finally:
        # Save current target map, even if something goes wrong
        if json_file is not None:
            json_file.parent.mkdir(parents=True, exist_ok=True)
            _pulse_logger.info(f"Writing {json_file}")
            serialize_patient_time_series_validation_list_to_file(all_tgts, json_file)
    return all_tgts


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    # Option to validate/generate tables for either the baseline/verification csv or the test_results



    parser = argparse.ArgumentParser(description="Process the full pipeline for timeseries validation")
    parser.add_argument(
        "-d", "--documentation",
        type=str,
        default=None,
        help="Perform patient validation for documentation on results in the `verification` or `test_result` directory."
    )
    # The rest of the arguments are for more granular control of what/how to validate
    parser.add_argument(
        "-b", "--filename_base_paths",
        type=Path,
        default=None,
        help="base path(s) to files to process during this pipeline run." \
             "E.g. path/to/file1 indicates path/to/file1.csv and path/to/file1.log should be processed."
    )
    parser.add_argument(
        "-e", "--exec_status_file",
        type=Path,
        default=None,
        help="Path to a scenario exec status file to process during this pipeline run."
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

    if opts.documentation is not None:
        mode = opts.documentation
        search = Path(mode + "/scenarios/validation/")

        files = search.glob('*.log')
        filename_base_paths = []
        for file in files:
            filename_base_paths.append(Path(str(file.parent)+'/'+file.stem))

        all_validation = bulk_timeseries_validation_pipeline(
            filename_base_paths=filename_base_paths,
            exec_status_file=None,
            json_file=None,
            table_dir=Path("./test_results/tables"),
            serialize_per_file=False
        )
        # Only write a html file for test results
        if opts.documentation == "test_results":
            # Push Standard patients to the front
            all_validation.insert(0, all_validation.pop(
                [idx for idx, tgt in enumerate(all_validation) if tgt.get_patient().get_name() == "StandardFemale"][0]))
            all_validation.insert(0, all_validation.pop(
                [idx for idx, tgt in enumerate(all_validation) if tgt.get_patient().get_name() == "StandardMale"][0]))
            html_file = "./test_results/PatientValidation.html"
            _pulse_logger.info(f"Writing {html_file}")
            f = open(html_file, "w")
            f.write("<html>\n")
            f.writelines("<body>\n")
            f.write("<h1>Patient Validation</h1>\n")
            for validation in all_validation:
                f.write("<br>\n")
                for type, tgts in validation.get_targets().items():
                    f.writelines("<table border=\"1\">\n")
                    f.write("<tr>")
                    f.write("<th> " + validation.get_patient().get_name() + " " + type + " Validation </th>")
                    f.write("<th> Expected Value </th>")
                    f.write("<th> Engine Value </th>")
                    f.write("<th> Percent Error </th>")
                    f.write("<th> Notes </th>")
                    f.write("</tr>\n")
                    for tgt in tgts:
                        if tgt.get_error_value() <= 10.0:
                            f.write("<tr bgcolor=\"#00FF00\">")
                        elif tgt.get_error_value() <= 30.0:
                            f.write("<tr bgcolor=\"#FFFF00\">")
                        elif tgt.get_error_value() > 30.0:
                            f.write("<tr bgcolor=\"#FF0000\">")
                        else:
                            f.write("<tr bgcolor=\"#FFFFFF\">")
                        f.write("<td>" + tgt.get_header() + "</td>")
                        f.write("<td>" + gen_expected_str(tgt) + "</td>")
                        f.write("<td>" + gen_engine_val_str(tgt) + "</td>")
                        f.write(f"<td>{tgt.get_error_value():{tgt.get_table_formatting()}}</td>")
                        f.write("<td>" + tgt.get_notes() + "</td></tr>\n")
                    f.write("</table><br>\n");
            f.write("</body>\n");
            f.write("</html>\n");
            f.close()
    else:
        json_file = opts.output_file
        if json_file is None:
            if opts.filename_base_paths and len(opts.filename_base_paths) == 1:
                base_path = opts.filename_base_paths[0]
                json_file = base_path.parent / f"{base_path.stem}ValidationTargets.json"
            elif opts.exec_status_file is not None:
                json_file = opts.exec_status_file.parent / f"{opts.exec_status_file.stem}ValidationTargets.json"
            else:
                _pulse_logger.error("You must provide an output file when multiple basenames are provided")
                sys.exit(1)

        bulk_timeseries_validation_pipeline(
            filename_base_paths=opts.filename_base_paths,
            exec_status_file=opts.exec_status_file,
            json_file=json_file,
            table_dir=opts.table_dir,
            serialize_per_file=opts.serialize_per_file
        )
