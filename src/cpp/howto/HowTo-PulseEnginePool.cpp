/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

#include <memory>

// Include the various types you will be using in your code
#include "PulseEnginePool.h"

#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/TimingProfile.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating many engines to be run in a thread pool
///
//--------------------------------------------------------------------------------------------------
void HowToPulseEnginePool()
{
  /*std::stringstream ss;
  size_t numEngine = 1;
  TimingProfile profiler;
  double sim_time_s = 60;

  PulseEnginePool pool;
  pool.GetLogger()->LogToConsole(true);
  pool.GetLogger()->SetLogFile("./test_results/HowToPulseEnginePool/Pool.log");

  pool.Info("Creating a pool with " + std::to_string(pool.GetWorkerCount()) + " workers");
  pool.Info("Creating a pool with " + std::to_string(numEngine) + " engines");
  for (int i=0; i<numEngine; i++)
  {
    SEPhysiologyEnginePoolEngine* e = pool.CreateEngine(i, eModelType::HumanAdultWholeBody);
    e->EngineInitialization.SetStateFilename("./states/StandardMale@0s.json");
    e->EngineInitialization.SetLogFilename("./test_results/HowToPulseEndingPool/PoolEngine"+std::to_string(i)+".log");
    // The engine IsActive flag will be set to false on any engine error/fatal messages
    // By default logs and events are not kept
    // You really only want to keep if you are generating SEDataRequested objects via PullDataRequested
    e->EngineInitialization.KeepLogMessages(true);
    e->EngineInitialization.KeepEventChanges(true);
    // Optionally add some data requests to write to file or retrieve (as shown below)
    e->EngineInitialization.GetDataRequestManager().CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
    e->EngineInitialization.GetDataRequestManager().CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
    e->EngineInitialization.GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  }

  // Note you can access the engines to set event and log handlers if you want!
  //pool.GetEngine(0)->Engine->GetLogger()->AddForward(yourLogHandler);
  // You might want to add your own event handler as the DataRequested will track ALL events
  // Including the many StartOfInhal, StartOfExhale, StartOfCardiacCycle...
  //pool.GetEngine(0)->Engine->GetEventManager().ForwardEvents(yourEventHandler);

  // Only retuns false is ALL engines do not initialize
  // A warning will be logged if any engine could not initialize
  if (!pool.InitializeEngines())
  {
    pool.Fatal("No engines were able to initialize");
    return;
  }
  // Check each of the engine IsActive flags to see who initialized

  // There are 2 ways to add actions to engines
  // You can just call Process actions on the pool engine directly
  // The process will occur on this thread
  // ProcessAction just copies the action
  // This is very fast, and fine for all actions execpt SEAdvanceTime
  // SEAdvanceTime actions advance the engine in the ProcessAction call!
  // Since you are using a pool, you want to advance the engine in a thread
  // So add those to the SEPhysiologyEnginePoolEngine Action vector
  // Actions in this vector are processed in a pool thread

  // Add a hemorrhage to one engine on this thread
  SEHemorrhage h;
  h.GetSeverity().SetValue(0.4);
  h.SetCompartment(eHemorrhage_Compartment::VenaCava);
  pool.GetEngine(0)->Engine->ProcessAction(h);
  // Advance some time on an engine, in the thread pool
  SEAdvanceTime adv;
  adv.GetTime().SetValue(15, TimeUnit::s);
  pool.GetEngine(0)->Actions.push_back(&adv);
  pool.ProcessActions();

  // If you added data requests, you can pull them into an array
  std::vector<SEDataRequested*> dataRequested;
  pool.PullDataRequested(dataRequested);
  for (SEDataRequested* dr : dataRequested)
  {
    ss << "Engine " << dr->GetID() << " is " << (dr->IsActive() ? "active" : "not active");
    pool.Info(ss);
    // Index 0 is ALWAYS Current Sim Time in seconds
    pool.Info("Current time of engine is :"+std::to_string(dr->GetValues()[0])+"s");
    // Data is indexed from 1 in the order the data requests were created
    pool.Info("Current Blood Volume(mL) :" + std::to_string(dr->GetValues()[12])+"mL");
    ss << dr->GetLogMessages();
    pool.Info(ss);
    pool.Info("");
  }

  // Once you are done analyzing this vector, clear out the results
  // This will clear out log/events so you don't get the same log/events in the next PullDataRequested
  pool.ClearDataRequested();

  profiler.Start("s");
  pool.AdvanceModelTime(sim_time_s, TimeUnit::s);
  double elapsed_time_s = profiler.GetElapsedTime_s("s");
  pool.Info("It took " + std::to_string(elapsed_time_s) + "s to simulate " + std::to_string(sim_time_s)+"s");

  // You can always access data straight from the engine as well
  pool.Info("Engine 0 Blood Volume(mL) :"+std::to_string(pool.GetEngine(0)->Engine->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL)));
  pool.Info("Engine 1 Blood Volume(mL) :" + std::to_string(pool.GetEngine(1)->Engine->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL)));
*/
}
