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
#include "cdm/utils/TimingProfile.h"


void PulseScenarioExec::Clear()
{
  SEScenarioExec::Clear();
  m_ModelType = eModelType::HumanAdultWholeBody;
  SAFE_DELETE_VECTOR(m_Statuses);
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
    SEScenarioExecStatus status;
    PulseScenario sce(GetLogger(), GetDataRootDirectory());
    SplitPathFilename(GetScenarioFilename(), scenarioPath, scenarioFilename);
    GetDataRequestFilesSearch().insert(scenarioPath);
    if (!sce.SerializeFromFile(GetScenarioFilename()))
      return false;
    return Execute(sce, &status);
  }
  else if (!GetScenarioDirectory().empty() || !GetScenarioExecListFilename().empty())
  {
    TimingProfile profiler;
    profiler.Start("Total");

    m_Statuses.clear();
    if (!GetScenarioExecListFilename().empty())
    {
      if (!SEScenarioExecStatus::SerializeFromFile(GetScenarioExecListFilename(), m_Statuses, GetLogger()))
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
        SEScenarioExecStatus* status = new SEScenarioExecStatus();
        status->SetScenarioFilename(filename);
        m_Statuses.push_back(status);
      }
      m_CompletedStatusesFilename = "./ScenarioDirectoryStatuses.json";
    }
    size_t numThreadsToUse = ComputeNumThreads();
    if (numThreadsToUse <= 0)
    {
      Fatal("Unable to compute the number of threads to use");
      return false;
    }

    for (size_t p = 0; p < numThreadsToUse; p++)
      m_Threads.push_back(std::thread(&PulseScenarioExec::ControllerLoop, this));
    for (size_t p = 0; p < numThreadsToUse; p++)
      m_Threads[p].join();

    Info("It took " + pulse::cdm::to_string(profiler.GetElapsedTime_s("Total")) + "s to run these scenarios");
    profiler.Clear();
    return true;
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
      PulseScenarioExec* pse = new PulseScenarioExec(GetLogger());
      opts.emplace_back(pse);
      pse->Copy(*this);
      pse->SetScenarioLogFilename(filename);
      futures.emplace_back(pool.enqueue([&pse]() -> bool { return pse->ConvertLog(); }));
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

void PulseScenarioExec::ControllerLoop()
{
  std::pair <PulseScenarioExec*, SEScenarioExecStatus*> ss;
  while (true)
  {
    SEScenarioExecStatus* status = GetNextScenarioStatus();
    if (!status)
      break;
    PulseScenario sce(GetDataRootDirectory());
    if (sce.SerializeFromFile(status->GetScenarioFilename()))
      Execute(sce, status);
    else
    {
      status->SetFatalRuntimeError(true);
      Error("Unable to serialize scenario file: " + ss.first->GetScenarioFilename());
    }
    FinalizeExecutionStatus(*status);
  }
}
SEScenarioExecStatus* PulseScenarioExec::GetNextScenarioStatus()
{
  m_Mutex.lock();
  SEScenarioExecStatus* found = nullptr;
  for (SEScenarioExecStatus* status : m_Statuses)
  {
    if (status->GetScenarioExecutionState() == eScenarioExecutionState::Waiting)
    {
      found = status;
      found->SetScenarioExecutionState(eScenarioExecutionState::Executing);
      break;
    }
  }
  m_Mutex.unlock();
  return found;
}
void PulseScenarioExec::FinalizeExecutionStatus(SEScenarioExecStatus& status)
{
  m_Mutex.lock();
  status.SetScenarioExecutionState(eScenarioExecutionState::Complete);
  m_Completed.push_back(&status);
  Info("Completed "+status.GetScenarioFilename());
  SEScenarioExecStatus::SerializeToFile(m_Completed, m_CompletedStatusesFilename, GetLogger());
  m_Mutex.unlock();
}
