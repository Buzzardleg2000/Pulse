# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import logging

from pulse.cdm.scenario import SEScenarioExecStatus
from pulse.cdm.io.scenario import serialize_scenario_exec_status_list_from_file

_pulse_logger = logging.getLogger('pulse')


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    #  Add various options to clear statuses for a full rerun
    patient_scenarios_list_filename = "./test_results/scenarios"
    # Look for a patient scenario exec status list file
    # if it's not there
    #   Call C++ to generate our patient scenarios
    #   This will make a patient scenario exec status list file
    # Execute all those scenarios

    injury_scenarios_list_filename = "./test_results/patient_variability/test/scenarios/tccc.json"
    # Look for an injury scenario exec status list file
    # if it's not there
    # Call C++ to generate our tccc injury scenarios
    #   This will make an injury scenario exec status list file
    # Execute all those scenarios

    # Read in the executed injury scenario exec status
    injury_scenarios = []
    serialize_scenario_exec_status_list_from_file(injury_scenarios_list_filename, injury_scenarios)

    # Iterate over these injury scenario to
    for injury_scenario in injury_scenarios:
        print("Generating observations for "+str(injury_scenario.get_scenario_filename()))
        # Call our post processor to generate observation files for each scenario
        # Observations should go here: (just replace the 'scenarios' in the csv results location)
        # ./test_results/patient_variability/test/observations/tccc/Male_44yr_180.34cm_23.71bmi_0.21bff_72bpm_87mmHg_40.5mmHg_12bpm/AO0_H0.5_TP0_D5s.json

