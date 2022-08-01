/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/utils/Logger.h"
#include "cdm/utils/FileUtils.h"

class SEAction;
class SEScenario;
class PhysiologyEngine;
class SEEngineConfiguration;

class CDM_DECL SEScenarioExec : public Loggable
{
public:
  SEScenarioExec(Logger* logger);
  virtual ~SEScenarioExec();

  virtual void Clear();
  void Copy(const SEScenarioExec& src);

  void SetLoggerForward(LoggerForward* lf) { m_LoggerForward = lf; }

  eSwitch LogToConsole() const { return m_LogToConsole; }
  void LogToConsole(eSwitch s) { m_LogToConsole = s; }

  std::string GetDataRootDirectory() const { return m_DataRootDirectory; }
  void SetDataRootDirectory(const std::string& fn) { m_DataRootDirectory = fn; }

  std::string GetOutputRootDirectory() const { return m_OutputRootDirectory; }
  void SetOutputRootDirectory(const std::string& d) { m_OutputRootDirectory = d; }

  eSwitch OrganizeOutputDirectory() const { return m_OrganizeOutputDirectory; }
  void OrganizeOutputDirectory(eSwitch s) { m_OrganizeOutputDirectory = s; }

  std::string GetScenarioContent() const { return m_ScenarioContent; }
  void SetScenarioContent(const std::string& sc)
  {
    m_ScenarioContent = sc;
    m_ScenarioFilename = "";
    m_ScenarioDirectory = "";
  }
  std::string GetScenarioFilename() const { return m_ScenarioFilename; }
  void SetScenarioFilename(const std::string& fn)
  {
    m_ScenarioContent = "";
    m_ScenarioFilename = fn;
    m_ScenarioDirectory = "";
  }
  std::string GetScenarioDirectory() const { return m_ScenarioDirectory; }
  void SetScenarioDirectory(const std::string& dir)
  {
    m_ScenarioContent = "";
    m_ScenarioFilename = "";
    m_ScenarioDirectory = dir;
  }

  eSerializationFormat GetContentFormat() const { return m_ContentFormat; }
  void SetContentFormat(eSerializationFormat s) { m_ContentFormat = s; }

  int GetThreadCount() const { return m_ThreadCount; }
  void SetThreadCount(int c) { m_ThreadCount = c; }

  std::string GetEngineConfigurationContent() const { return m_EngineConfigurationContent; }
  void SetEngineConfigurationContent(const std::string& ecc)
  {
    m_EngineConfigurationContent = ecc;
    m_EngineConfigurationFilename = "";
  }
  std::string GetEngineConfigurationFilename() const { return m_EngineConfigurationFilename; }
  void SetEngineConfigurationFilename(const std::string& fn)
  {
    m_EngineConfigurationContent = "";
    m_EngineConfigurationFilename = fn;
  }


  // These parameters will help debug any serialization issues.
  // A serialization issue is where get different results from running the same scenario where we
  // 1. run we run the scenario as is
  // 2. run the scenario but serialized out and in (after actions, or any time really)
  // The two generated CSV files should match as well as match the baseline
  // If they do not, then we are not maintaining state properly to the file and back into the engine
  // To find serialization issues, uncomment out the AutoSerialization line in the ScenarioVerification.config
  // Then run the `run ScenarioVerification` command to run all scenarios, have then save out and in after each action
  // This test will also compare the resulting CSV file to the baseline CSV that did not save in and out
  // If any scenario fails this comparison... run it through here with this block turned on.
  // To debug the issue, this block will:
  // 1. Run one run where we save out states after we apply an action and after the subsequent time step
  // 2. Run another run where we save out the same times, but we reload the state after the first save
  // This gives us two states at the same point in time, per action, that we can compare to see what is different
  // Both times can have differences depending on the bug...good luck!

  eSwitch AutoSerializeAfterActions() const { return m_AutoSerializeAfterActions; }
  void AutoSerializeAfterActions(eSwitch s) { m_AutoSerializeAfterActions = s; }

  double GetAutoSerializePeriod_s() const { return m_AutoSerializePeriod_s; }
  void SetAutoSerializePeriod_s(double s) { m_AutoSerializePeriod_s = s; }

  eSwitch TimeStampSerializedStates() const { return m_TimeStampSerializedStates; }
  void TimeStampSerializedStates(eSwitch s) { m_TimeStampSerializedStates = s; }

  bool SerializeToString(std::string& output, eSerializationFormat m, Logger* logger=nullptr) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, Logger* logger=nullptr);

protected:

  bool Execute(PhysiologyEngine& pe, SEScenario& sce);
  bool Process(PhysiologyEngine& pe, SEScenario& sce);
  bool ProcessActions(PhysiologyEngine& pe, SEScenario& sce);
  /// This does not include advance time actions
  /// To override default functionality with those 
  /// actions override the ProcessActions method
  bool ProcessAction(PhysiologyEngine& pe, SEAction& action);
  void AdvanceEngine(PhysiologyEngine& pe);

  LoggerForward* m_LoggerForward;

  eSwitch     m_LogToConsole;
  std::string m_DataRootDirectory;
  std::string m_OutputRootDirectory;
  eSwitch     m_OrganizeOutputDirectory;

  // These are computed based on the scenario name
  std::string m_BaseFilename;
  std::string m_LogFilename;
  std::string m_DataRequestCSVFilename;

  // Note, its up to the deriving exec's to merge configs
  // The basic config is empty
  std::string m_EngineConfigurationContent;
  std::string m_EngineConfigurationFilename;

  std::string m_ScenarioContent;
  std::string m_ScenarioFilename;
  std::string m_ScenarioDirectory;

  // For both the EC and Scenrio Content
  eSerializationFormat m_ContentFormat;

  int                        m_ThreadCount;

  // Settings for serialization
  bool                       m_SaveNextStep;
  std::string                m_AutoSerializeFilename;
  std::string                m_AutoSerializeBaseFilename;
  std::string                m_AutoSerializeFilenameExt;
  eSwitch                    m_AutoSerializeAfterActions;
  double                     m_AutoSerializePeriod_s;
  double                     m_AutoSerializeTime_s;
  eSwitch                    m_TimeStampSerializedStates;
  eSwitch                    m_ReloadSerializedState;
  std::stringstream          m_SerializationOutput;
  std::stringstream          m_SerializationActions;
};