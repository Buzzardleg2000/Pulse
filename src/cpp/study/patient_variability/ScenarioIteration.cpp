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

  void ScenarioIteration::SetScenarioDirectory(const std::string& d)
  {
    if (d.empty())
      m_ScenarioDirectory = "./test_results/patient_variability/"+GetIterationName()+"/scenarios/";
    else
      m_ScenarioDirectory = d;
    if (m_ScenarioDirectory.back() != '/')
      m_ScenarioDirectory = m_ScenarioDirectory + "/";
  }

  void ScenarioIteration::SetStateDirectory(const std::string& d)
  {
    if (d.empty())
      m_StateDirectory = "./test_results/patient_variability/"+GetIterationName()+"/states/";
    else
      m_StateDirectory = d;
    if (m_StateDirectory.back() != '/')
      m_StateDirectory = m_StateDirectory + "/";
  }

  bool ScenarioIteration::GenerateScenarios(const PatientIteration& patients)
  {
    FixUp();
    m_NumScenarios = 0;
    m_ScenarioStates.clear();

    Info("Generating " + GetIterationName() + " scenarios to: " + m_ScenarioDirectory);
    Info("Generating " + GetIterationName() + " states to: " + m_StateDirectory);

    for (auto& itr : patients.GetPatientStates())
    {
      switch (m_GenStyle)
      {
      case eGenStyle::Combo:
        GenerateCombinationActionSets(itr);
        break;
      case eGenStyle::Slice:
        GenerateSlicedActionSets(itr);
        break;
      }
    }

    Info("Removed " + std::to_string(m_Duplicates) + " duplicate scenarios");
    Info("Defined " + std::to_string(m_NumScenarios) + " scenarios");

    return true;
  }
}
