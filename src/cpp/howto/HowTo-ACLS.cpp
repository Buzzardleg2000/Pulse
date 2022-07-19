/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

#include "cdm/scenario/SEScenario.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/patient/actions/SEIntubation.h"
#include "cdm/patient/actions/SEChestCompressionForceScale.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"


//--------------------------------------------------------------------------------------------------
/// \brief
/// Example of going into cardiac arrest and performing CPR and a Bag,Valve,Mask on the patient
//--------------------------------------------------------------------------------------------------
void HowToACLS()
{
  SEAdvanceTime adv;
  SEArrhythmia  arrhythmia;
  SEBagValveMaskConfiguration bvm;
  SEBagValveMaskSqueeze bvmSqueeze;
  SEChestCompressionForceScale cpr;
  std::stringstream ss;

  double unassisted_min = 7;
  bool intubatedAssistance = false;
  double assisted_min = 7;
  // We will do cpr_block_s of CPR compressions every cpr_interval_s
  double cpr_block_s = 30;
  double cpr_interval_s = 1/Convert(120, FrequencyUnit::Per_min, FrequencyUnit::Per_s);
  // Then we will do 2 bvm squeezes every bvm_interval_s
  size_t bvmSqueezes = 2;
  double bvm_interval_s = 1/Convert(12, FrequencyUnit::Per_min, FrequencyUnit::Per_s);
  // We will keep doing CPR blocks, BVM blocks for the duration of the assisted_min
  double recovery_min = 10;

  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/howto/HowTo_ACLS.cpp/HowTo_ACLS.log");
  pe->GetLogger()->Info("HowTo_ALCS");

  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  SEDataRequestManager& dMgr = pe->GetEngineTracker()->GetDataRequestManager();
  dMgr.SetResultsFilename("./test_results/howto/HowTo_ACLS.cpp/HowTo_ACLS.csv");

  dMgr.CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  dMgr.CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  dMgr.CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  dMgr.CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  dMgr.CreatePhysiologyDataRequest("CarbonDioxideProductionRate", VolumePerTimeUnit::mL_Per_min);
  dMgr.CreatePhysiologyDataRequest("OxygenConsumptionRate", VolumePerTimeUnit::mL_Per_min);
  dMgr.CreatePhysiologyDataRequest("OxygenSaturation");
  dMgr.CreatePhysiologyDataRequest("PulseOximetry");
  dMgr.CreateSubstanceDataRequest("Oxygen", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
  dMgr.CreateSubstanceDataRequest("CarbonDioxide", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
  dMgr.CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  dMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Oxygen", "PartialPressure", PressureUnit::mmHg);
  dMgr.CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  dMgr.CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);


  dMgr.CreatePatientDataRequest("HeartRateBaseline", FrequencyUnit::Per_min);
  dMgr.CreatePatientDataRequest("MeanArterialPressureBaseline", PressureUnit::mmHg);
  dMgr.CreatePhysiologyDataRequest("BaroreceptorHeartRateScale");
  dMgr.CreatePhysiologyDataRequest("BaroreceptorHeartElastanceScale");
  dMgr.CreatePhysiologyDataRequest("BaroreceptorResistanceScale");
  dMgr.CreatePhysiologyDataRequest("BaroreceptorComplianceScale");
  dMgr.CreatePhysiologyDataRequest("ChemoreceptorHeartRateScale");
  dMgr.CreatePhysiologyDataRequest("ChemoreceptorHeartElastanceScale");
  dMgr.CreatePhysiologyDataRequest("PulmonaryCapillariesCoverageFraction");
  dMgr.CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  dMgr.CreatePhysiologyDataRequest("SystemicVascularResistance", PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  dMgr.CreateSubstanceDataRequest("CarbonDioxide", "MassInBlood", MassUnit::g);
  dMgr.CreateSubstanceDataRequest("CarbonDioxide", "MassInTissue", MassUnit::g);
  dMgr.CreateSubstanceDataRequest("Oxygen", "MassInBlood", MassUnit::g);
  dMgr.CreateSubstanceDataRequest("Oxygen", "MassInTissue", MassUnit::g);

  std::vector<std::pair<std::string, std::string>> tissues =
  {
    std::pair<std::string, std::string>(pulse::VascularCompartment::Fat,pulse::ExtravascularCompartment::FatExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Bone,pulse::ExtravascularCompartment::BoneExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Brain,pulse::ExtravascularCompartment::BrainExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Gut,pulse::ExtravascularCompartment::GutExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::LeftKidney,pulse::ExtravascularCompartment::LeftKidneyExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::RightKidney,pulse::ExtravascularCompartment::RightKidneyExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::LeftLung,pulse::ExtravascularCompartment::LeftLungExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::RightLung,pulse::ExtravascularCompartment::RightLungExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Liver,pulse::ExtravascularCompartment::LiverExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Muscle,pulse::ExtravascularCompartment::MuscleExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Myocardium,pulse::ExtravascularCompartment::MyocardiumExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Skin,pulse::ExtravascularCompartment::SkinExtracellular),
    std::pair<std::string, std::string>(pulse::VascularCompartment::Spleen,pulse::ExtravascularCompartment::SpleenExtracellular)
  };
  for (auto& itr : tissues)
  {
    // Vascular Properties
    dMgr.CreateLiquidCompartmentDataRequest(itr.first, "Oxygen", "Mass", MassUnit::g);
    dMgr.CreateLiquidCompartmentDataRequest(itr.first, "CarbonDioxide", "Mass", MassUnit::g);
    dMgr.CreateLiquidCompartmentDataRequest(itr.first, "Oxygen", "PartialPressure", PressureUnit::mmHg);
    dMgr.CreateLiquidCompartmentDataRequest(itr.first, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
    // Tissue Properties
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "Oxygen", "Mass", MassUnit::g);
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "CarbonDioxide", "Mass", MassUnit::g);
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "Oxygen", "PartialPressure", PressureUnit::mmHg);
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "InFlow", VolumePerTimeUnit::mL_Per_s);
    dMgr.CreateLiquidCompartmentDataRequest(itr.second, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  }

  AdvanceAndTrackTime_s(30, *pe);

  arrhythmia.SetRhythm(eHeartRhythm::CoarseVentricularFibrillation);
  pe->ProcessAction(arrhythmia);

  AdvanceAndTrackTime_s(unassisted_min*60, *pe);

  if (intubatedAssistance)
  {
    SEIntubation intubate;
    intubate.SetType(eIntubation_Type::Tracheal);
    pe->ProcessAction(intubate);
  }

  bvm.GetConfiguration().SetConnection(eSwitch::On);
  bvm.GetConfiguration().GetValvePositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  pe->ProcessAction(bvm);

  bool cpr_block = true;
  double block_s = cpr_block_s;
  double squeezeIn_s = 0;
  double compressIn_s = 0;
  double timeStep_s = pe->GetTimeStep(TimeUnit::s);

  double assisted_s = assisted_min*60;
  while(assisted_s > 0)
  {
    AdvanceAndTrackTime(*pe);

    block_s -= timeStep_s;
    if (block_s <= 0)
    {
      cpr_block = !cpr_block;
      if (cpr_block)
      {
        compressIn_s = 0;
        block_s = cpr_block_s;
        pe->Info("Starting CPR Block");
      }
      else
      {
        squeezeIn_s = 0;
        block_s = bvm_interval_s * bvmSqueezes;
        pe->Info("Starting BVM Block");
      }
    }

    if (cpr_block)
    {
      compressIn_s -= timeStep_s;
      if (compressIn_s <= 0)
      {
        cpr.GetForceScale().SetValue(0.6228);
        pe->ProcessAction(cpr);
        compressIn_s = cpr_interval_s;
      }
      
    }
    else
    {
      squeezeIn_s -= timeStep_s;
      if (squeezeIn_s <= 0)
      {
        bvmSqueeze.GetExpiratoryPeriod().SetValue(1.0, TimeUnit::s);
        bvmSqueeze.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s);
        pe->ProcessAction(bvmSqueeze);
        squeezeIn_s = bvm_interval_s;
      }
    }

    
    assisted_s -= timeStep_s;
  }

  arrhythmia.SetRhythm(eHeartRhythm::NormalSinus);
  pe->ProcessAction(arrhythmia);

  AdvanceAndTrackTime_s(recovery_min*60, *pe);
}
