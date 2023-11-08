/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "TCCCIteration.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse::study::patient_variability
{
  TCCCIteration::TCCCIteration(Logger& logger) : ActionIteration(logger)
  {
    m_IterationName = "TCCC";
    SetStateDirectory("");
    SetScenarioExecListFilename("");

    m_PerformInterventions = false;

    m_Hemorrhage.SetType(eHemorrhage_Type::External);
    m_Hemorrhage.SetCompartment(eHemorrhage_Compartment::RightLeg);
    m_TensionPneumothorax.SetType(eGate::Open);
    m_TensionPneumothorax.SetSide(eSide::Left);

    m_DataRequestMgr->CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    m_DataRequestMgr->CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
    m_DataRequestMgr->CreatePhysiologyDataRequest("OxygenSaturation");
    m_DataRequestMgr->CreatePhysiologyDataRequest("PeripheralPerfusionIndex");
    m_DataRequestMgr->CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
    m_DataRequestMgr->CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
    m_DataRequestMgr->CreateLiquidCompartmentDataRequest("RightArmVasculature", "InFlow", VolumePerTimeUnit::mL_Per_s);
    m_DataRequestMgr->CreateLiquidCompartmentDataRequest("BrainVasculature", "Oxygen", "PartialPressure", PressureUnit::mmHg);

  }
  TCCCIteration::~TCCCIteration()
  {

  }

  void TCCCIteration::Clear()
  {
    m_Actions.clear();
    ScenarioIteration::Clear();
  }

  void TCCCIteration::FixUp()
  {
    if (m_AirwayObstructionSeverity.Empty())
      m_AirwayObstructionSeverity.SetValues({ 0 });
    if (m_HemorrhageSeverity.Empty())
      m_HemorrhageSeverity.SetValues({ 0 });
    if (m_TensionPneumothoraxSeverity.Empty())
      m_TensionPneumothoraxSeverity.SetValues({ 0 });
    if (m_InsultDuration_s.Empty())
      m_InsultDuration_s.SetValues({ 0 });
    if (m_SalineAvailable.Empty())
      m_SalineAvailable.SetValues({ 0 });
    if (m_NeedleAvailable.Empty())
      m_NeedleAvailable.SetValues({ 0 });
    if (m_ChestWrapAvailable.Empty())
      m_ChestWrapAvailable.SetValues({ 0 });
  }

  void TCCCIteration::GenerateSlicedActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename)
  {

  }

  void TCCCIteration::GenerateCombinationActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename)
  {
    std::vector<size_t> opts; // -1 as the opts holds the max index of that option
    opts.push_back(m_AirwayObstructionSeverity.GetValues().size()-1);
    opts.push_back(m_HemorrhageSeverity.GetValues().size()-1);
    opts.push_back(m_HemorrhageLocation.GetValues().size()-1);
    opts.push_back(m_TensionPneumothoraxSeverity.GetValues().size()-1);
    opts.push_back(m_InsultDuration_s.GetValues().size()-1);
    opts.push_back(m_SalineAvailable.GetValues().size()-1);
    opts.push_back(m_NeedleAvailable.GetValues().size()-1);
    opts.push_back(m_ChestWrapAvailable.GetValues().size()-1);

    std::vector<std::vector<size_t>> permutations;
    GeneralMath::Combinations(opts, permutations);

    double AirwayObstructionSeverity = 0;
    double HemorrhageSeverity = 0;
    double TensionPneumothoraxSeverity = 0;
    double InsultDuration_s = 0;
    size_t HemorrhageLocation = 0;

    SetDescription(patientFolderAndStateFilename.first);
    SetEngineStateFile(patientFolderAndStateFilename.second);

    for (auto idxs : permutations)
    {
      AirwayObstructionSeverity = m_AirwayObstructionSeverity.GetValues()[idxs[0]];
      HemorrhageSeverity = m_HemorrhageSeverity.GetValues()[idxs[1]];
      HemorrhageLocation = m_HemorrhageLocation.GetValues()[idxs[2]];
      TensionPneumothoraxSeverity = m_TensionPneumothoraxSeverity.GetValues()[idxs[3]];
      InsultDuration_s = m_InsultDuration_s.GetValues()[idxs[4]];
      if (AirwayObstructionSeverity > 0 ||
          HemorrhageSeverity > 0 ||
          TensionPneumothoraxSeverity > 0)
      {
        GenerateScenario(AirwayObstructionSeverity, HemorrhageSeverity, HemorrhageLocation,
                         TensionPneumothoraxSeverity, InsultDuration_s, patientFolderAndStateFilename.first);
      }
      m_Actions.clear();
    }
  }

  void TCCCIteration::GenerateScenario(double AirwayObstructionSeverity,
                                       double HemorrhageSeverity,
                                       size_t HemorrhageLocation,
                                       double TensionPneumothoraxSeverity,
                                       double InsultDuration_s,
                                       const std::string& PatientName)
  {
    std::string name;
    name =  "AO" + pulse::cdm::to_string(AirwayObstructionSeverity) + "_";
    name += "H"  + pulse::cdm::to_string(HemorrhageSeverity);
    switch ((eHemorrhageLocation)HemorrhageLocation) {
      case eHemorrhageLocation::Leg:
        name += "Leg_";
        break;
      case eHemorrhageLocation::Arm:
        name += "Arm_";
        break;
      case eHemorrhageLocation::Abdomen:
        name += "Abdomen_";
        break;
      case eHemorrhageLocation::_LOC_COUNT:
      default:
        name += "Undefined_";
        break;
    }
    name += "TP" +pulse::cdm::to_string(TensionPneumothoraxSeverity) + "_";
    name += "D"+pulse::cdm::to_string(InsultDuration_s)+"s";
    SetName(PatientName +"/"+name);
    if (m_ScenarioStates.find(m_Name) != m_ScenarioStates.end())
    {
      Info("Ignoring duplicate TCCC scenario: " + m_Name);
      m_Duplicates++;
      return;
    }
    GetDataRequestManager().SetResultsFilename(m_ResultsDirectory+m_Name + ".csv");

    double TotalAdvanceTime_s = 0;
    double FinalAdvanceTime_min = 0;

    // Advance to the insults
    if (m_BaselineDuration_s > 0)
    {
      m_Adv2Insult.GetTime().SetValue(m_BaselineDuration_s, TimeUnit::s);
      m_Actions.push_back(&m_Adv2Insult);
      TotalAdvanceTime_s = m_BaselineDuration_s;
    }

    // Apply the insults
    if (AirwayObstructionSeverity > 0)
    {
      m_AirwayObstruction.GetSeverity().SetValue(AirwayObstructionSeverity);
      m_Actions.push_back(&m_AirwayObstruction);
    }
    if (HemorrhageSeverity > 0)
    {
      m_Hemorrhage.GetSeverity().SetValue(HemorrhageSeverity);
      switch ((eHemorrhageLocation)HemorrhageLocation) {
        case eHemorrhageLocation::Leg:
          m_Hemorrhage.SetCompartment(eHemorrhage_Compartment::RightLeg);
          break;
        case eHemorrhageLocation::Arm:
          m_Hemorrhage.SetCompartment(eHemorrhage_Compartment::RightArm);
          break;
        case eHemorrhageLocation::Abdomen:
          m_Hemorrhage.SetCompartment(eHemorrhage_Compartment::Splanchnic);
          break;
        case eHemorrhageLocation::_LOC_COUNT:
        default:
          break;
      }
      m_Actions.push_back(&m_Hemorrhage);
    }
    if (TensionPneumothoraxSeverity > 0)
    {
      m_TensionPneumothorax.GetSeverity().SetValue(TensionPneumothoraxSeverity);
      m_Actions.push_back(&m_TensionPneumothorax);
    }

    // Advance the duration of the insult
    if (InsultDuration_s > 0)
    {
      TotalAdvanceTime_s += InsultDuration_s;
      m_Adv2Intervention.GetTime().SetValue(InsultDuration_s, TimeUnit::s);
      m_Actions.push_back(&m_Adv2Intervention);
    }

    // Save State
    m_Serialize.SetFilename(m_StateDirectory+"/"+m_Name+".pbb");
    m_Actions.push_back(&m_Serialize);

    if (m_PerformInterventions)
    {
      // TODO
    }

    // Advance the duration of the scenario
    FinalAdvanceTime_min = m_MaxSimTime_min - (TotalAdvanceTime_s / 60);
    if (FinalAdvanceTime_min > 0)
    {
      m_Adv2End.GetTime().SetValue(FinalAdvanceTime_min, TimeUnit::min);
      m_Actions.push_back(&m_Adv2End);
    }

    // Track and Write the scenario
    WriteScenario();
  }
}
