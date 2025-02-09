
set(BIND_H_FILES
# cdm bindings
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Actions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BagValveMask.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BagValveMaskActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BlackBox.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Circuit.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Compartment.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Conditions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ECMO.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ECMOActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Engine.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Enums.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Environment.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EquipmentActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Events.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Inhaler.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/InhalerActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/MechanicalVentilator.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/MechanicalVentilatorActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Patient.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientActions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientConditions.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Physiology.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Plots.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Properties.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Scenario.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Substance.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.h"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/TestReport.pb.h"
# engine bindings
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Configuration.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Enums.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Environment.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Equipment.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Physiology.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Scenario.pb.h"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/State.pb.h"
# study bindings
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/Hydrocephalus.pb.h"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/MultiplexVentilation.pb.h"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/PatientVariability.pb.h"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/SensitivityAnalysis.pb.h"
)

set(BIND_CPP_FILES
# cdm bindings
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Actions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BagValveMask.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BagValveMaskActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/BlackBox.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Circuit.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Compartment.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Conditions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ECMO.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ECMOActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Engine.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Enums.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Environment.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/EquipmentActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Events.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Inhaler.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/InhalerActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/MechanicalVentilator.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/MechanicalVentilatorActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Patient.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientActions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientConditions.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Physiology.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Plots.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Properties.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Scenario.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/Substance.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.cc"
  "${DST_ROOT}/cpp/${CDM_PACKAGE}/TestReport.pb.cc"
# engine bindings
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Configuration.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Enums.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Environment.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Equipment.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Physiology.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/Scenario.pb.cc"
  "${DST_ROOT}/cpp/${ENGINE_PACKAGE}/State.pb.cc"
# study bindings
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/Hydrocephalus.pb.cc"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/MultiplexVentilation.pb.cc"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/PatientVariability.pb.cc"
  "${DST_ROOT}/cpp/${STUDY_PACKAGE}/SensitivityAnalysis.pb.cc"
)

set(BIND_H_FILES ${BINDING_H_FILES} PARENT_SCOPE)
set(BIND_CPP_FILES ${BINDING_CPP_FILES} PARENT_SCOPE)

