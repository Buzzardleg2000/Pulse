# I used this command in windows cmd: dir /a-D /S /B

set(PULSE_BIND_JAVA_FILES
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Actions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/AnesthesiaMachine.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/AnesthesiaMachineActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/BagValveMask.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/BagValveMaskActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/BlackBox.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Circuit.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Compartment.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Conditions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/ECMO.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/ECMOActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/ElectroCardioGram.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Engine.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Enums.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Environment.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/EnvironmentActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/EnvironmentConditions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/EquipmentActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Events.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Inhaler.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/InhalerActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/MechanicalVentilator.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/MechanicalVentilatorActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Patient.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/PatientActions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/PatientAssessments.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/PatientConditions.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/PatientNutrition.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Plots.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Physiology.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Properties.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Scenario.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/Substance.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/SubstanceQuantity.java
  ${DST_ROOT}/java/com/kitware/pulse/cdm/bind/TestReport.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Configuration.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Enums.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Environment.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Equipment.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Physiology.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/Scenario.java
  ${DST_ROOT}/java/com/kitware/pulse/engine/bind/State.java
  ${DST_ROOT}/java/com/kitware/pulse/study/bind/hydrocephalus/Hydrocephalus.java
  ${DST_ROOT}/java/com/kitware/pulse/study/bind/multiplex_ventilation/MultiplexVentilation.java
  ${DST_ROOT}/java/com/kitware/pulse/study/bind/sensitivity_analysis/SensitivityAnalysis.java
)
set(PULSE_JAVA_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/SerializationType.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/actions/SEAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/actions/SEAdvanceTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/actions/SEAdvanceUntilStable.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/actions/SESerializeRequested.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/actions/SESerializeState.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SECompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SECompartmentManager.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SEFluidCompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SEGasCompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SELiquidCompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SEThermalCompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/compartment/SETissueCompartment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/conditions/SECondition.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/doxygen/CDM2MD.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/doxygen/DoxygenPreprocessor.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/doxygen/Proto2Doxygen.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEActiveEvent.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEDataRequest.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEDataRequestManager.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEDynamicStabilization.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEDynamicStabilizationEngineConvergence.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEEngineConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEEventHandler.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEEventManager.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SEPatientConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/engine/SETimedStabilization.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/exceptions/InvalidUnitException.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/SEPatient.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEAcuteStress.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEAirwayObstruction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEArrhythmia.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEAsthmaAttack.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEBrainInjury.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEBronchoconstriction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEChestCompression.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEChestCompressionAutomated.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEChestCompressionInstantaneous.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEChestOcclusiveDressing.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEConsciousRespiration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEConsciousRespirationCommand.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEConsumeNutrients.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEDyspnea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEExercise.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEForcedExhale.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEForcedInhale.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEForcedPause.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEHemorrhage.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEHemothorax.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEImpairedAlveolarExchangeExacerbation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEIntubation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEMechanicalVentilation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SENeedleDecompression.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEPatientAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEPatientAssessmentRequest.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEPericardialEffusion.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEPneumoniaExacerbation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEPulmonaryShuntExacerbation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SERespiratoryFatigue.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SERespiratoryMechanicsConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SESubstanceBolus.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SESubstanceCompoundInfusion.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SESubstanceInfusion.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SESupplementalOxygen.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SETensionPneumothorax.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEUrinate.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/actions/SEUseInhaler.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SEArterialBloodGasTest.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SECompleteBloodCount.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SEComprehensiveMetabolicPanel.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SEPatientAssessment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SEUrinalysis.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/assessments/SEUrinalysisMicroscopic.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEAcuteRespiratoryDistressSyndrome.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEChronicAnemia.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEChronicPericardialEffusion.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEChronicRenalStenosis.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEConsumeMeal.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEImpairedAlveolarExchange.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEPatientCondition.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEPneumonia.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEPulmonaryFibrosis.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SEPulmonaryShunt.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/conditions/SESepsis.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/nutrition/SEMeal.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/patient/nutrition/SENutrition.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/CommonUnits.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEArray.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SECurve.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEFunction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEFunctionElectricPotentialVsTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEFunctionVolumeVsTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEHistogram.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEHistogramFractionVsLength.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEProperty.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalar.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalar0To1.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarAmount.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarAmountPerMass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarAmountPerTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarAmountPerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarElectricPotential.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarEnergy.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarEquivalentWeightPerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarForce.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarFrequency.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarHeatCapacitancePerMass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarHeatConductancePerArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarHeatResistanceArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarInversePressure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarLength.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarLengthPerTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMassPerAmount.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMassPerAreaTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMassPerMass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMassPerTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarMassPerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarNegative1To1.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarOsmolality.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarOsmolarity.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPower.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressurePerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressureTimePerArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressureTimePerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressureTimePerVolumeArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarPressureTimeSquaredPerVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarTemperature.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolume.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerPressure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerTime.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerTimeArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerTimeMass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerTimePressure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SEScalarVolumePerTimePressureArea.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SESegment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SESegmentConstant.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SESegmentLinear.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SESegmentParabolic.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/properties/SESegmentSigmoidal.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/scenario/SEScenario.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/scenario/SEScenarioExec.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstance.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceAerosolization.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceClearance.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceCompound.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceConcentration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceFraction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceManager.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstancePharmacodynamics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstancePharmacokinetics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstancePhysicochemicals.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/SESubstanceTissuePharmacokinetics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/quantity/SEGasSubstanceQuantity.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/quantity/SELiquidSubstanceQuantity.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/substance/quantity/SESubstanceQuantity.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/SESystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/SEActiveConditioning.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/SEAppliedTemperature.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/SEEnvironment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/SEEnvironmentalConditions.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/actions/SEChangeEnvironmentalConditions.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/actions/SEEnvironmentAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/actions/SEThermalApplication.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/conditions/SEEnvironmentCondition.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/environment/conditions/SEInitialEnvironmentalConditions.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/SEEquipment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/SEEquipmentAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachine.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineChamber.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveLeak.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveLeak.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineMaskLeak.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineSodaLimeFailure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVaporizerFailure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVentilatorPressureLoss.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineYPieceDisconnect.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/SEBagValveMask.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/ecmo/SEECMO.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/ecmo/actions/SEECMOAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/ecmo/actions/SEECMOConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/electrocardiogram/SEElectroCardioGram.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/inhaler/SEInhaler.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/inhaler/actions/SEInhalerAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/inhaler/actions/SEInhalerConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorContinuousPositiveAirwayPressure.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorHold.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorLeak.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorPressureControl.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorVolumeControl.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEBloodChemistrySystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SECardiovascularSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEDrugSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEEndocrineSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEEnergySystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEGastrointestinalSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEHepaticSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SENervousSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEPhysiologySystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SEPupillaryResponse.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SERenalSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SERespiratoryMechanics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SERespiratorySystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/system/physiology/SETissueSystem.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/BindingConverter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/CDMUnitTestDriver.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/Rebase.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/Reconfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SEReportResults.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestCase.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestCaseListener.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestDriver.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestJob.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestReport.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/SETestSuite.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/csv/CSVComparison.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/validation/MutliResultTool.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/validation/PatientValidation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/validation/SystemValidation.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/validation/ValidationMatrix.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/testing/validation/ValidationTool.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/validation/SESegmentValidationTarget.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/validation/SETimeSeriesValidationTarget.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/cdm/validation/SEValidationTarget.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/dataset/DataSetReader.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/PulseConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/PulseEngine.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/PulseScenario.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/PulseScenarioExec.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/testing/EngineUnitTestDriver.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/engine/testing/ScenarioTestDriver.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_AnesthesiaMachine.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_EngineUse.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_ECMO.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_Envionment.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_Hemorrhage.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_MechanicalVentilator.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_RespiratoryMechanics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_RunScenario.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/howto/HowTo_VentilationMechanics.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/Base64.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/CurrentClass.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/DoubleUtils.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/FileNameFilter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/FileUtils.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/FindObjects.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/JNIBridge.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/Log.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/LogListener.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/Pair.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/RunConfiguration.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/SEEqualOptions.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/SimpleEquals.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/StringUtils.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/TimingProfile.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/UnitConverter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/WaveformUtils.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/ConvexHullMaker.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/CSVAnalysis.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/CSVContents.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/ReduceCSV.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/RenameCSVHeaders.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/ActionEventPlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/ConvexHullPlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/CSVComparePlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/CSVPlotTool.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/LegendPlot.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/LogEvent.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/MultiPlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/MultiSourcePlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/PairPlotter.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/PlotDriver.java
  ${CMAKE_CURRENT_SOURCE_DIR}/com/kitware/pulse/utilities/csv/plots/Plotter.java
)