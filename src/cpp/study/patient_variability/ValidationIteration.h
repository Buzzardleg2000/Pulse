/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "ScenarioIteration.h"

namespace pulse::study::patient_variability
{
  class ValidationIteration : public ScenarioIteration
  {
  public:
    ValidationIteration(Logger& logger);
    virtual ~ValidationIteration();

  protected:
    void GenerateScenarios(std::pair<std::string, std::string>, const std::string destDir);
    void GenerateSlicedActionSets(std::pair<std::string, std::string>, const std::string destDir) override;
    void GenerateCombinationActionSets(std::pair<std::string, std::string>, const std::string destDir) override;
  };
}
