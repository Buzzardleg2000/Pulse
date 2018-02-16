/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEEngineTracker.h"
#include "scenario/SEScenario.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "scenario/SEAdvanceTime.h"
#include "scenario/SEDataRequestManager.h"
#include "properties/SEScalarTime.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating an engine based on a scenario file
///
/// \details
/// Read a scenario file and pull things out for your use case
//--------------------------------------------------------------------------------------------------
void HowToScenarioBase()
{
  // Create our engine
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("HowToScenarioBase.log");
  pe->GetLogger()->Info("HowToScenarioBase");
  
  
  //Let's read the scenario we want to base this engine on
  SEScenario sce(pe->GetSubstanceManager());
  sce.LoadFile("YourScenario.pba");

  if (sce.HasEngineStateFile())
  {
    if (!pe->LoadStateFile(sce.GetEngineStateFile()))
    {
      pe->GetLogger()->Error("Could not load state, check the error");
      return;
    }
  }
  else if (sce.HasInitialParameters())
  {
    SEScenarioInitialParameters& sip = sce.GetInitialParameters();
    if (sip.HasPatientFile())
    {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c);// Copy to const
      if (!pe->InitializeEngine(sip.GetPatientFile(), &conditions))
      {
        pe->GetLogger()->Error("Could not load state, check the error");
        return;
      }
    }
    else if (sip.HasPatient())
    {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c);// Copy to const
      if (!pe->InitializeEngine(sip.GetPatient(), &conditions))
      {
        pe->GetLogger()->Error("Could not load state, check the error");
        return;
      }
    }
  }

  // Make a copy of the data requests, note this clears out data requests from the engine
  // This will clear out the data requests if any exist in the DataRequestManager
  pe->GetEngineTracker()->GetDataRequestManager().SaveFile("YourDataRequestsFile.pba");

  // NOTE : You can just make a DataRequests file that holds only data requests
  // And serialize that in instead of a sceanrio file, if all you want is a consistent
  // This will clear out any requests already in the object
  pe->GetEngineTracker()->GetDataRequestManager().LoadFile("YourDataRequestsFile.pba", pe->GetSubstanceManager());
  // Don't need to delete drData as obj is wrapped in a unique_ptr
 

  if (!pe->GetEngineTracker()->GetDataRequestManager().HasResultsFilename())
    pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./ResultsFileName.txt");

  // Let's request data do be tracked that is in the scenario  
  HowToTracker tracker(*pe);
  SEAdvanceTime* adv;
  // Now run the scenario actions
  for (SEAction* a : sce.GetActions())
  {
    // We want the tracker to process an advance time action so it will write each time step of data to our track file
    adv = dynamic_cast<SEAdvanceTime*>(a);
    if (adv != nullptr)
      tracker.AdvanceModelTime(adv->GetTime(TimeUnit::s));// you could just do pe->AdvanceModelTime without tracking timesteps
    else
      pe->ProcessAction(*a);
  }

  // At this point your engine is where you want it to be
  // You could read in a new scenario and run it's actions 
  // or programatically add actions as your applications sees fit

}