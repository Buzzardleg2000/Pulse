/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "ScenarioIteration.h"

namespace pulse::study::patient_variability
{
  ScenarioIteration::ScenarioIteration(Logger& logger) : PulseScenario(&logger)
  {
    m_GenStyle = eGenStyle::Combo;
    m_Duplicates = 0;
    m_NumScenarios = 0;
  }
  ScenarioIteration::~ScenarioIteration()
  {

  }

  void ScenarioIteration::Clear()
  {
    PulseScenario::Clear();
  }

  bool ScenarioIteration::GenerateScenarios(const PatientIteration& patients, const std::string destDir)
  {
    FixUp();
    m_NumScenarios = 0;

    for (auto& itr : patients.GetPatientStates())
    {
      switch (m_GenStyle)
      {
      case eGenStyle::Combo:
        GenerateCombinationActionSets(itr, destDir);
        break;
      case eGenStyle::Slice:
        GenerateSlicedActionSets(itr, destDir);
        break;
      }
    }

    Info("Removed " + std::to_string(m_Duplicates) + " duplicates");
    Info("Defined " + std::to_string(m_NumScenarios) + " scenarios");

    return true;
  }
}
