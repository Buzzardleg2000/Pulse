# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.engine.PulseEngine import PulseEngine

def HowTo_Sepsis():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_Sepsis.py.log")
    pulse.log_to_console(True)

    # TODO Model is currently incomplete!

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    sepsis = pc.get_conditions().get_sepsis()
    sepsis.get_infection_severity().set_value(.3)
    sepsis.get_progression_severity().set_value(.7)

    # Get some data from the engine
    results = pulse.pull_data()
    pulse.print_results()

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_Sepsis()

