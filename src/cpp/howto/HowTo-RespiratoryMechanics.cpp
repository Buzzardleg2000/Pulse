/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/patient/actions/SERespiratoryMechanicsConfiguration.h"
#include "cdm/system/physiology/SERespiratoryMechanics.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SECurve.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Custom driver settings for our respiratory system
/// Drive respiration with your own driver
///
/// \details
/// Refer to the SERespiratoryMechanics class
//--------------------------------------------------------------------------------------------------
void HowToRespiratoryMechanics()
{
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = pulse::engine::CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/howto/HowTo_RespiratoryMechanics.log");
  pe->GetLogger()->Info("HowTo_RespiratoryMechanics");

  // With this engine, you do not initialize it, its already ready to go at construction time

  // You can load a previously saved state, but this is optional!
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))// Select patient
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Setup data requests to write to a csv file so we can plot data
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/howto/HowTo_RespiratoryMechanics.csv");

  for (size_t i = 0; i < 6; i++)
  {
    AdvanceAndTrackTime_s(10, *pe);
    pe->GetLogger()->Info(std::stringstream() << "\nSimTime : " << pe->GetSimulationTime(TimeUnit::s) << "s");
    pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
    pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
    pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << "mL");
    pe->GetLogger()->Info(std::stringstream() << "Expiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetExpiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Inspiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetInspiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << "mL/cmH2O");
    pe->GetLogger()->Info(std::stringstream() << "Total Pulmonary Ventilation : " << pe->GetRespiratorySystem()->GetTotalPulmonaryVentilation(VolumePerTimeUnit::mL_Per_s) << "mL/s\n");
  }

  SERespiratoryMechanicsConfiguration config;
  SERespiratoryMechanics& mechanics = config.GetSettings();
  mechanics.SetActive(eSwitch::On);
  // Create a mechanics profile
  double resistance_cmH2O_s_Per_L = 13.0;
  double compliance_mL_Per_cmH2O = 50.0;
  double musclePressure_cmH2O = 13.0;
  double respirationRate_bpm = 15.0;
  double inspiratoryTime_s = 0.9;

  // These need to add to 1.0 (100%)
  // const double UpperResistanceFraction = 0.5;
  // UpperResistanceFraction is implied
  const double BronchiResistanceFraction = 0.3;
  const double AlveoliDuctResistanceFraction = 0.2;

  double upperResistance = resistance_cmH2O_s_Per_L - (BronchiResistanceFraction * resistance_cmH2O_s_Per_L + AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L) / 2;
  double bronchiResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L;
  double alveoliDuctResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - bronchiResistance;
  double sideResistance = bronchiResistance + alveoliDuctResistance;

  double RespiratorySideCompliance_L_Per_cmH2O = compliance_mL_Per_cmH2O / 2.0;
  double LungCompliance_L_Per_cmH2O = 2.0 * RespiratorySideCompliance_L_Per_cmH2O;
  double ChestWallCompliance_L_Per_cmH2O = LungCompliance_L_Per_cmH2O;

  double totalBreathTime_s = 1.0 / (respirationRate_bpm / 60.0);
  double inspiratoryFraction = inspiratoryTime_s / totalBreathTime_s;
  double expiratoryFraction = 1.0 - inspiratoryFraction;

  double InspiratoryRiseFraction = inspiratoryFraction;
  double InspiratoryHoldFraction = 0.0;
  double InspiratoryReleaseFraction = MIN(inspiratoryFraction, expiratoryFraction * 0.5);
  double InspiratoryToExpiratoryPauseFraction = 1.0 - InspiratoryRiseFraction - InspiratoryReleaseFraction;
  double ExpiratoryRiseFraction = 0.0;
  double ExpiratoryHoldFraction = 0.0;
  double ExpiratoryReleaseFraction = 0.0;

  mechanics.GetUpperInspiratoryResistance().SetValue(upperResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetUpperExpiratoryResistance().SetValue(upperResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetLeftInspiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetLeftExpiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetRightInspiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetRightExpiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SECurve& lcc = mechanics.GetLeftComplianceCurve();
  SESegmentConstant& lc = lcc.AddConstantSegment();
  lc.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lc.GetEndVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lc.GetCompliance().SetValue(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit::mL_Per_cmH2O);
  SECurve& rcc = mechanics.GetRightComplianceCurve();
  SESegmentConstant& rc = rcc.AddConstantSegment();
  rc.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  rc.GetEndVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  rc.GetCompliance().SetValue(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit::mL_Per_cmH2O);

  mechanics.GetInspiratoryRiseTime().SetValue(InspiratoryRiseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryHoldTime().SetValue(InspiratoryHoldFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryReleaseTime().SetValue(InspiratoryReleaseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryToExpiratoryPauseTime().SetValue(InspiratoryToExpiratoryPauseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryRiseTime().SetValue(ExpiratoryRiseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryHoldTime().SetValue(ExpiratoryHoldFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryReleaseTime().SetValue(ExpiratoryReleaseFraction * totalBreathTime_s, TimeUnit::s);

  mechanics.GetInspiratoryPeakPressure().SetValue(-13.0, PressureUnit::cmH2O);
  mechanics.GetExpiratoryPeakPressure().SetValue(0.0, PressureUnit::cmH2O);

  pe->ProcessAction(config);

  for (size_t i = 0; i < 12; i++)
  {
    AdvanceAndTrackTime_s(10, *pe);
    pe->GetLogger()->Info(std::stringstream() << "\nSimTime : " << pe->GetSimulationTime(TimeUnit::s) << "s");
    pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
    pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
    pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << "mL");
    pe->GetLogger()->Info(std::stringstream() << "Expiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetExpiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Inspiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetInspiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << "mL/cmH2O");
    pe->GetLogger()->Info(std::stringstream() << "Total Pulmonary Ventilation : " << pe->GetRespiratorySystem()->GetTotalPulmonaryVentilation(VolumePerTimeUnit::mL_Per_s) << "mL/s\n");
  }
}