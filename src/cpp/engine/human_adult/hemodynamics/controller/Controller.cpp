/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/hemodynamics/controller/Controller.h"
#include "engine/human_adult/hemodynamics/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/controller/CircuitManager.h"
#include "engine/human_adult/whole_body/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/system/physiology/CardiovascularModel.h"
#include "engine/human_adult/whole_body/system/physiology/DrugModel.h"
#include "engine/human_adult/whole_body/system/physiology/EnergyModel.h"
#include "engine/human_adult/whole_body/system/physiology/NervousModel.h"
#include "engine/human_adult/whole_body/system/physiology/Saturation.h"

#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/patient/SEPatient.h"

namespace pulse { namespace human_adult_hemodynamics
{
  Controller::Controller(Logger* logger) : pulse::human_adult_whole_body::Controller(logger)
  {

  }
 
  // Serialization is not supported
  bool Controller::SerializeFromFile(const std::string& /*filename*/)
  {
    Error("Serialization is not supported in this engine");
    return false;
  }
  bool Controller::SerializeToFile(const std::string& /*filename*/) const
  {
    Error("Serialization is not supported in this engine");
    return false;
  }

  bool Controller::SerializeFromString(const std::string& /*src*/, eSerializationFormat /*m*/)
  {
    Error("Serialization is not supported in this engine");
    return false;
  }
  bool Controller::SerializeToString(std::string& /*output*/, eSerializationFormat /*m*/) const
  {
    Error("Serialization is not supported in this engine");
    return false;
  }

  bool Controller::GetPatientAssessment(SEPatientAssessment& /*assessment*/) const
  {
    Error("No assesments are supported in this engine");
    return false;
  }

  bool Controller::IsAirwayModeSupported(pulse::eAirwayMode)
  {
    // No Airway Modes supported
    return false;
  }

  void Controller::Allocate()
  {
    // Create common objects we will use
    m_Substances = new pulse::SubstanceManager(*this);

    m_InitialPatient = new SEPatient(GetLogger());
    m_CurrentPatient = new SEPatient(GetLogger());

    m_Config = new PulseConfiguration(GetLogger());
    m_Config->Initialize("");//Setup defaults that don't need files on disk

    m_Actions = new SEActionManager(*m_Substances);
    m_Conditions = new SEConditionManager(GetLogger());

    m_CardiovascularModel = new pulse::CardiovascularModel(*this);

    // Not doing anything with these, but CV looks for data on them
    m_EnergyModel = new pulse::EnergyModel(*this);
    m_DrugModel = new pulse::DrugModel(*this);
    m_NervousModel = new pulse::NervousModel(*this);

    m_EventManager = new SEEventManager(GetLogger());

    // Create our derived objects
    m_BlackBoxes = new pulse::BlackBoxManager(*this);
    m_Compartments = new CompartmentManager(*this);
    m_Circuits = new pulse::CircuitManager(*this);

    m_LogForward = new pulse::FatalListner(*m_EventManager, m_CurrentTime);
    m_Logger->AddForward(m_LogForward);

    SetupTracker();
  }

  bool Controller::Stabilize(const SEPatientConfiguration& patient_configuration)
  {
    if (patient_configuration.HasConditions() && !patient_configuration.GetConditions()->IsEmpty())
      Warning("Provided conditions will be ignored, this engine does not need to stabilize");

    return true;// Nothing to do here, we are good to go!
  }

  void Controller::InitializeModels()
  {
    Info("Initializing Models");
    m_CardiovascularModel->Clear();

    m_CardiovascularModel->Initialize();
  }

  void Controller::AtSteadyState(pulse::EngineState state)
  {
    m_State = state;

    m_CardiovascularModel->AtSteadyState();
  }

  void Controller::PreProcess()
  {
    m_CardiovascularModel->PreProcess();
  }

  void Controller::Process()
  {
    m_CardiovascularModel->Process();
  }

  void Controller::PostProcess()
  {
    m_CardiovascularModel->PostProcess();
  }
END_NAMESPACE_EX
