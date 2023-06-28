/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#define _USE_MATH_DEFINES

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorHold.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorLeak.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorContinuousPositiveAirwayPressure.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorPressureControl.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorVolumeControl.h"
#include "cdm/patient/actions/SEDyspnea.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolumeArea.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolumePerTimeArea.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalar0To1.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the Mechanical Ventilator
/// Drive respiration with equipment
///
/// \details
/// Refer to the SEMechanicalVentilator class
//--------------------------------------------------------------------------------------------------
void HowToMechanicalVentilator()
{
  //Note: Setting circuit values (resistance/compliances/etc.) needs to be done in the engine code - they currently are not directly exposed
  
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo/HowTo_MechanicalVentilator.cpp.log");
  
  pe->GetLogger()->Info("HowTo_MechanicalVentilator");
  
 
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Vitals Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  // Ventilator Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("AirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxideFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalOxygenFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalOxygenPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("ExpiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("ExpiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryExpiratoryRatio");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("IntrinsicPositiveEndExpiredPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("LeakFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("MeanAirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PlateauPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TotalLungVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_MechanicalVentilator.cpp.csv");

  //Dyspnea
  SEDyspnea Dyspnea;
  Dyspnea.GetSeverity().SetValue(1.0);
  pe->ProcessAction(Dyspnea);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  // We have action support for several commonly used ventilator modes
  // Pulse is not limited to these modes, These modes are designe for simple understanding
  // Our implementation supports any ventilator mode, you will just need to translate the user facing inputs
  // to a timing/control profile using our configuration settings
  // These modes are internally converted into a configuration setting,
  // You can retrieve the resulting settings action and use it as a basis for more configurations if you want.
  // For example, if you wanted to lengthen the InspirationPatientTriggerFlow of the mode

  SEMechanicalVentilatorContinuousPositiveAirwayPressure cpap;
  cpap.SetConnection(eSwitch::On);
  cpap.SetInspirationWaveform(eDriverWaveform::AscendingRamp);
  cpap.GetFractionInspiredOxygen().SetValue(0.21);
  cpap.GetDeltaPressureSupport().SetValue(10.0, PressureUnit::cmH2O);
  cpap.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  cpap.GetSlope().SetValue(0.2, TimeUnit::s);

  cpap.GetInspirationPatientTriggerFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
  //Other trigger options (choose one):
  //  cpap.GetInspirationPatientTriggerPressure().SetValue(-1.0, PressureUnit::cmH2O);
  //  cpap.SetInspirationPatientTriggerRespiratoryModel(eSwitch::On);
  
  cpap.GetExpirationCycleFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
  //Other cycle options (choose one):
  //  cpap.GetExpirationCyclePressure().SetValue(0.000001, PressureUnit::cmH2O);
  //  cpap.SetExpirationCycleRespiratoryModel(eSwitch::On);
  
  pe->ProcessAction(cpap);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);


  SEMechanicalVentilatorPressureControl pc_ac;
  pc_ac.SetConnection(eSwitch::On);
  pc_ac.SetMode(eMechanicalVentilator_PressureControlMode::AssistedControl);
  pc_ac.SetInspirationWaveform(eDriverWaveform::Square);
  pc_ac.GetFractionInspiredOxygen().SetValue(0.21);
  pc_ac.GetInspiratoryPeriod().SetValue(1.1, TimeUnit::s); //This is optional
  pc_ac.GetInspiratoryPressure().SetValue(23.0, PressureUnit::cmH2O);
  pc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  pc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit::Per_min);
  //pc_ac.GetSlope().SetValue(0.0, TimeUnit::s); //No slope for square waveform

  pc_ac.GetInspirationPatientTriggerFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
  //Other trigger options (choose one):
  //  pc_ac.GetInspirationPatientTriggerPressure().SetValue(-1.0, PressureUnit::cmH2O);
  //  pc_ac.SetInspirationPatientTriggerRespiratoryModel(eSwitch::On);

  pe->ProcessAction(pc_ac);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);


  SEMechanicalVentilatorVolumeControl vc_ac;
  vc_ac.SetConnection(eSwitch::On);
  vc_ac.SetMode(eMechanicalVentilator_VolumeControlMode::AssistedControl);
  vc_ac.SetInspirationWaveform(eDriverWaveform::DescendingRamp);
  vc_ac.GetFlow().SetValue(60.0, VolumePerTimeUnit::L_Per_min);
  vc_ac.GetFractionInspiredOxygen().SetValue(0.21);
  vc_ac.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s); //This is optional
  vc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  vc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit::Per_min);
  vc_ac.GetTidalVolume().SetValue(900.0, VolumeUnit::mL);
  vc_ac.GetSlope().SetValue(0.2, TimeUnit::s); //This is optional and would be left out for square waveforms

  // Trigger options (choose one):
  //  vc_ac.GetInspirationPatientTriggerPressure().SetValue(-1.0, PressureUnit::cmH2O);
  //  vc_ac.GetInspirationPatientTriggerFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
  // If neither are provided, we will use the respiratory model trigger

  pe->ProcessAction(vc_ac);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);


  // Here is an example of programming a custom ventilator mode
  SEMechanicalVentilatorConfiguration mv_config;
  SEMechanicalVentilatorSettings& mv = mv_config.GetSettings();
  mv.SetConnection(eSwitch::On);
  mv.SetInspirationWaveform(eDriverWaveform::Square);
  mv.SetExpirationWaveform(eDriverWaveform::Square);
  mv.GetPeakInspiratoryPressure().SetValue(21.0, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(10.0, PressureUnit::cmH2O);
  SESubstanceFraction& fractionFiO2 = mv.GetFractionInspiredGas(*pe->GetSubstanceManager().GetSubstance("Oxygen"));
  fractionFiO2.GetFractionAmount().SetValue(0.5);
  double respirationRate_per_min = 20.0;
  double IERatio = 0.5;
  // Translate ventilator settings
  double totalPeriod_s = 60.0 / respirationRate_per_min;
  double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
  double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
  mv.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
  mv.GetExpirationCycleTime().SetValue(inspiratoryPeriod_s, TimeUnit::s);
  pe->ProcessAction(mv_config);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);


  // You can also perform holds
  SEMechanicalVentilatorHold hold;

  //Do an instantaneous hold
  hold.SetState(eSwitch::On);
  hold.SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle::Instantaneous);
  pe->ProcessAction(hold);
  AdvanceAndTrackTime_s(3, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  hold.SetState(eSwitch::Off);
  pe->ProcessAction(hold);

  AdvanceAndTrackTime_s(10, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  //Activate inspiratory hold for 3s during plateau of the next inspiratory phase
  hold.SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle::Inspiratory);
  hold.SetState(eSwitch::On);
  pe->ProcessAction(hold);
  //Keep advancing until the hold is applied
  while (pe->GetMechanicalVentilator()->GetBreathState() != eBreathState::InspiratoryHold)
  {
    AdvanceAndTrackTime(*pe);
  }
  //Hold for 3s
  AdvanceAndTrackTime_s(3, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  hold.SetState(eSwitch::Off);
  pe->ProcessAction(hold);
  //Output the resulting plateau pressure at end-inspiration
  double airwayPressure_cmH2O = pe->GetMechanicalVentilator()->GetAirwayPressure(PressureUnit::cmH2O);
  ss << "Inspiratory hold plateau pressure is " << airwayPressure_cmH2O << " cmH2O";
  pe->GetLogger()->Info(ss);

  AdvanceAndTrackTime_s(10, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  //Activate expiratory hold for 3s at the end of expiration
  hold.SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle::Expiratory);
  hold.SetState(eSwitch::On);
  pe->ProcessAction(hold);
  //Keep advancing until the hold is applied
  while (pe->GetMechanicalVentilator()->GetBreathState() != eBreathState::ExpiratoryHold)
  {
    AdvanceAndTrackTime(*pe);
  }
  //Hold for 3s
  AdvanceAndTrackTime_s(3, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  hold.SetState(eSwitch::Off);
  pe->ProcessAction(hold);
  //Output the resulting auto PEEP value
  airwayPressure_cmH2O = pe->GetMechanicalVentilator()->GetAirwayPressure(PressureUnit::cmH2O);
  ss << "Expiratory hold auto PEEP pressure is " << airwayPressure_cmH2O << " cmH2O";
  pe->GetLogger()->Info(ss);

  AdvanceAndTrackTime_s(10, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);


  // A leak can be specified
  SEMechanicalVentilatorLeak leak;
  leak.GetSeverity().SetValue(0.5);
  pe->ProcessAction(leak);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  leak.GetSeverity().SetValue(0.0);// Turn off the leak
  pe->ProcessAction(leak);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  pe->GetLogger()->Info("Finished");
}
