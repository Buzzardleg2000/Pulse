# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEAcuteStress
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_AcuteStress():
    pulse = PulsePhysiologyEngine("pulse_AcuteStress.log")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    acute_stress = SEAcuteStress()
    acute_stress.set_comment("Patient undergoes acute stress")
    acute_stress.get_severity().set_value(1.0)
    pulse.process_action(acute_stress)

    # Advance some time and print out the vitals
    pulse.advance_time_s(60)
    results = pulse.pull_data()
    print(results)

HowTo_AcuteStress()

