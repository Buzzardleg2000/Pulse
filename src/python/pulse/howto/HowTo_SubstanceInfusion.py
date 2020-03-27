# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.scalars import VolumePerTimeUnit, MassPerVolumeUnit
from pulse.cdm.patient_actions import SESubstanceInfusion
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_SubstanceInfusion():
    pulse = PulsePhysiologyEngine("pulse_SubstanceInfusion.log")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    infusion = SESubstanceInfusion()
    infusion.set_comment("Patient receives an infusion of <>")
    infusion.set_substance("Calcium")
    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(1.0, MassPerVolumeUnit.from_string("g/L"))
    pulse.process_action(infusion)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_SubstanceInfusion()

