# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.pipelines.segment_validation import *
from pulse.pipelines.dataset.timeseries_dataset_reader import *

if __name__ == "__main__":

    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    # Generate Data Requests
    xls_file = Path(get_validation_dir()) / "SystemValidationData.xlsx"
    generate_data_requests(xls_file, Path("./validation/requests"))

    # In the future, we will do all xlsx in the data/validation dir
    # But for now, we are just hard coding the automated xlsx files
    xls_files = ["AirwayObstruction.xlsx"]

    for xls_file in xls_files:
        segment_validation_pipeline(
            xls_file=Path(xls_file),
            exec_opt=eExecOpt.GenerateOnly
        )
