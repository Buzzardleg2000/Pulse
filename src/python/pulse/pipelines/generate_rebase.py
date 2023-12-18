# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import logging
from pathlib import Path
from typing import Dict, List

from pulse.cdm.utils.file_utils import get_config_dir
from pulse.cdm.testing import SETestReport
from pulse.cdm.io.testing import serialize_test_report_from_file


_pulse_logger = logging.getLogger('pulse')


def gen_test_cache(configs: List[Path]) -> Dict[Path, Dict[str, str]]:
    """
    Generate "cache" of test definitions per given config file

    :param configs: List of config file paths

    :return: Dictionary of dictionaries. Each config file (that exists)
        will serve as a top-level key. Each config dictionary will represent
        test name -> test definition
    """
    out = dict()
    for config in configs:
        if not config.is_file():
            _pulse_logger.warning(f"{config} does not appear to exist. Skipping")
            continue

        out[config] = dict()

        with open(config) as f:
            lines = f.readlines()

        found_group = False
        multi_line = False
        test_key = None
        test_value = None
        for line in lines:
            if not line.strip():                      # Empty line
                continue
            if line.lstrip().startswith("#"):         # Comment
                continue
            if line.lstrip().startswith("@group"):    # First group indicates to start parsing
                found_group = True
                continue
            if not found_group:                       # Skip all lines until first group identified
                continue

            if multi_line:
                if not line.rstrip().endswith("\\"):  # End of multi-line definition
                    multi_line = False
                    test_value += line
                    out[config][test_key] = test_value
                    continue
                else:                                 # Continuation of multi-line definition
                    test_value += line
                    continue

            if "=" not in line:
                _pulse_logger.warning(f"Unable to parse line: {line}")
                continue

            def_idx = line.find("=")
            test_key = line[:def_idx].strip()
            test_value = line[def_idx+1:]

            if test_value.rstrip().endswith("\\"):    # Beginning of multi-line definition
                multi_line = True
            else:
                out[config][test_key] = test_value    # Single line test definition

    return out


def gen_rebase_config(test_cache: Dict[Path, Dict[str, str]]) -> None:
    """
    Locate failures in test reports and generate a "./test_results/errors.config"
    to be used for easy rebase configuration.

    :param test_cache: Cache of test names and definitions per config file
    """
    # Find failures for each config/report pair
    failures = list()
    results_str = "ResultsReport"
    for config, tests in test_cache.items():
        test_report_path = Path("./test_results/") / f"{config.stem}Report.json"
        if not test_report_path.is_file():
            _pulse_logger.warning(f"Could not find {test_report_path}")
            continue

        test_report = SETestReport()
        serialize_test_report_from_file(test_report_path, test_report)

        if test_report.get_errors() > 0:
            test_suites = test_report.get_test_suites()
            for ts in test_suites:
                if ts.get_num_errors() > 0:
                    if ts.get_name() in tests:  # Unit test or assessment test
                        failures.append(f"{ts.get_name()} = {tests[ts.get_name()].lstrip()}")
                    elif ts.get_name().endswith(results_str):  # Scenario test
                        json_sce = f"{ts.get_name()[:-len(results_str)]}.json"
                        for t_key, t_val in tests.items():
                            # We don't have the entire path from just the test report
                            if t_key.endswith(json_sce):
                                failures.append(f"{t_key} = {t_val.lstrip()}")

        # Write out failures to error config
        with open(Path("./test_results/errors.config"), "w") as f:
            f.writelines(failures)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    config_dir = Path(get_config_dir())
    configs = [
        "CDMUnitTests.config",
        "EngineUnitTests.config",
        "LongVerificationScenarios.config",
        "DrugPKVerification.config",
        "PatientVerification.config",
        "ScenarioVerification.config"
    ]
    configs = [config_dir / c for c in configs]

    # TODO: Save cache and look for file modifications?
    test_cache = gen_test_cache(configs)

    gen_rebase_config(test_cache)
