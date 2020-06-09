# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SESubstanceBolus, eSubstance_Administration
from pulse.cdm.scalars import MassPerVolumeUnit, VolumeUnit
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_SubstanceBolus():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_SubstanceBolus.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    substance = SESubstanceBolus()
    substance.set_comment("Patient receives injection of Epinephrine")
    substance.set_admin_route(eSubstance_Administration.Intramuscular)
    substance.set_substance("Epinephrine")
    substance.get_dose().set_value(1.0, VolumeUnit.mL)
    substance.get_concentration().set_value(1.1, MassPerVolumeUnit.from_string("ug/L"))
    pulse.process_action(substance)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_SubstanceBolus()

