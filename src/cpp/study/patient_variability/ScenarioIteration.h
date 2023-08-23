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

    eGenStyle GetGenStyle() const { return m_GenStyle; }
    void SetGenStyle(eGenStyle s) { m_GenStyle = s; }

    bool CreateStates() const { return m_CreateStates; }
    void CreateStates(bool b) { m_CreateStates = b; }

    double GetBaselineDuration_s() const { return m_BaselineDuration_s; }
    void SetBaselineDuration_s(double d) { m_BaselineDuration_s = d; }

    double GetMaxSimTime_min() const { return m_MaxSimTime_min; }
    void SetMaxSimTime_min(double d) { m_MaxSimTime_min = d; }

    virtual bool GenerateScenarios(const PatientIteration& patients, const std::string destDir);

  protected:
    virtual void FixUp() {};
    virtual void GenerateSlicedActionSets(std::pair<std::string,std::string>, const std::string destDir) = 0;
    virtual void GenerateCombinationActionSets(std::pair<std::string, std::string>, const std::string destDir) = 0;

    // Statefull
    eGenStyle                                     m_GenStyle;
    bool                                          m_CreateStates;
    double                                        m_BaselineDuration_s;
    double                                        m_MaxSimTime_min;
    // Stateless
    size_t                                        m_Duplicates;
    size_t                                        m_NumScenarios;
  };
}
