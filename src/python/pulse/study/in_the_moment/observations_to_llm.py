# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import json
import logging
import argparse
from typing import Dict
from pathlib import Path


_pulse_logger = logging.getLogger('pulse')


def observations_to_llm(filename: Path, START_counts: Dict[str, int]):
    """
    Post process the observation json for LLM input
    """
    # Replace "observations" in filepath with "llm"
    parts = list(filename.parts)
    parts[parts.index("observations")] = "llm"
    out_file = Path(*parts)
    out_file.parent.mkdir(parents=True, exist_ok=True)

    # Read observation file into dictionary
    with open(filename) as f:
        observations = json.load(f)

    # Iterate through each observation to pull out desired info
    out_obs = list()
    for obs in observations["Observations"]:
        out_obs.append(dict())
        out_obs[-1]["scenario"] = f"{obs['UnstructuredText']} blah blah blah"
        out_obs[-1]["state"] = None
        out_obs[-1]["probe"] = ""
        choices = list()
        out_obs[-1]["choices"] = choices  # Note this list will continue to change if you modify it

        # Accumulate START tag counts
        if obs["START"] not in START_counts:
            START_counts[obs["START"]] = 0
        START_counts[obs["START"]] += 1

    # Write out new structure
    with open(out_file, "w") as f:
        _pulse_logger.info(f"Writing {out_file}")
        f.write(json.dumps(out_obs, indent=2))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    parser = argparse.ArgumentParser(description="Postprocess an observation json(s)")
    parser.add_argument(
        "filenames",
        type=Path,
        nargs="+",
        help="File(s) to process"
    )

    args = parser.parse_args()

    # Post process each observation file
    START_counts = dict()
    for file in args.filenames:
        observations_to_llm(file, START_counts)

    # Print START tag counts
    _pulse_logger.info(f"Total START Tag Counts ({len(args.filenames)} patients):")
    for tag, count in START_counts.items():
        _pulse_logger.info(f"\t{tag}: {count}")
