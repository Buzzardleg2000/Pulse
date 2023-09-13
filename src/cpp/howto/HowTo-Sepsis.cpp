/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/patient/actions/SESepsisExacerbation.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolumePerTime.h"


//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying sepsis to the patient
//--------------------------------------------------------------------------------------------------
void HowToSepsis()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo/HowTo_Sepsis.log");
  pe->GetLogger()->Info("HowTo_Sepsis");
  
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit::C);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalMetabolicRate", PowerUnit::W);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystemicVascularResistance", PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_Sepsis.csv");

  // Advance some time to get some resting data
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetEngineTracker()->LogRequestedValues();
  
  SESepsisExacerbation sepsis;
  sepsis.GetInfectionSeverity().SetValue(0.5);
  sepsis.GetProgressionSeverity().SetValue(0.5);
  pe->ProcessAction(sepsis);

  AdvanceAndTrackTime_s(60*60, *pe);
  pe->GetLogger()->Info("The patient is declining");
  pe->GetEngineTracker()->LogRequestedValues();

  pe->GetLogger()->Info("Finished");
}
