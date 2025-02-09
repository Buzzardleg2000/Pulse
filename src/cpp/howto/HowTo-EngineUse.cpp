/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SEEnergySystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEFunctionVolumeVsTime.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarLength.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward log messages for application specific handling logic
///
/// \details
/// The data model has a logging capability that the engine utilizes to log any info to a log file.
///
/// This logger has different levels at which it can log events:
///
///     Debug   - Extremely low level information, intended for the developer to assess any questions in functionality
///     Info    - A general level of information where the engine wants to inform a user of something
///     Warning - Something was not input properly or an assumption was made in the underlying methodology
///     Error   - A recoverable error has occured and the engine is still able to proceed, but results should be inspected for correctness
///     Fatal   - A unrcoverable error has occured and the engine will stop calculating
///
/// The user can create custom forwarders and add them to the engine's logger so that the forwarding methods are called
/// when events are logged. This allows a calling program to be notified of logging messages in order to handle them as needed.
//--------------------------------------------------------------------------------------------------
class MyLogger : public LoggerForward
{
public:
  void ForwardDebug  (const std::string& msg) override { std::cout << "[DEBUG] " << msg << std::endl; }
  void ForwardInfo   (const std::string& msg) override { std::cout << "[INFO] " << msg << std::endl; }
  void ForwardWarning(const std::string& msg) override { std::cout << "[WARN] " << msg << std::endl; }
  void ForwardError  (const std::string& msg) override { std::cout << "[ERROR] " << msg << std::endl; }
  void ForwardFatal  (const std::string& msg) override { std::cout << "[FATAL] " << msg << std::endl; }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward event activity for application specific handling logic
///
/// \details
/// Various systems/equipment can enter into and out of a particular state, this will let you know when that happens.
///
/// The time provided is the current engine time.
/// The provided methods will be called at the end of the time step when the event was triggered
/// This allows a calling program to be notified of events in order to handle them as needed.
//--------------------------------------------------------------------------------------------------
class MyEventHandler : public SEEventHandler
{
public:
  MyEventHandler() : SEEventHandler() {}
  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr) override {}
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of using a Physiology Engine
///
/// \details
/// Usage of the Logger class
/// Loading a SEPatient from disk
/// Advancing time
/// Retrieving data from a Physiology Engine
//--------------------------------------------------------------------------------------------------
void HowToEngineUse()
{
  // Create an engine object
  // By default, this creates a human adult/whole body physiology engine
  // You can create other engines by specifying a eModelType enum
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();

  // By default, PulseEngine will always output log messages to stdout
  // You can turn that off and on like this
  pe->GetLogger()->LogToConsole(false);
  // If you want this engine to write a log file,provided a log filename
  // By default, no log file will be created (no default name)
  pe->GetLogger()->SetLogFile("./test_results/howto/HowTo_EngineUse.log");
  // You can change this at any time, 
  // Use nullptr or empty string to disable writing to a log file

  // This PulseEngine logger is based on log4cpp (which is based on log4j)
  // PulseEngine logs to several distinct, ordered
  // category levels: DEBUG, INFO, WARN, ERROR, FATAL
  // These categories are orders, if your level is set to DEBUG you will recieve ALL messages.
  // If set to INFO, you will not recieve DEBUG, but everything else
  // If set to WARN, you will not recieve DEBUG and INFO, but everything else
  // You can specify which level you would like the engine to log
  // By Default the LogLevel is INFO
  pe->GetLogger()->SetLogLevel(Logger::Level::Info);

  // You can tell the PulseEngine to also direct any output
  // to your own function. For example if you want to capture and process messages
  // and handle them in your own way, Give PulseEngine an instance of the LoggerForward class
  // and PulseEngine will log and also call the provided method with the message.
  // You can specify a set of functions to be called for any one of the log levels
  MyLogger myLogger;
  pe->GetLogger()->AddForward(&myLogger);
  pe->GetLogger()->Info("HowTo_EngineUse");

  // You can tell the PulseEngine to also notify you of any events as well
  // You can Poll objects for events, for example
  pe->GetEventManager().IsEventActive(eEvent::BrainOxygenDeficit);

  // NOTE, setting the LogLevel, Forwarder, EventHandler can be done after initialize and as many times as you want

  // The first order of business is to initialize the engine by loading a patient state.
  // Patient states provided in the SDK are the state of the engine at the time they stabilize
  // More details on creating a patient and stabilizing the engine can be found in HowTo-CreateAPatient.cpp
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  // You can specify a specific simulation time for the engine to use as its initial simulation time
  // If no time is provided, the simulation time that is in the state file will be used
  // Note the provided state files are named to include what is simulation time is
  SEScalarTime startTime;
  startTime.SetValue(0, TimeUnit::s);
  pe->SetSimulationTime(startTime);
  // See below on how to save a state

  // There are specific events that can occur while the engine runs and you submit various actions
  // You can either poll/query the patient object to see if it is in a specific state
  pe->GetEventManager().IsEventActive(eEvent::CardiacArrest);
  // You can also derive a callback class that will be called whenever an Event is entered or exited by the engine
  MyEventHandler myEventHandler;
  pe->GetEventManager().ForwardEvents(&myEventHandler);

  // We are interested in 2 substances and their amounts in compartments
  // Let's grab the substance via name from the substance manager, string look ups are slow
  // So it's a good idea to cache this pointer so you can reuse it when asking at various time steps
  // You can find all substances defined in json files in the substances directory of your working directory
  // Names are in those json files. ALL substance json files are loaded into a substance managers when the engine is created
  const SESubstance* O2 = pe->GetSubstanceManager().GetSubstance("Oxygen");
  const SESubstance* CO2 = pe->GetSubstanceManager().GetSubstance("CarbonDioxide");

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");  
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Oxygen", "PartialPressure");
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "CarbonDioxide", "PartialPressure");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Lungs, "Volume");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Carina, "InFlow");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/howto/HowToEngineUse.csv");

  // We are ready to execute the engine
  // simply tell the engine how long you would like it to execute
  if (!AdvanceAndTrackTime_s(5, *pe))// Note this tracker class takes in seconds
  {
    pe->GetLogger()->Fatal("Unable to advance engine time");
    return;
  }
  // If not using the tracker call
  // pe->AdvanceModelTime(5,TimeUnit::s); // Test for true
  // If you do not provide a time, the engine will execute 1 time step, the Pulse timestep can be retrieved with:
  //pe->GetTimeStep(TimeUnit::s);

  // You will notice that the timing calls mentioned above take in a specific unit
  // The Common Data Model has a conversion library internal to it that will convert data to whatever you specify.
  // There are predefined units defined in each unit quantity scalar class header.
  // Note the SE simpley stands for Synthetic Enviroment.
  // Each unit quantity has it's own class representation along with commonly used unit definitions
  // You may use strings to define your unit if you do not find a unit predefined (but strings have a runtime cost to them)
  // Units are defined in the standard abbreviation, for example:
  // http://www.bipm.org/en/si/
  // Along with si units, english units and medical units are supported
  // You can check to see if a unit is valid by calling the IsValidUnit method on a particular quantity scalar class.
  
  // Engine Data is available in 3 general forms
  // Assessments - Data formed at a level of a clinicians report. 
  //               This is high level data, such as a mean or generalized data 
  //               value intended to give a general overview of the state of the system.
  // Systems - System specific data properties that provide more specific information for a specific system
  //           A system can be a physiological system (cardiovascular, respiratory, etc.). 
  //           It could also be a piece of equipment (anesthesia machine, ECG, etc.)
  // Compartments - Compartments are a more detailed view into the components that make up a system (Anatomy or Equipment)
  //                compartments reflect the low level data such as Volume, Pressure, Flow, and Substance Quantities (Mass/Concentration Volume/VolumeFraction)
  //                Compartments are contained in a hierarchical object model that  encapsulates the body and its anatomy down to organs and organ components
  //                For example the lung compartment is made up of the left lung and right lung

  // When retrieving one of these types of data structures, the structure will contain various properties of a SEScalar types, std::string and enums.
  // (ex. HeartRate, Volume, Flow, ResperatoryRate, etc.)
  // The general naming convention used in creating these structures and their properties is a 'Get', 'Has' style
  // For Scalar objects, these structures only instantiate scalar objects for data they have, in order to maintain a small memory footprint.
  // Calling the Get method will create A new scalar (managed by the structure) and the value will be set to NaN. 
  // ex. pe->GetCardiovascularSystem()->GetBloodVolume()->GetValue(VolumeUnit::L);
  // You can check to see if an engine has output a particular property by calling a complimentary 'Has' method
  // ex. pe->GetCardiovascularSystem()->HasBloodVolume(), if the value is not NaN, true will be returned
  //
  // When you set a value to a scalar, you MUST provide a unit!
  // When you which to get the value of a scalar, you MUST provide a unit! 
  // If the scalar is of a unitless quantity (i.e. fractions) you can use SEScalar::unitless or SEScalar::nullUnit
  // Scalars hold the data in the unit provided, conversions are only performed if the requested unit is not the same as the Scalars set unit
  // Conversions are fast, but it is recommended to understand the units used in the engine and try to use those for optimal use of the engine
  // 
  // SEScalar has overloaded the << operator and will out put in the form of 'value(unit)' (ex. '50(mL)')
  // If the scalar is unitless no parens will be provided.

  // Here we demostrate pulling data from various physiology systems
  
  pe->GetLogger()->Info(std::stringstream() << "HeartRate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "BloodVolume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "ArterialSystolicPressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "ArterialDiastolicPressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "MeanArterialPressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);

  // Here we demonstrate pulling data from a compartment
  // NOTE THAT WHEN YOU PULL A COMPARTMENT FROM THE COMPARTMENT MANAGER, THAT IS A STRING LOOK UP, TRY TO CACHE COMPARTMENTS OF INTEREST INTO YOUR OWN LIST/MAP
  // This allows a more direct access to the underlying data calculated by the methodology 
  // For example, getting an Invasive Blood Pressure, any arterial compartment pressure can be pulled, i.e. femoral artery (right/left leg)
  // Since this is an arterial compartment, blood is flowing through this compartment, where as the carina compartment is air flow
  pe->GetLogger()->Info(std::stringstream() << "Invasive Blood Pressure : " << pe->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  
  pe->GetLogger()->Info(std::stringstream() << "RespirationRate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "End Tidal CarbonDioxide Fraction : "  << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxideFraction());

  // Using the compartment we can get more detailed respiratory information
  // Here we will test the flow of air in the carina compartment. A positive flow is an inhale and a negative flow is an exhale.
  // We can get the amount of CO2 exhaled and O2 inhaled by looking at the volume fraction of the carina of a particular substance
    
  const SEGasCompartment* carina = pe->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
  if (carina->GetInFlow(VolumePerTimeUnit::L_Per_s)>0)
  {// We are inhaling, so let's grab the amount of O2 coming into the body
    pe->GetLogger()->Info(std::stringstream() << "O2 Inhaled " << carina->GetSubstanceQuantity(*O2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  }
  else
  {// We are exhaling, so let's grab the amount of CO2 that is leaving the body
    pe->GetLogger()->Info(std::stringstream() << "CO2 Exhaled " << carina->GetSubstanceQuantity(*CO2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  }

  pe->GetLogger()->Info(std::stringstream() << "OxygenSaturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood pH : " << pe->GetBloodChemistrySystem()->GetBloodPH());
  //  You should save off the SESubstanceQuantity* if you will need it more than once
  pe->GetLogger()->Info(std::stringstream() << "Lactate Concentration : " << pe->GetSubstanceManager().GetSubstance("Lactate")->GetBloodConcentration(MassPerVolumeUnit::mg_Per_dL) << MassPerVolumeUnit::mg_Per_dL);
  pe->GetLogger()->Info(std::stringstream() << "Core Body Temperature : " << pe->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);


  // Save the state of the engine
  pe->SerializeToFile("./test_results/howto/HowToEngineUse-FinalState.json");

  pe->GetLogger()->Info("Finished");
}