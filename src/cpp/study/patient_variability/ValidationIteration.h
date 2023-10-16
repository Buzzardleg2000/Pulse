/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "ScenarioIteration.h"
#include "cdm/engine/SEAdvanceTime.h"

namespace pulse::study::patient_variability
{
  class ValidationIteration : public ScenarioIteration
  {
  public:
    ValidationIteration(Logger& logger);
    virtual ~ValidationIteration();

    void Clear() override;

  protected:
    void GenerateScenarios(std::pair<std::string, std::string>);
    void GenerateSlicedActionSets(std::pair<std::string, std::string>) override;
    void GenerateCombinationActionSets(std::pair<std::string, std::string>) override;


    // Stateless
    SEAdvanceTime         m_Adv;
  };
}
