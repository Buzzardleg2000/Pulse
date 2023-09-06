/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "ValidationIteration.h"

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse::study::patient_variability
{
  ValidationIteration::ValidationIteration(Logger& logger) : ScenarioIteration(logger)
  {
    // This is where the python data generator puts these validation data request files
    // The ./ is implied
    m_DataRequestFiles.push_back("validation/requests/BloodChemistry.json");
    m_DataRequestFiles.push_back("validation/requests/Cardiovascular.json");
    m_DataRequestFiles.push_back("validation/requests/CardiovascularCompartments.json");
    m_DataRequestFiles.push_back("validation/requests/Endocrine.json");
    m_DataRequestFiles.push_back("validation/requests/Energy.json");
    m_DataRequestFiles.push_back("validation/requests/Gastrointestinal.json");
    m_DataRequestFiles.push_back("validation/requests/Renal.json");
    m_DataRequestFiles.push_back("validation/requests/RenalCompartments.json");
    m_DataRequestFiles.push_back("validation/requests/RenalSubstances.json");
    m_DataRequestFiles.push_back("validation/requests/Respiratory.json");
    m_DataRequestFiles.push_back("validation/requests/RespiratoryCompartments.json");
    m_DataRequestFiles.push_back("validation/requests/Tissue.json");
    m_DataRequestFiles.push_back("validation/requests/TissueCompartments.json");
    m_DataRequestFiles.push_back("validation/requests/TissueSubstances.json");
    m_DataRequestMgr->CreatePatientDataRequest("Weight", MassUnit::g);
    m_DataRequestMgr->CreatePatientDataRequest("IdealBodyWeight", MassUnit::g);


    m_Adv.GetTime().SetValue(2, TimeUnit::min);
    m_Actions.push_back(&m_Adv);
  }
  ValidationIteration::~ValidationIteration()
  {
    m_Actions.clear();
  }

  void ValidationIteration::GenerateSlicedActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    GenerateScenarios(patientFolderAndStateFilename, destDir);
  }

  void ValidationIteration::GenerateCombinationActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    GenerateScenarios(patientFolderAndStateFilename, destDir);
  }

  void ValidationIteration::GenerateScenarios(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    m_NumScenarios++;
    SetName(patientFolderAndStateFilename.first);
    SetDescription("Generate data for validation");
    SetEngineStateFile(patientFolderAndStateFilename.second);
    GetDataRequestManager().SetResultsFilename(destDir + "/results/validation/" + patientFolderAndStateFilename.first + ".csv");
    SerializeToFile(destDir + "/scenarios/validation/" + m_Name + ".json");
  }
}
