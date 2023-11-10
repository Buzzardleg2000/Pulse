# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SECardiovascularMechanicsModification
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

    cvMod = SECardiovascularMechanicsModification()
    cvMod.get_modifiers().get_heart_rate_multiplier().set_value(1.05)
    # We want the system to restabilize to our requested modification
    # This is TRUE by default, so you don't need to explicitly set it
    # cvMod.set_restabilization(True);
    # If you were wanting to incrementally modify the system, you can set this to false
    # When we do restabilization, we turn OFF the feedback systems (like baroreceptors)
    # Then reset all the baselines once we get to homeostatis
    # If you don't restabilize, the feedback systems are NOT turned off, so make sure increments are small

    pulse.process_action(cvMod)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_CardiovascularModification()

