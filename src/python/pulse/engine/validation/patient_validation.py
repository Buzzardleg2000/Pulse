# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
from pathlib import Path

_pulse_logger = logging.getLogger('pulse')

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
    logging.getLogger("pycel").setLevel(logging.WARNING)

    # Option to validate/generate tables for either the baseline/verification csv or the test_results

    mode = "verification"
    if len(sys.argv) > 1:
        mode = sys.argv[1]
    search = Path(mode+"/scenarios/validation")

    files = search.glob('**/*.csv')
    for file in files:
        logging.info("Processing "+str(file))