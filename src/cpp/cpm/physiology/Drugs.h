/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SEDrugSystem.h"
class SEFluidCircuitPath;
class SELiquidCompartment;
class SETissueCompartment;

/**
 * @brief
 * The drug system contains the physiologically based pharmacokinetic (PBPK) model and the pharmacodynamic (PD) model.
 * @details 
 * Drug transvascular transport is modeled with a physiologically-based pharmacokinetic (PBPK) model,
 * and the physiologic effects on the body are modeled with a low-fidelity pharmacodynamic (PD) model.
 */  
class PULSE_DECL Drugs : public PulseDrugSystem
{
  friend class PulseData;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
protected:
  Drugs(PulseData& data);
  PulseData& m_data;

public:
  virtual ~Drugs();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess(){}

protected:
  void ComputeExposedModelParameters() override;

  void AdministerSubstanceBolus();
  void AdministerSubstanceInfusion();
  void AdministerSubstanceCompoundInfusion();

  void CalculatePartitionCoefficients();
  void CalculateSubstanceClearance();
  void CalculatePlasmaSubstanceConcentration();
  void CalculateDrugEffects();

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  SELiquidCompartment*  m_aortaVascular;
  SELiquidCompartment*  m_venaCavaVascular;
  SELiquidCompartment*  m_muscleIntracellular;
  SELiquidCompartment*  m_liverVascular;
  SETissueCompartment*  m_liverTissue;
  SEFluidCircuitPath*   m_IVToVenaCava;
  SETissueCompartment*  m_fatTissue;
};