/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "scenario/SEScenario.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "scenario/SEScenarioExec.h"
#include "scenario/SEDataRequestManager.h"
#include "scenario/SEAdvanceTime.h"
#include "compartment/SECompartmentManager.h"
#include "engine/SEAdvanceHandler.h"

#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"


//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to handle any specific logic you may want to do each time step
///
/// \details
/// This method will be called at the end of EACH time step of the engine
/// The SEScenarioExecutor will process the advance time actions in a scenario and 
/// step the engine, calling this method each time step
//--------------------------------------------------------------------------------------------------
class MyCustomExec : public SEAdvanceHandler
{
public:
  MyCustomExec() : SEAdvanceHandler(false) { } // Nix callbacks during stabilization
  void OnAdvance(double time_s, const PhysiologyEngine& engine)
  {
    // you are given the current scenairo time and the engine, so you can do what ever you want
  }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of creating and running a scenario
///
/// \details
//--------------------------------------------------------------------------------------------------
void HowToRunScenario()
{
  // Create an engine object
  // PulseEngines will always output log messages to stdout and a log file  
  // If you want this engine to write a log file, include the name 
  // of the log file. If nullptr is given, the engine will only output to the console
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("HowToRunScenario.log");
  pe->GetLogger()->Info("HowToRunScenario");

  // Let's do something everytime the engine advances
  pe->SetAdvanceHandler(new MyCustomExec());
  
  // This PulseEngine logger is based on log4cpp (which is based on log4j)
  // PulseEngine logs to several distinct, ordered
  // category levels: DEBUG, INFO, WARN, ERROR, FATAL
  // These categories are orders, if your level is set to DEBUG you will recieve ALL messages.
  // If set to INFO, you will not recieve DEBUG, but everything else
  // If set to WARN, you will not recieve DEBUG and INFO, but everything else
  // You can specify which level you would like the engine to log
  pe->GetLogger()->SetLogLevel(Logger::level::Info);

  // You can forward logs as demonstrated in HowTo-EngineUse

  // Create a Scenario Executor
  SEScenarioExec executor(*pe);
  // Let's make a scenario (you could just point the executor to a scenario pba file on disk as well)
  SEScenario sce(pe->GetSubstanceManager());
  sce.SetName("HowToRunScenario");
  sce.SetDescription("Simple Scenario to demonstraight building a scenario by the CDM API");
  sce.GetInitialParameters().SetPatientFile("StandardMale.pba");
  // Note you can set an Engine state, or create your own SEPatient object (see HowTo-CreateAPatient)
  // When filling out a data request, units are optional
  // The units will be set to whatever units the engine uses.
  // NOTE: The scenario makes it's own copy of these requests
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  sce.GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate",FrequencyUnit::Per_min);
  sce.GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  sce.GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  // you can specify where the file goes 
  sce.GetDataRequestManager().SetResultsFilename("./HowTo-RunScenarioResults.csv");
  // If you don't set the file name it will try to make a place for the results in a bin/Scenarios/ folder
  // Let's just run for 2 minutes
  // NOTE: the scenario will make it's own copy of this action
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  SEAdvanceTime adv;
  adv.GetTime().SetValue(2, TimeUnit::min);
  sce.AddAction(adv);

  executor.Execute(sce, "./HowTo-RunScenarioResults.csv");
}
