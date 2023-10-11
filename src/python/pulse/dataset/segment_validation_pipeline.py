# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import glob
import shutil
import logging
import argparse
import matplotlib.pyplot as plt
from enum import Enum
from pathlib import Path

from pulse.cdm.engine import SESegmentValidationConfig, eSwitch
from pulse.cdm.io.engine import serialize_segment_validation_config_from_file, \
                                serialize_data_requested_result_from_file
from pulse.cdm.utils.markdown import process_file
from pulse.cdm.utils.file_utils import get_root_dir, get_validation_dir
from pulse.cdm.utils.plotter import create_plots, plot_with_test_results
from pulse.dataset.segment_dataset_reader import gen_scenarios_and_targets
from pulse.dataset.segment_validation import validate
from pulse.engine.PulseScenarioExec import PulseScenarioExec

_pulse_logger = logging.getLogger('pulse')

class eExecOpt(Enum):
    GenerateOnly = 0
    SkipScenarioExecution = 1
    MarkdownOnly = 2
    Full = 3

# Pipeline Flow
#   1. Read the xlsx and generate scenario and validation target files
#    - ./validation/scenarios/xlsx filename/scenario tab name.json
#   2. Run the generated scenario(s)
#    - ./test_results/scenarios/xlsx filename/scenario name.(log||csv)
#   3. Generate validation tables
#   4. Generate any plots
#   5. Preprocess markdown file (inserts tables)
# Exec Options:
#   GenerateOnly = Step 1
#   SkipScenarioExecution = Steps 1,3,4,5
#   MarkdownOnly = Steps 3,4,5
#   Full = Steps 1,2,3,4,5


def segment_validation_pipeline(xls_file: Path, exec_opt: eExecOpt, use_test_results: bool = False) -> None:

    if not xls_file.is_file():
        xls_file = Path(get_validation_dir()) / "Scenarios" / xls_file
        if not xls_file.is_file():
            _pulse_logger.error("Could not find " + str(xls_file))
            _pulse_logger.error("Please provide a valid xls file")
            sys.exit(1)

    xls_dir = xls_file.parent
    xls_basename = "".join(xls_file.name.rsplit("".join(xls_file.suffixes), 1))

    # This is where we will generate scenarios
    scenario_dir = Path("./validation/scenarios/"+xls_basename)
    # This is where we read the csv to validate
    validate_dir = Path("./verification/scenarios/"+xls_basename)
    # This is where the scenario should generate results
    test_results_dir = Path("./test_results/scenarios/" + xls_basename)

    scenario_dir.mkdir(parents=True, exist_ok=True)
    validate_dir.mkdir(parents=True, exist_ok=True)

    if exec_opt is eExecOpt.Full and not use_test_results:
        use_test_results = True
        _pulse_logger.info("Running full pipeline, switching to validate test results")
    if use_test_results:  # Let's validate what we generate
        validate_dir = test_results_dir
    elif not validate_dir.is_dir():
        _pulse_logger.error(f"Results directory ({validate_dir}) does not exist. Aborting")
        return

    sheets = gen_scenarios_and_targets(xls_file, scenario_dir, test_results_dir, exec_opt == eExecOpt.MarkdownOnly)
    if exec_opt is eExecOpt.GenerateOnly:
        return

    # plots and md files are expected to be:
    # 1. In the same directory as the xlsx file
    # 2. In the run.config root/docs/Validation directory
    # It is assumed the json and md file names are the same, just different extension

    md_files = []
    sheets.insert(0, xls_basename)
    for sheet in sheets:
        md_file = Path(xls_dir / (xls_basename + "-" + sheet + ".md"))
        if not md_file.is_file():
            md_file = Path(get_root_dir()) / "docs" / "Validation" / (sheet + ".md")
            if not md_file.is_file():
                md_file = None
        if md_file is not None:
            md_files.append(md_file)


    if len(md_files) == 0:
        _pulse_logger.error(f"Could not find md files, at least one should be in:")
        _pulse_logger.error(f"The same dir as the xlsx, or in your source/docs/Validation directory")
        sys.exit(1)
    plots_file = Path(xls_dir / (xls_basename + ".json"))
    if not plots_file.is_file():
        plots_file = Path(get_root_dir()) / "docs" / "Validation" / (xls_basename + ".json")
    if not plots_file.is_file():
        plots_file = None
    # Is there a custom bib file
    bib_file = Path(xls_dir / "Sources.bib")
    if bib_file.is_file():
        # Copy this file up as "Custom.bib"
        shutil.copyfile(xls_dir/"Sources.bib", "./docs/Custom.bib")
        # Are there any images in this directory
        images = glob.glob(str(xls_dir/'*'))
        for image in images:
            ext = [".jpg", ".png"]
            if image.endswith(tuple(ext)):
                image_dir = Path("./docs/html/Images/"+xls_basename)
                image_dir.mkdir(parents=True, exist_ok=True)
                shutil.copy(image, str(image_dir))

    # Run scenarios if we are running full pipeline
    if exec_opt is eExecOpt.Full:
        # Get list of all scenarios
        scenarios = [item.name for item in scenario_dir.glob("*")
                     if not item.is_dir() and "-ValidationTargets.json" not in item.name]
        for scenario in scenarios:
            scenario_file = scenario_dir / scenario
            sce_exec = PulseScenarioExec()
            sce_exec.set_log_to_console(eSwitch.On)
            sce_exec.set_scenario_filename(scenario_file.as_posix())
            if not sce_exec.execute_scenario():
                _pulse_logger.warning("Scenario {scenario} was not successfully run.")

    plots = None
    if plots_file is not None:
        plots = SESegmentValidationConfig()
        serialize_segment_validation_config_from_file(plots_file, plots)

    # Carry out validation on each scenario
    targets = [item.name for item in scenario_dir.glob("*")
               if not item.is_dir() and "-ValidationTargets.json" in item.name]
    for target_file in targets:
        abs_targets_filename = Path(scenario_dir / target_file)
        abs_segments_filename = Path(validate_dir / target_file.replace("-ValidationTargets", "Results-Segments"))
        if not abs_segments_filename.exists():
            _pulse_logger.error(f"Unable to locate segments for {target_file}. Continuing without validating.")
            continue

        table_dir = Path("./validation/tables/" + xls_basename + '/' + target_file.split('-')[0])
        table_dir.mkdir(parents=True, exist_ok=True)
        validate(abs_targets_filename, abs_segments_filename, table_dir=table_dir)

    if plots is not None:
        if use_test_results:
            plot_with_test_results(plots.get_plotters())
        create_plots(plots.get_plotters())

    # Run doxygen preprocessor
    for md_file in md_files:
        process_file(
            fpath=md_file,
            ref_dir=Path("./validation/tables"),
            dest_dir=Path("./validation/markdown"),
            replace_refs=False
        )


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')
    logging.getLogger('PIL').setLevel(logging.INFO)
    plt.set_loglevel("info")

    parser = argparse.ArgumentParser(description="Process the full pipeline for segment validation")
    parser.add_argument(
        "xls_file",
        nargs='?',
        type=Path,
        default=None,
        help="xls file to process during this pipeline run"
    )
    parser.add_argument(
        "-t", "--use-test-results",
        action='store_true',
        help="verify the latest test results."
    )
    run_group = parser.add_mutually_exclusive_group()
    run_group.add_argument(
        "-g", "--generate-only",
        action='store_true',
        help="only generate scenarios and validation target files"
    )
    run_group.add_argument(
        "-s", "--skip_exec",
        action='store_true',
        help="do not run the scenarios, results are expected to be found"
    )
    run_group.add_argument(
        "-m", "--markdown",
        action='store_true',
        help="only validate and build markdown"
    )
    run_group.add_argument(
        "-f", "--full",
        action='store_true',
        help="Run the full pipeline"
    )

    opts = parser.parse_args()

    xls_files = []
    if opts.xls_file is not None:
        xls_files.append(opts.xls_file)
    else:
        # In the future, we will do all xlsx in the data/validation dir
        # But for now, we are just hard coding the automated xlsx files
        xls_files.append("AirwayObstruction.xlsx")

    exec_opt = eExecOpt.Full
    if opts.generate_only:
        exec_opt = eExecOpt.GenerateOnly
    elif opts.skip_exec:
        exec_opt = eExecOpt.SkipScenarioExecution
    elif opts.markdown:
        exec_opt = eExecOpt.MarkdownOnly

    for xls_file in xls_files:
        segment_validation_pipeline(
            xls_file=Path(xls_file),
            exec_opt=exec_opt,
            use_test_results=opts.use_test_results
    )
