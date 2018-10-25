/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioDriver.h"
#include "PulsePhysiologyEngine.h"
#include "controller/Controller.h"
#include "controller/ScenarioExec.h"
#include "utils/FileUtils.h"

bool PulseScenarioDriver::Configure(int argc, char* argv[])
{
  if (argc <= 1)
  {
    std::cerr << "Need scenario file or config file to execute" << std::endl;
    return false;
  }
  m_file = argv[1];

  for (int i = 2; i < argc; ++i)
  {
    std::string argument(argv[i]);
    if (argument.at(0) == '-')
    {
      m_arguments.insert(argument.substr(1));
    }
  }
  return true;
}

void PulseScenarioDriver::Run()
{
  // Set up the log file
  std::string logFile = m_file;
  logFile = Replace(logFile, "verification", "test_results");
  logFile = Replace(logFile, ".pba", ".log");
  // Set up the verification output file  
  std::string dataFile = m_file;
  dataFile = Replace(dataFile, "verification", "test_results");
  dataFile = Replace(dataFile, ".pba", "Results.csv");
  // What are we creating?
  std::cout << "Log File : " << logFile << std::endl;
  std::cout << "Results File : " << dataFile << std::endl;
  // Delete any results file that may be there
  remove(dataFile.c_str());
  std::unique_ptr<PhysiologyEngine> Pulse = CreatePulseEngine(logFile.c_str());
  if (!Pulse)
  {
    std::cerr << "Unable to create PulseEngine" << std::endl;
    return;
  }
  try
  {
    PulseScenarioExec exec(*((PulseEngine*)Pulse.get()));
    exec.Execute(m_file.c_str(), dataFile.c_str());
  }
  catch (CommonDataModelException ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unable to run scenario " << m_file << std::endl;
  }
}

bool PulseScenarioDriver::HasArgument(const std::string& argument)
{
    return m_arguments.find(argument) != end(m_arguments);
}

int main(int argc, char* argv[])
{
  try
  {
    PulseScenarioDriver scenarioDriver;
    if (!scenarioDriver.Configure(argc, argv))
    {
      return 1;
    }
    scenarioDriver.Run();
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
  }

    return 0;
}
