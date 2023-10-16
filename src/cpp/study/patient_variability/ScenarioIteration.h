/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulseScenario.h"

#include "PatientIteration.h"

namespace pulse::study::patient_variability
{
  class ScenarioIteration : public PulseScenario
  {
  public:
    ScenarioIteration(Logger& logger);
    virtual ~ScenarioIteration();

    void Clear() override;

    virtual void SetIterationName(const std::string& n) { m_IterationName = n; }
    virtual std::string GetIterationName() const { return m_IterationName; }

    void SetScenarioDirectory(const std::string& d);
    std::string GetScenarioDirectory() const { return m_ScenarioDirectory; }

    void SetStateDirectory(const std::string& d);
    std::string GetStateDirectory() const { return m_StateDirectory; }

    eGenStyle GetGenStyle() const { return m_GenStyle; }
    void SetGenStyle(eGenStyle s) { m_GenStyle = s; }

    bool CreateStates() const { return m_CreateStates; }
    void CreateStates(bool b) { m_CreateStates = b; }

    double GetBaselineDuration_s() const { return m_BaselineDuration_s; }
    void SetBaselineDuration_s(double d) { m_BaselineDuration_s = d; }

    double GetMaxSimTime_min() const { return m_MaxSimTime_min; }
    void SetMaxSimTime_min(double d) { m_MaxSimTime_min = d; }

    virtual bool GenerateScenarios(const PatientIteration& patients);

  protected:
    virtual void FixUp() {};
    virtual void GenerateSlicedActionSets(std::pair<std::string,std::string>) = 0;
    virtual void GenerateCombinationActionSets(std::pair<std::string, std::string>) = 0;

    // Statefull
    eGenStyle                                     m_GenStyle;
    bool                                          m_CreateStates;
    double                                        m_BaselineDuration_s;
    double                                        m_MaxSimTime_min;
    std::string                                   m_IterationName;
    std::string                                   m_ScenarioDirectory;
    std::string                                   m_StateDirectory;

    // Stateless
    size_t                                        m_Duplicates;
    size_t                                        m_NumScenarios;
    std::map<std::string, std::string>            m_ScenarioStates;
  };
}
