# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import logging
import os
import shutil
import sys
import numpy as np
import pandas as pd
from typing import Optional

from pulse.cdm.utils.csv_utils import read_csv_into_df
from pulse.cdm.utils.file_utils import get_verification_dir
from pulse.cdm.utils.math_utils import percent_difference
from pulse.cdm.plots import SEComparePlotter, SEPlotConfig, SEPlotSource
from pulse.cdm.utils.plotter import compare_plotter
from pulse.cdm.testing import SETestReport
from pulse.cdm.io.testing import serialize_test_report_to_file

class CSVComparison(SETestReport):
    __slots__ = ["error_limit"]

    def __init__(self, error_limit: float=2.0):
        super().__init__()
        self.error_limit = error_limit

    def compare(self, expected_file_path: str, computed_file_path):
        if not os.path.isfile(expected_file_path):
            logging.error(f"Expected file does not exist {expected_csv}")
            logging.info(f"I am going to try to plot the computed.")
            # TODO: check for zip file
        if not os.path.isfile(computed_file_path):
            logging.error(f"Computed file does not exist {computed_file_path}")

        report = f"{computed_file_path[:-4]}/{os.path.basename(computed_file_path)[:-4]}Report.json"
        self.set_full_report_path(report)

        # Remove and recreate directory
        try:
            shutil.rmtree(self.report_dir)
        except OSError as e:
            logging.warning(f"Could not remove old report directory: {self.report_dir}")
        os.makedirs(self.report_dir)

        # Create the test case
        suite = self.create_test_suite()
        suite.set_name(self.name)
        suite.start_case(self.name)

        # Use log file and directory from computed source
        log_file = computed_file_path[:-len("Results.csv")] + ".log"
        if not os.path.isfile(log_file):
            log_file = None
            logging.warning(f"Could not locate log file: {computed_file_path}")

        # Prepare plot sources so we can open csvs just once
        expected = SEPlotSource(
            csv_data=expected_file_path,
            line_format="-k",
            line_width=4.0,
            label="Expected",
            log_file=log_file)
        computed = SEPlotSource(csv_data=computed_file_path, line_format="-r", label="Computed")
        expected_df = expected.get_data_frame()
        computed_df = computed.get_data_frame()

        # Check for header differences
        total_errors = 0
        if len(expected_df.columns) != len(computed_df.columns):
            logging.warning(f"Number of results is difference, expected ({expected_file_path}) " \
                  f"{len(expected_df.columns)} but computed ({computed_file_path}) is {len(computed_df.columns)}")
        for y_header in expected_df.columns:
            if y_header not in computed_df.columns:
                total_errors += 1
                logging.error(f'Computed results did not provide expected result "{y_header}"')

        # Get times/first column
        first_col_is_time = expected_df.columns[0].lower().startswith("time")
        times = expected_df.iloc[:,0] if first_col_is_time else pd.Series(expected_df.index)

        # For report, only compare when both dataframes have values so truncate dfs while preserving
        # original dfs for plotting
        expected_df_trunc = expected_df
        computed_df_trunc = computed_df
        min_rows = len(expected_df.index)
        if len(expected_df.index) != len(computed_df.index):
            total_errors += 1
            logging.error(f"Computed ({computed_file_path}) is not the same length as expected")

            min_rows = min(len(expected_df.index), len(computed_df.index)) - 1
            expected_df_trunc = expected_df_trunc.loc[:min_rows]
            computed_df_trunc = computed_df_trunc.loc[:min_rows]

        # Actually compare CSVs
        compare_df = expected_df_trunc.combine(computed_df_trunc, lambda x, y: series_percent_difference(x, y, epsilon=1e-10))
        error_summary = compare_df.apply(lambda x: get_error_info(x, expected_df_trunc, computed_df_trunc, self.error_limit))
        failures = [column for column in error_summary.columns if error_summary.loc['total'][column] > 0]
        error_summary = error_summary[failures]

        total_errors += int(error_summary.loc['total'].sum())

        # TODO: get report differences value
        report_differences = False
        if report_differences:
            for f in failures:
                for t, ex, c, err in zip(times, expected_df_trunc[f], computed_df_trunc[f], compare_df[f]):
                    if err > self.error_limit:
                        if not first_col_is_time:
                            logging.error(f"{f} does not match expected {ex} != computed {c} [{err}%]")
                        else:
                            logging.error(f"{f} @Time {t}: expected {ex} != computed {c} [{err}%]")

        # Log all error summary info
        logging.info(f"Compared {len(expected_df_trunc.index)} total times")
        def _time(idx: int):
            return times.iloc[int(idx)]
        if total_errors > 0:
            logging.error(f"{total_errors} total errors found")
            for f in failures:
                e = error_summary[f]
                errPercent = 100 * e.loc['total'] / (e['last row'] + 1 - e['first row'])
                logging.error(f"{f} has a total of {int(e.loc['total'])} errors between times [{_time(e.loc['first row'])}, {_time(e.loc['last row'])}]\n" \
                              f"-  {errPercent:.2f}% of timesteps have errors between these times\n" \
                              f"-  min error : @Time {_time(e.loc['min row'])}: expected {e.loc['min expected']} != computed {e.loc['min computed']} [{e.loc['min']}%]\n" \
                              f"-  max error : @Time {_time(e.loc['max row'])}: expected {e.loc['max expected']} != computed {e.loc['max computed']} [{e.loc['max']}%]" )
        if suite.get_active_case().has_failures():
            logging.error(f"{computed_file_path} Comparison failed!!")
        else:
            logging.info(f"{computed_file_path} Comparison SUCCESS!!")

        # Write out results
        suite.end_case()
        serialize_test_report_to_file(self, self.report_dir + self.file_name)

        # Create plots
        config = SEPlotConfig(set_defaults=True)
        config.set_values(
            gridlines=True,
            output_path_override=self.report_dir,
            plot_actions=True,
            plot_events=True,
        )
        plotter = SEComparePlotter(
            config=config,
            expected_source=expected,
            computed_source=computed,
            failures=set(failures),
        )
        compare_plotter(plotter)


def series_percent_difference(x: pd.Series, y: pd.Series, epsilon: float):
    return pd.Series([percent_difference(xi, yi, epsilon, verbose=False) for xi, yi in zip(x,y)])


def get_error_info(x: pd.Series, expected: pd.DataFrame, computed: pd.DataFrame, threshold: float=2.0):
    summary = {}

    # Filter by threshold
    filtered = x[x > threshold]

    if filtered.empty:  # No errors above threshold
        summary = {
            "first row": np.nan,
            "last row": np.nan,
            "max": np.nan,
            "max computed": np.nan,
            "max expected": np.nan,
            "max row": np.nan,
            "min": np.nan,
            "min computed": np.nan,
            "min expected": np.nan,
            "max row": np.nan,
            "total": 0,
        }
    else:
        expected_s = expected[x.name]
        computed_s = computed[x.name]

        summary["first row"] = filtered.index[0]
        summary["last row"] = filtered.index[-1]

        max_row = filtered.idxmax()
        summary["max row"] = max_row
        summary["max"] = x[max_row]
        summary["max expected"] = expected_s[max_row] if max_row < expected_s.size else np.nan
        summary["max computed"] = computed_s[max_row] if max_row < computed_s.size else np.nan

        min_row = filtered.idxmin()
        summary["min row"] = min_row
        summary["min"] = x[min_row]
        summary["min expected"] = expected_s[min_row] if min_row < expected_s.size else np.nan
        summary["min computed"] = computed_s[min_row] if min_row < computed_s.size else np.nan

        # Num errors > threshold
        summary["total"] = filtered.count()

    return pd.Series(summary.values(), summary.keys())


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    expected_csv = None
    computed_csv = None
    output_directory = None
    error_limit = 2.0

    if len(sys.argv) < 2:
        logging.error("Expected inputs : [expected results file path] [computed results file path] [output directory] [percent tolerance]")
        sys.exit(1)
    else:
        if sys.argv[1].endswith(".csv"):
            expected_csv = sys.argv[1]
            computed_csv = sys.argv[2]
            next_arg = 3
        else:
            # Treat this as a scenario name and attempt to locate it
            expected_csv = f"{get_verification_dir()}/scenarios/{sys.argv[1]}Results.csv"
            computed_csv = f"./test_results/scenarios/{sys.argv[1]}Results.csv"
            next_arg = 2

    if len(sys.argv) > next_arg:
        output_directory = sys.argv[next_arg]
        next_arg += 1

    if len(sys.argv) > next_arg:
        error_limit = float(sys.argv[next_arg])
        next_arg += 1

    logging.info(f"Comparing {expected_csv} to {computed_csv}")
    c = CSVComparison(error_limit=error_limit)
    c.compare(expected_csv, computed_csv)
