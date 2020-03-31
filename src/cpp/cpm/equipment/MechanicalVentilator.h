/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
class SEEquipmentActionCollection;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

/**
 * @brief 
 * Generic mechanical ventilator for positive pressure ventilation.
 */
class PULSE_DECL MechanicalVentilator : public SEMechanicalVentilator, public PulseMechanicalVentilator, public PulseSystem
{
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  MechanicalVentilator(PulseController& pc);
  PulseController& m_data;

public:
  virtual ~MechanicalVentilator();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void StateChange();

  void PreProcess();
  void Process();
  void PostProcess();

protected:
  void SetConnection();
  void SetConnection(eMechanicalVentilator_Connection c);
  void InvalidateConnection();

  void CalculateVentilatorPressure();
  void CalculateCyclePhase();

  // Serializable member variables (Set in Initialize and in schema)
  //aaron - serialize
  bool         m_inhaling;
  double       m_currentBreathingCycleTime_s;

  // Stateless member variable (Set in SetUp())
  double                m_dt_s;

  SEGasCompartment*     m_Environment;
  SEGasCompartment*     m_ventilator;
  SEFluidCircuitPath*   m_pEnvironmentToVentilator;
};
