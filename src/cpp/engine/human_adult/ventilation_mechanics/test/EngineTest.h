/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "pulsetests_export.h"
#include "cdm/utils/testing/SETestManager.h"
#include "engine/human_adult/ventilation_mechanics/Engine.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  class PULSETESTS_EXPORT EngineTest : public SETestManager
  {
  public:
    EngineTest(Logger* logger = nullptr);
    virtual ~EngineTest();

    virtual bool RunTest(const std::string& testName, const std::string& sOutputDirectory) override;

  protected:
    virtual void FillFunctionMap() override;
    typedef void(EngineTest::* testFunction)(const std::string&);
    std::map<std::string, testFunction> testMap;
    std::stringstream m_ss;

  public:
    void SmokeTest(const std::string& sTestDirectory);
    void GenerateScenarios();
  };
END_NAMESPACE_EX
