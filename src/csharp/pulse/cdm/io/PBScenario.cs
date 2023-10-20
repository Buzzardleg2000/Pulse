/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBScenario
  {
    #region SEScenarioExec
    public static void Load(pulse.cdm.bind.ScenarioExecData src, SEScenarioExec dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScenarioExecData src, SEScenarioExec dst)
    {
      dst.Clear();
      dst.SetLogToConsole((eSwitch)(int)src.LogToConsole);
      if (!string.IsNullOrEmpty(src.DataRootDirectory))
        dst.SetDataRootDirectory(src.DataRootDirectory);
      if (!string.IsNullOrEmpty(src.OutputRootDirectory))
        dst.SetOutputRootDirectory(src.OutputRootDirectory);
      dst.SetOrganizeOutputDirectory((eSwitch)(int)src.OrganizeOutputDirectory);

      dst.SetAutoSerializeAfterActions((eSwitch)(int)src.AutoSerializeAfterActions);
      dst.SetAutoSerializePeriod_s(src.AutoSerializePeriodS);
      dst.SetTimeStampSerializedStates((eSwitch)(int)src.TimeStampSerializedStates);

      if (!string.IsNullOrEmpty(src.EngineConfigurationContent))
        dst.SetEngineConfigurationContent(src.EngineConfigurationContent);
      else if (!string.IsNullOrEmpty(src.EngineConfigurationFilename))
        dst.SetEngineConfigurationFilename(src.EngineConfigurationFilename);

      if (!string.IsNullOrEmpty(src.ScenarioContent))
        dst.SetScenarioContent(src.ScenarioContent);
      else if (!string.IsNullOrEmpty(src.ScenarioFilename))
        dst.SetScenarioFilename(src.ScenarioFilename);
      else if ( !string.IsNullOrEmpty(src.ScenarioDirectory))
        dst.SetScenarioDirectory(src.ScenarioDirectory);

      dst.SetContentFormat((eSerializationFormat)(int)src.ContentFormat);
      dst.SetThreadCount(src.ThreadCount);
    }
    public static bool SerializeFromString(string src, SEScenarioExec dst)
    {
      try
      {
        pulse.cdm.bind.ScenarioExecData data = JsonParser.Default.Parse<pulse.cdm.bind.ScenarioExecData>(src);
        PBScenario.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ScenarioExecData Unload(SEScenarioExec src)
    {
      pulse.cdm.bind.ScenarioExecData dst = new pulse.cdm.bind.ScenarioExecData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScenarioExec src, pulse.cdm.bind.ScenarioExecData dst)
    {
      dst.LogToConsole = (pulse.cdm.bind.eSwitch)(int)src.GetLogToConsole();
      dst.DataRootDirectory = src.GetDataRootDirectory();
      dst.OutputRootDirectory = src.GetOutputRootDirectory();
      dst.OrganizeOutputDirectory = (pulse.cdm.bind.eSwitch)(int)src.GetOrganizeOutputDirectory();

      dst.AutoSerializeAfterActions = (pulse.cdm.bind.eSwitch)(int)src.GetAutoSerializeAfterActions();
      dst.AutoSerializePeriodS = src.GetAutoSerializePeriod_s();
      dst.TimeStampSerializedStates = (pulse.cdm.bind.eSwitch)(int)src.GetTimeStampSerializedStates();

      if(!string.IsNullOrEmpty(src.GetEngineConfigurationContent()))
        dst.EngineConfigurationContent = src.GetEngineConfigurationContent();
      else if (!string.IsNullOrEmpty(src.GetEngineConfigurationFilename()))
        dst.EngineConfigurationFilename = src.GetEngineConfigurationFilename();

      if (!string.IsNullOrEmpty(src.GetScenarioContent()))
        dst.ScenarioContent = src.GetScenarioContent();
      else if (!string.IsNullOrEmpty(src.GetScenarioFilename()))
        dst.ScenarioFilename = src.GetScenarioFilename();
      else if (!string.IsNullOrEmpty(src.GetScenarioDirectory()))
        dst.ScenarioDirectory = src.GetScenarioDirectory();

      dst.ContentFormat = (pulse.cdm.bind.eSerializationFormat)(int)src.GetContentFormat();
      dst.ThreadCount = src.GetThreadCount();

    }
    public static string SerializeToString(SEScenarioExec src)
    {
      var pb = PBScenario.Unload(src);
      return pb.ToString();
    }
    #endregion

    #region SEScenario
    public static void Load(pulse.cdm.bind.ScenarioData src, SEScenario dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScenarioData src, SEScenario dst)
    {
      dst.Clear();
      dst.SetName(src.Name);
      dst.SetDescription(src.Description);
      if (!string.IsNullOrEmpty(src.EngineStateFile))
        dst.SetEngineState(src.EngineStateFile);
      else if (src.PatientConfiguration != null)
        PBPatientConfiguration.Load(src.PatientConfiguration, dst.GetPatientConfiguration());
      if (src.DataRequestManager != null)
        PBDataRequest.Load(src.DataRequestManager, dst.GetDataRequestManager());
      foreach (var action in src.AnyAction)
        dst.GetActions().Add(PBAction.Load(action));
    }

    public static bool SerializeFromString(string src, SEScenario dst)
    {
      try
      {
        pulse.cdm.bind.ScenarioData data = JsonParser.Default.Parse<pulse.cdm.bind.ScenarioData>(src);
        PBScenario.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidProtocolBufferException)
      {
        try
        {
          pulse.engine.bind.ScenarioData data = JsonParser.Default.Parse<pulse.engine.bind.ScenarioData>(src);
          PBScenario.Load(data.Scenario, dst);
        }
        catch (Google.Protobuf.InvalidProtocolBufferException)
        {
          return false;
        }
      }
      return true;
    }

    public static pulse.cdm.bind.ScenarioData Unload(SEScenario src)
    {
      pulse.cdm.bind.ScenarioData dst = new pulse.cdm.bind.ScenarioData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScenario src, pulse.cdm.bind.ScenarioData dst)
    {
      dst.Name = src.GetName();
      dst.Description = src.GetDescription();
      if (src.HasEngineState())
        dst.EngineStateFile = src.GetEngineState();
      else if (src.HasPatientConfiguration())
        dst.PatientConfiguration = PBPatientConfiguration.Unload(src.GetPatientConfiguration());
      dst.DataRequestManager = PBDataRequest.Unload(src.GetDataRequestManager());
      if (src.GetActions().Count > 0)
      {
        //dst.AnyAction = new Google.Protobuf.Collections.RepeatedField<pulse.cdm.bind.AnyActionData>();
        foreach (var action in src.GetActions())
          dst.AnyAction.Add(PBAction.Unload(action));
      }
    }
    public static string SerializeToString(SEScenario src)
    {
      var pb = PBScenario.Unload(src);
      return pb.ToString();
    }
    #endregion
  }
}

