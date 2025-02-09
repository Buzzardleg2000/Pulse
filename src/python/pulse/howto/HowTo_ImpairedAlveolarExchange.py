# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEImpairedAlveolarExchangeExacerbation
from pulse.engine.PulseEngine import PulseEngine

def HowTo_ImpairedAvleolarExchange():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_ImpairedAlveolarExchange.py.log")
    pulse.log_to_console(True)

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    alveolar_exchange = pc.get_conditions().get_impaired_alveolar_exchange()
    # You can set one of two options
    #alveolar_exchange.get_impaired_surface_area().set_value(2, AreaUnit.cm2)
    alveolar_exchange.get_impaired_fraction().set_value(.3)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    pulse.print_results()

    # Perform an action to exacerbate the initial condition state
    exacerbation = SEImpairedAlveolarExchangeExacerbation()
    exacerbation.set_comment("Patient's Impaired Alveolar Exchange is exacerbated")
    # You can set one of two options
    #exacerbation.get_impaired_surface_area().set_value(2, AreaUnit.cm2)
    exacerbation.get_impaired_fraction().set_value(.3)
    pulse.process_action(exacerbation)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_ImpairedAvleolarExchange()

