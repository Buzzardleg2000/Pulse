# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.study.bind.SensitivityAnalysis_pb2 import *
from google.protobuf import json_format

import json
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import sys

from SALib.sample import saltelli
from scipy import stats


def create_histograms(sample_params_df, output_dir):
    """
    Create histograms of sampled parameters.
    :param sample_params_df: Pandas DataFrame - holds sampled parameters
    :param output_dir: String - output directory
    :return: None
    """

    print("Writing histograms ...")
    if not os.path.exists(os.path.join(output_dir, "histograms")):
        os.mkdir(os.path.join(output_dir, "histograms"))

    for key in sample_params_df.keys():
        sample_params_df.hist(column=key, bins=30)
        plt.savefig(os.path.join(output_dir, "histograms", "{}.png".format(key)))


def run_simulation_list(param_values_df, sobol_params_dict, output_dir):
    """
    Write the simulation list.
    :param sim_list: SimulationListData object
    :param param_values_df: Pandas DataFrame - holds sampled parameter values
    :param sobol_params_dict: Dictionary - holds the unit of each parameter
    :param output_dir: String - output directory
    :return: None
    """

    # this method will be updated once we settle on a better serialization method
    print("Population simulation list object ...")
    sim_list = SimulationListData()
    sim_list.OutputRootDir = output_dir + "simulations/"

    # Write out that as a json object
    count = 0
    for index, row in param_values_df.iterrows():
        sim = sim_list.Simulation.add()
        sim.ID = index
        sim.Name = "Simulation{}".format(index)
        for unit, path in enumerate(param_values_df.keys()):
            sim0 = sim.Overrides[path]
            sim0.Value = row[path]
            sim0.Unit = sobol_params_dict["unit"][unit]

        if count % 500 == 0 and count > 0:
            print("Writing simulation list object {} to {}...".format(count - 500, count))
            out = json_format.MessageToJson(sim_list)
            file = open(os.path.join(output_dir, "sim_list_{}.json".format(count)), "w")
            file.write(out)
            file.close()
            sim_list = SimulationListData()
            sim_list.OutputRootDir = output_dir + "simulations/"
        count += 1

    print("Writing simulation list object {} to {}...".format(
        param_values_df.shape[0] - param_values_df.shape[0] % 500, param_values_df.shape[0]))
    out = json_format.MessageToJson(sim_list)
    file = open(os.path.join(output_dir, "sim_list_{}.json".format(param_values_df.shape[0])), "w")
    file.write(out)
    file.close()


def gen_local_sa_sim_list(percent_variation, baseline_file, distribution, output_dir, compartment):
    """
    Generate the simulation list.
    :param percent_variation: Float - variation for each parameter
    :param baseline_file: String - patient state file to read baseline vales
    :param distribution: String - "normal or "uniform"
    :param output_dir: String - output directory
    :return: Pandas DataFrame - holds sampled parameters, dictionary - holds parameter units
    """

    sobol_params_dict = get_paths_and_values(percent_variation, baseline_file, distribution, compartment)
    problem_size = 100

    sobol_params_dict_to_write = {"problem_size": problem_size, "paths": sobol_params_dict["paths"]}
    with open(os.path.join(output_dir, 'local_sa_problem.json'), 'w') as fp:
        json.dump(sobol_params_dict_to_write, fp)

    param_values_df = pd.DataFrame(columns=['Path', 'Value', 'Unit'])
    for index, path in enumerate(sobol_params_dict["paths"]):
        path_values = [path] * problem_size
        unit_values = [sobol_params_dict["unit"][index]] * problem_size
        values = np.linspace(sobol_params_dict["bounds"][index][0], sobol_params_dict["bounds"][index][1], problem_size)
        param_values_df = pd.concat([param_values_df,
                                     pd.DataFrame(data={"Path": path_values, "Value": values, "Unit": unit_values},
                                                  columns=["Path", "Value", "Unit"])], ignore_index=True)
    print("Created {} simulations from {} parameters.".format(param_values_df.size, param_values_df.shape[1]))

    # this method will be updated once we settle on a better serialization method
    print("Population simulation list object ...")
    sim_list = SimulationListData()
    sim_list.OutputRootDir = output_dir + "simulations/"

    # Write out that as a json object
    # This will be updated once we settle on a serialization method
    for index, row in param_values_df.iterrows():
        sim = sim_list.Simulation.add()
        sim.ID = index
        sim.Name = "Simulation{}".format(index)
        sim0 = sim.Overrides[row["Path"]]
        sim0.Value = row["Value"]
        sim0.Unit = row["Unit"]
        if index % 500 == 0 and index > 0:
            print("Writing simulation list object {} to {}...".format(index - 500, index))
            out = json_format.MessageToJson(sim_list)
            file = open(os.path.join(output_dir, "sim_list_{}.json".format(index)), "w")
            file.write(out)
            file.close()
            sim_list = SimulationListData()
            sim_list.OutputRootDir = output_dir + "simulations/"

    print("Writing simulation list object {} to {}...".format(
        param_values_df.shape[0] - param_values_df.shape[0] % 500, param_values_df.shape[0]))
    out = json_format.MessageToJson(sim_list)
    file = open(os.path.join(output_dir, "sim_list_{}.json".format(param_values_df.shape[0])), "w")
    file.write(out)
    file.close()


def gen_sim_list_from_defaults(percent_variation, baseline_file, distribution, output_dir, compartment):
    """
    Generate the simulation list.
    :param percent_variation: Float - variation for each parameter
    :param baseline_file: String - patient state file to read baseline vales
    :param distribution: String - "normal or "uniform"
    :param output_dir: String - output directory
    :param compartment: String - 'cv' or 'combined'
    :return: Pandas DataFrame - holds sampled parameters, dictionary - holds parameter units
    """

    print("Sampling parameter space ...")
    sobol_problem_size = 1000
    sobol_params_dict = get_paths_and_values(percent_variation, baseline_file, distribution, compartment)
    sobol_problem = sample_params(sobol_params_dict, output_dir, sobol_problem_size)

    param_values = saltelli.sample(sobol_problem, sobol_problem_size)
    sampled_params_df = pd.DataFrame(data=param_values, columns=sobol_params_dict["paths"])
    print("Created {} simulations from {} parameters.".format(param_values.shape[0], param_values.shape[1]))
    return sampled_params_df.sample(frac=1), sobol_params_dict


def get_paths_and_values(percent_variation, baseline_file, distribution, compartment):
    """
    Parse the patient state value for baselines.
    :param percent_variation: Float - amount of variation for each parameter
    :param baseline_file: String - patient state file
    :param distribution: String - "normal" or "uniform"
    :return: Dictionary - holds values for Sobol sampling
    """

    print("Parsing patient state file ...")
    file = open(baseline_file, 'r')
    initial_patient_state_file = json.load(file)
    file.close()

    # get all cardiovascular and/or respiratory paths
    # enter any resistance or compliance paths that should not be included in analysis
    cv_paths_to_remove = []
    resp_paths_to_remove = []
    full_cv_paths = next(
        item for item in initial_patient_state_file["CircuitManager"]["FluidCircuit"]
        if item["Circuit"]["Name"] == "Cardiovascular")
    full_cv_paths = [ele for ele in full_cv_paths["Circuit"]["Path"] if ele not in cv_paths_to_remove]
    full_resp_paths = next(
        item for item in initial_patient_state_file["CircuitManager"]["FluidCircuit"]
        if item["Circuit"]["Name"] == "Respiratory")
    full_resp_paths = [ele for ele in full_resp_paths["Circuit"]["Path"] if ele not in resp_paths_to_remove]

    sobol_params = {"paths": [], "bounds": [], "dists": [], "unit": []}
    full_cv_paths.sort()

    for circuit_path in initial_patient_state_file["CircuitManager"]["FluidPath"]:
        if circuit_path["CircuitPath"]["Name"] in full_cv_paths:
            if 'Resistance' in circuit_path.keys():
                populate_sobol_params(circuit_path["Resistance"]["ScalarPressureTimePerVolume"]["Value"],
                                      circuit_path["CircuitPath"]["Name"], sobol_params, distribution,
                                      circuit_path["Resistance"]["ScalarPressureTimePerVolume"]["Unit"],
                                      percent_variation)
            elif 'Compliance' in circuit_path.keys():
                populate_sobol_params(circuit_path["Compliance"]["ScalarVolumePerPressure"]["Value"],
                                      circuit_path["CircuitPath"]["Name"], sobol_params, distribution,
                                      circuit_path["Compliance"]["ScalarVolumePerPressure"]["Unit"],
                                      percent_variation)
        elif compartment == "combined" and circuit_path["CircuitPath"]["Name"] in full_resp_paths:
            if 'Resistance' in circuit_path.keys():
                populate_sobol_params(circuit_path["Resistance"]["ScalarPressureTimePerVolume"]["Value"],
                                      circuit_path["CircuitPath"]["Name"], sobol_params, distribution,
                                      circuit_path["Resistance"]["ScalarPressureTimePerVolume"]["Unit"],
                                      percent_variation)
            elif 'Compliance' in circuit_path.keys():
                populate_sobol_params(circuit_path["Compliance"]["ScalarVolumePerPressure"]["Value"],
                                      circuit_path["CircuitPath"]["Name"], sobol_params, distribution,
                                      circuit_path["Compliance"]["ScalarVolumePerPressure"]["Unit"],
                                      percent_variation)

    return sobol_params


def populate_sobol_params(value, name, sobol_params, distribution, unit, percent_variation):
    """
    Populate a dictionary to make it easier to sample the parameter space.
    :param circuit_path: Dictionary - holds parts of the patient state file
    :param sobol_params: Dictionary - to populate with parameters to sample
    :param lumped_element: String - "Resistance" or "Compliance"
    :param distribution: String - "normal" or "uniform"
    :param unit: String - unit from patient state file
    :param percent_variation: Float - amount of variation within each parameter
    :return: Dictionary - holds parameters with bounds to sample
    """

    if distribution == "normal":
        bound1 = value
        bound2 = value * percent_variation / 100.0
        sobol_params["dists"].append("norm")
    elif distribution == "uniform":
        bound1 = value - percent_variation / 100.0 * value
        bound2 = value + percent_variation / 100.0 * value
        sobol_params["dists"].append("unif")
    else:
        raise ValueError("Incorrect distribution.")
    sobol_params["bounds"].append([bound1, bound2])
    sobol_params["paths"].append(name)
    sobol_params["unit"].append(unit)

    return sobol_params


def sample_params(sobol_params_dict, output_dir, sobol_problem_size=1000):
    """
    Create the Sobol problem (https://salib.readthedocs.io/en/latest/)
    :param sobol_params_dict: Dictionary - used to populate Sobol problem
    :param output_dir: String - output directory
    :return: Dictionary - Sobol problem
    """

    problem = {
        'problem_size': sobol_problem_size,
        'num_vars': len(sobol_params_dict["paths"]),
        'names': sobol_params_dict["paths"],
        'bounds': sobol_params_dict["bounds"],
        'dists': sobol_params_dict["dists"]
    }

    with open(os.path.join(output_dir, 'sobol_problem.json'), 'w') as fp:
        json.dump(problem, fp)

    del problem["problem_size"]
    return problem


def gen_bounds_file(percent_variation, baseline_file, distribution, output_dir):
    """
    Create a JSON file with each parameter and its bounds that you can manually edit.
    :param percent_variation: Float - amount of variation for each parameter
    :param baseline_file: String - patient state file
    :param distribution: String - "normal" or "uniform"
    :param output_dir: String - output directory
    :return: None
    """

    sobol_params_dict = get_paths_and_values(percent_variation, baseline_file, distribution)
    sobol_problem = sample_params(sobol_params_dict, output_dir)

    out_dict = {}
    for index in range(0, len(sobol_problem["names"])):
        out_dict[sobol_problem["names"][index]] = {"bounds": sobol_problem["bounds"][index],
                                                   "dist": sobol_problem["dists"][index],
                                                   "unit": sobol_params_dict["unit"][index]}

    with open(os.path.join(output_dir, 'sensitivity_analysis_bounds_list.json'), 'w') as fp:
        json.dump(out_dict, fp)


def gen_sim_list_from_manual_edits(output_dir):
    """
    Create a simulation list from a manually edited JSON file.
    :param output_dir: String - output directory
    :return: Pandas DataFrame and dictionary - populate simulation list
    """

    if not os.path.exists(os.path.join(output_dir, 'sensitivity_analysis_bounds_list.json')):
        raise ValueError("Sobol parameters file does not exist!")

    file = open(os.path.join(output_dir, 'sensitivity_analysis_bounds_list.json'), "r")
    file_json = json.load(file)
    file.close()

    sobol_params_dict = {"paths": [], "bounds": [], "dists": [], "unit": []}
    for key in file_json:
        sobol_params_dict["paths"].append(key)
        sobol_params_dict["bounds"].append(file_json[key]["bounds"])
        sobol_params_dict["dists"].append(file_json[key]["dist"])
        sobol_params_dict["unit"].append(file_json[key]["unit"])

    sobol_problem = sample_params(sobol_params_dict, output_dir)

    param_values = saltelli.sample(sobol_problem, 10)
    sampled_params_df = pd.DataFrame(data=param_values, columns=sobol_params_dict["paths"])

    return sampled_params_df.sample(frac=1), sobol_params_dict


if __name__ == '__main__':
    """
    Creates series of JSON files for Pulse simulations.
    To run cardiovascular system analysis: python3 utils.py cv
    To run combined cardiovascular and respiratory systems analysis: python3 utils.py combined
    """

    if len(sys.argv) != 2:
        raise ValueError("Incorrect number of arguments. Please enter 'cv' if analyzing only the cardiovascular system"
                         " or 'combined' if analyzing the combined cardiovascular and respiratory systems.")

    write_only = True

    """
    Amount to vary the input parameters (resistances and compliances). If sampling parameters with a uniform
    distribution, the percent_variation will describe the range of values. If sampling parameters with a normal
    distribution, the percent_variation will describe the standard deviation."
    """
    percent_variation = 10.0

    # set distribution to 'normal' or 'uniform'
    distribution = "normal"

    output_dir = "./test_results/sensitivity_analysis/"
    baseline_file = "./states/StandardMale@0s.json"

    # set sensitivity_analysis to 'global' or 'local'
    sensitivity_analysis = "global"

    if sensitivity_analysis == "global":
        print("Global sensitivity analysis")
        create_sim_list = False
        if write_only:
            # Generate the whole list
            sample_params_df, sobol_param_dict = gen_sim_list_from_defaults(percent_variation,
                                                                            baseline_file,
                                                                            distribution,
                                                                            output_dir,
                                                                            sys.argv[1])
            create_sim_list = True
        else:
            if not os.path.exists(os.path.join(output_dir, "sensitivity_analysis_bounds_list.json")):
                gen_bounds_file(percent_variation, baseline_file, distribution, output_dir)
            else:
                sample_params_df, sobol_param_dict = gen_sim_list_from_manual_edits(output_dir)
                create_sim_list = True

        if create_sim_list:
            create_histograms(sample_params_df, output_dir)
            run_simulation_list(sample_params_df, sobol_param_dict, output_dir)
    elif sensitivity_analysis == "local":
        print("Local sensitivity analysis")
        gen_local_sa_sim_list(percent_variation, baseline_file, distribution, output_dir, sys.argv[1])
