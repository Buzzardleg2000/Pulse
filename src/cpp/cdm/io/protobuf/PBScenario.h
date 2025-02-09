/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Scenario)
CDM_BIND_DECL2(ScenarioExec)

class CDM_DECL PBScenario
{
public:

  static void Load(const CDM_BIND::ScenarioData& src, SEScenario& dst);
  static CDM_BIND::ScenarioData* Unload(const SEScenario& src);
  static void Serialize(const CDM_BIND::ScenarioData& src, SEScenario& dst);
  static void Serialize(const SEScenario& src, CDM_BIND::ScenarioData& dst);
  static void Copy(const SEScenario& src, SEScenario& dst);

  static bool SerializeToString(const SEScenario& src, std::string& output, eSerializationFormat m);
  static bool SerializeToFile(const SEScenario& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SEScenario& dst, eSerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEScenario& dst);

  static void Copy(const SEScenarioExec& src, SEScenarioExec& dst);
  static void Load(const CDM_BIND::ScenarioExecData& src, SEScenarioExec& dst);
  static CDM_BIND::ScenarioExecData* Unload(const SEScenarioExec& src);
  static void Serialize(const CDM_BIND::ScenarioExecData& src, SEScenarioExec& dst);
  static void Serialize(const SEScenarioExec& src, CDM_BIND::ScenarioExecData& dst);

  static bool SerializeToString(const SEScenarioExec& src, std::string& output, eSerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, SEScenarioExec& dst, eSerializationFormat m, Logger* logger);
};