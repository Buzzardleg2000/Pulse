# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SERespiratoryMechanicsModification
from pulse.engine.PulseEngine import PulseEngine

def HowTo_CardiovascularModification():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_CardiovascularModification.py.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    pulse.print_results()

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

    rMod = SERespiratoryMechanicsModification()
    rMod.get_modifiers().get_respiration_rate_multiplier().set_value(1.05)
    pulse.process_action(rMod)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_CardiovascularModification()

