/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "TCCCIteration.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse::study::patient_variability
{
  TCCCIteration::TCCCIteration(Logger& logger) : ScenarioIteration(logger)
  {
    m_PerformInterventions = false;

    m_Hemorrhage.SetType(eHemorrhage_Type::External);
    m_Hemorrhage.SetCompartment(eHemorrhage_Compartment::VenaCava);
    m_TensionPneumothorax.SetType(eGate::Open);
    m_TensionPneumothorax.SetSide(eSide::Left);

    m_DataRequestMgr->CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    m_DataRequestMgr->CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
    m_DataRequestMgr->CreatePhysiologyDataRequest("OxygenSaturation");
    m_DataRequestMgr->CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
    m_DataRequestMgr->CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
    m_DataRequestMgr->CreateLiquidCompartmentDataRequest("RightArmVasculature", "InFlow", FrequencyUnit::Per_min);
    m_DataRequestMgr->CreateLiquidCompartmentDataRequest("BrainVasculature", "Oxygen", "PartialPressure", PressureUnit::mmHg);
  }
  TCCCIteration::~TCCCIteration()
  {

  }

  void TCCCIteration::Clear()
  {
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

  void TCCCIteration::GenerateSlicedActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {

  }

  void TCCCIteration::GenerateCombinationActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    std::vector<size_t> opts; // -1 as the opts holds the max index of that option
    opts.push_back(m_AirwayObstructionSeverity.GetValues().size()-1);
    opts.push_back(m_HemorrhageSeverity.GetValues().size()-1);
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

    SetEngineStateFile(patientFolderAndStateFilename.second);

    SetEngineStateFile(patientFolderAndStateFilename.second);
    for (auto idxs : permutations)
    {
      AirwayObstructionSeverity = m_AirwayObstructionSeverity.GetValues()[idxs[0]];
      HemorrhageSeverity = m_HemorrhageSeverity.GetValues()[idxs[1]];
      TensionPneumothoraxSeverity = m_TensionPneumothoraxSeverity.GetValues()[idxs[2]];
      InsultDuration_s = m_InsultDuration_s.GetValues()[idxs[3]];
      if (AirwayObstructionSeverity > 0 &&
          HemorrhageSeverity > 0 &&
          TensionPneumothoraxSeverity > 0)
      {
        GenerateScenario(AirwayObstructionSeverity, HemorrhageSeverity, TensionPneumothoraxSeverity, InsultDuration_s);
        // Write the scenario
        SerializeToFile(destDir + "/TCCC/" + patientFolderAndStateFilename.first + "/" + m_Name + ".json");
      }
      m_Actions.clear();
    }
  }

  void TCCCIteration::GenerateScenario(double AirwayObstructionSeverity,
                                           double HemorrhageSeverity,
                                           double TensionPneumothoraxSeverity,
                                           double InsultDuration_s)
  {
    std::string name;
    name =  "AO" + pulse::cdm::to_string(AirwayObstructionSeverity) + "_";
    name += "H"  + pulse::cdm::to_string(HemorrhageSeverity) + "_";
    name += "TP" +pulse::cdm::to_string(TensionPneumothoraxSeverity) + "_";
    name += "D"+pulse::cdm::to_string(InsultDuration_s)+"s";
    SetName(name);

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
      m_Actions.push_back(&m_Hemorrhage);
    }
    if (TensionPneumothoraxSeverity > 0)
    {
      m_TensionPneumothorax.GetSeverity().SetValue(TensionPneumothoraxSeverity);
      m_Actions.push_back(&m_TensionPneumothorax);
    }

    // Save State

    // Advance the duration of the insult
    if (InsultDuration_s > 0)
    {
      TotalAdvanceTime_s += InsultDuration_s;
      m_Adv2Intervention.GetTime().SetValue(InsultDuration_s, TimeUnit::s);
      m_Actions.push_back(&m_Adv2Intervention);
    }

    // Save State

    if (m_PerformInterventions)
    {
      // TODO
    }

    // Advance the duration of the scenario
    FinalAdvanceTime_min = m_MaxSimTime_min - (TotalAdvanceTime_s / 60);
    if (FinalAdvanceTime_min > 0)
    {
      m_Adv2End.GetTime().SetValue(FinalAdvanceTime_min, TimeUnit::s);
      m_Actions.push_back(&m_Adv2End);
    }
  }
}
