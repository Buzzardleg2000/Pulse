/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseConfiguration.h"
#include "PulseScenario.h"
#include "PulseScenarioExec.h"
#include "engine/common/Engine.h"
#include "engine/io/protobuf/PBScenario.h"

#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/ConfigParser.h"
#include "cdm/utils/ThreadPool.h"


void PulseScenarioExec::Clear()
{
  SEScenarioExec::Clear();
  m_ModelType = eModelType::HumanAdultWholeBody;
}

void PulseScenarioExec::Copy(const PulseScenarioExec& src)
{
  pulse::PBScenario::Copy(src, *this);
}

bool PulseScenarioExec::SerializeToString(std::string& output, eSerializationFormat m, Logger* logger) const
{
  return pulse::PBScenario::SerializeToString(*this, output, m, logger);
}
bool PulseScenarioExec::SerializeFromString(const std::string& src, eSerializationFormat m, Logger* logger)
{
  return pulse::PBScenario::SerializeFromString(src, *this, m, logger);
}

bool PulseScenarioExec::ExecuteOpts(PulseScenarioExec* opts, PulseScenario* sce, SEScenarioExecStatus* status)
{
  if (!opts->GetScenarioFilename().empty())
  {
    if (sce == nullptr)
    {
      opts->Error("No scenario provided");
      return false;
    }
    if (status == nullptr)
    {
      opts->Error("No scenario status provided");
      return false;
    }
    opts->Info("Executing Scenario: " + opts->GetScenarioFilename());
    bool b = opts->Execute(*sce, status);
    if(b)
      opts->Info("Completed Scenario: " + opts->GetScenarioFilename());
    else
      opts->Error("Error Executing Scenario: " + opts->GetScenarioFilename());
    return b;
  }
  else if (!opts->GetScenarioLogFilename().empty())
  {
    opts->Info("Converting Log: " + opts->GetScenarioLogFilename());
    bool b = opts->ConvertLog();
    if(b)
      opts->Info("Completed Log Conversion: " + opts->GetScenarioLogFilename());
    else
      opts->Error("Error Executing Log Conversion: " + opts->GetScenarioLogFilename());
    return b;
  }
  opts->Error("No scenario/scenario log provided.");
  return false;
}
bool PulseScenarioExec::Execute()
{
  std::string scenarioPath, scenarioFilename;
  if (!GetScenarioContent().empty())
  {
    PulseScenario sce(GetLogger(), GetDataRootDirectory());
    if (!sce.SerializeFromString(GetScenarioContent(), GetContentFormat()))
      return false;
    return Execute(sce);
  }
  else if (!GetScenarioFilename().empty())
  {
    SEScenarioExecStatus status(GetLogger());
    PulseScenario sce(GetLogger(), GetDataRootDirectory());
    SplitPathFilename(GetScenarioFilename(), scenarioPath, scenarioFilename);
    GetDataRequestFilesSearch().insert(scenarioPath);
    if (!sce.SerializeFromFile(GetScenarioFilename()))
      return false;
    return Execute(sce, &status);
  }
  else if (!GetScenarioDirectory().empty() || !GetScenarioExecListFilename().empty())
  {
    std::vector<SEScenarioExecStatus*> statuses;
    if (!GetScenarioExecListFilename().empty())
    {
      if (!SEScenarioExecStatus::SerializeFromFile(GetScenarioExecListFilename(), statuses, GetLogger()))
      {
        Fatal("Unable to serialize ScenarioExecListFilename " + GetScenarioExecListFilename());
        return false;
      }
    }
    else if (!GetScenarioDirectory().empty())
    {
      std::vector<std::string> scenarios;
      ListFiles(GetScenarioDirectory(), scenarios, true, ".json");
      for (auto filename : scenarios)
      {
        SEScenarioExecStatus* status = new SEScenarioExecStatus(GetLogger());
        status->SetScenarioFilename(filename);
        statuses.push_back(status);
      }
    }
    size_t numThreadsToUse = ComputeNumThreads();
    if (numThreadsToUse <= 0)
    {
      Fatal("Unable to compute the number of threads to use");
      return false;
    }
    // Let's not kick off more threads than we need
    if (numThreadsToUse > statuses.size())
      numThreadsToUse = statuses.size();

    ThreadPool pool(numThreadsToUse);
    for (auto status : statuses)
    {
      PulseScenarioExec* opts = new PulseScenarioExec(GetLogger());
      opts->Copy(*this);
      opts->SetScenarioFilename(status->GetScenarioFilename());
      SplitPathFilename(GetScenarioFilename(), scenarioPath, scenarioFilename);
      opts->GetDataRequestFilesSearch().insert(scenarioPath);

      // Always have sceanrios create their own logger when threaded
      PulseScenario* sce = new PulseScenario(GetDataRootDirectory());
      sce->SerializeFromFile(GetScenarioFilename());
      pool.enqueue(ExecuteOpts, opts, sce, status);
    }

    /* ThreadPool waits for threads to complete in its
    *  destructor which happens implicitly as we leave
    *  function scope. If we need return values from
    *  ExecuteScenario calls, enqueue returns a std::future
    */
  }
  else if (!GetScenarioLogFilename().empty())
  {
    return ConvertLog();
  }
  else if (!GetScenarioLogDirectory().empty())
  {
    size_t numThreadsToUse = ComputeNumThreads();
    if (numThreadsToUse <= 0)
    {
      Fatal("Unable to compute the number of threads to use");
      return false;
    }
    // Let's get all the scenarios and create a thread pool
    std::vector<std::string> logs;
    ListFiles(GetScenarioLogDirectory(), logs, true, ".log", ".cnv.log");
    // Let's not kick off more threads than we need
    if (numThreadsToUse > logs.size())
      numThreadsToUse = logs.size();

    ThreadPool pool(numThreadsToUse);
    std::vector<std::future<bool>> futures;
    std::vector<PulseScenarioExec*> opts;
    for (auto filename : logs)
    {
      opts.emplace_back(new PulseScenarioExec(GetLogger()));
      opts.back()->Copy(*this);
      opts.back()->SetScenarioLogFilename(filename);
      futures.emplace_back(pool.enqueue(ExecuteOpts, opts.back(), nullptr, nullptr));
    }

    bool success = true;
    for (size_t i = 0; i < futures.size(); ++i)
    {
      if (!futures[i].get())
      {
        Error("Failed to convert " + opts[i]->m_ScenarioLogFilename);
        success = false;
      }
    }

    for ( PulseScenarioExec* o: opts)
      delete o;

    return success;
  }

  Error("No scenario content/log provided");
  return false;
}

bool PulseScenarioExec::Execute(PulseScenario& sce, SEScenarioExecStatus* status)
{
  auto pe = CreatePulseEngine(m_ModelType, sce.GetLogger());

  if (!GetEngineConfigurationFilename().empty())
  {
    PulseConfiguration pc(sce.GetLogger());
    if (!pc.SerializeFromFile(GetEngineConfigurationFilename(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  else if (!GetEngineConfigurationContent().empty())
  {
    PulseConfiguration pc(sce.GetLogger());
    if (!pc.SerializeFromString(GetEngineConfigurationFilename(), GetContentFormat(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  if (sce.HasConfiguration())
    pe->SetConfigurationOverride(&sce.GetConfiguration());

  return SEScenarioExec::Execute(*pe, sce, status);
}

bool PulseScenarioExec::ConvertLog()
{
  return SEScenarioExec::ConvertLog();
}

size_t PulseScenarioExec::ComputeNumThreads()
{
  size_t numThreadsSupported = std::thread::hardware_concurrency();
  if (numThreadsSupported == 0)
  {
    Fatal("Unable to detect number of processors");
    return 0;
  }

  size_t numThreadsToUse;
  if (m_ThreadCount > 0)
    numThreadsToUse = m_ThreadCount > (int)numThreadsSupported ? numThreadsSupported : m_ThreadCount;
  else if (m_ThreadCount == 0)
    numThreadsToUse = numThreadsSupported;
  else
  {
    numThreadsToUse = numThreadsSupported + m_ThreadCount;
    if (numThreadsToUse <= 0)
    {
      Warning("The abs of the negeative requested thread count is more than the number of cores available on the system, only using 1 thread");
      numThreadsToUse = 1;
    }
  }

  return numThreadsToUse;
}
