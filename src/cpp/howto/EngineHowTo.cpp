/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

// The following how-to functions demonstrate various ways of interacting with the Pulse engine
#include "howto/EngineHowTo.h"

#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequest.h"
#include "cdm/properties/SEScalarTime.h"

int main()
{
  // Uncomment a method to execute fuctionality!

  //HowToSandbox();

  //HowToEngineUse();
  //HowToCreateAPatient();
  //HowToSerialize();
  //HowToPulseEnginePool();

  HowToACLS();
  //HowToAirwayObstruction();
  //HowToAnesthesiaMachine();
  //HowToArrythmia();
  //HowToAsthmaAttack();
  //HowToBagValveMask();
  //HowToBolusDrug();
  //HowToBrainInjury();
  //HowToCombatMultitrauma();
  //HowToConsumeNutrients();
  //HowToCOPD();
  //HowToCOVID19Ventilated();
  //HowToCPR();
  //HowToECMO();
  //HowToEnvironmentChange();
  //HowToExercise();
  //HowToHemorrhage();
  //HowToLobarPneumonia();
  //HowToMechanicalVentilation();
  //HowToMechanicalVentilator();
  //HowToPulmonaryFibrosis();
  //HowToRespiratoryMechanics();
  //HowToSmoke();
  //HowToTensionPneumothorax();

  // This one does not really run, is a pure example
  //HowToRunScenario();

  // More complicated examples that do run
  //HowToConcurrentEngines();
  //HowToDynamicHemorrhage();

  //HowToTestSystemCapability();

  //HowToVentilationMechanics();
}

bool AdvanceAndTrackTime(PhysiologyEngine& engine)
{
  if (!engine.AdvanceModelTime())  // Compute 1 time step
    return false;

  // Pull Track will pull data from the engine and append it to the file
  engine.GetEngineTracker()->TrackData(engine.GetSimulationTime(TimeUnit::s));

  return true;
}
bool AdvanceAndTrackTime_s(double time_s, PhysiologyEngine& engine)
{
  double dT_s = engine.GetTimeStep(TimeUnit::s);
  int count = static_cast<int>(time_s / dT_s);
  for (int i = 0; i < count; i++)
  {
    if (!engine.AdvanceModelTime())  // Compute 1 time step
      return false;

    // Pull Track will pull data from the engine and append it to the file
    engine.GetEngineTracker()->TrackData(engine.GetSimulationTime(TimeUnit::s));
  }
  return true;
}
