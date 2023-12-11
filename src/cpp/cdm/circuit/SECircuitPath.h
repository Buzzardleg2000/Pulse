/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitNode.h"
#include "cdm/properties/SEScalarElectricCapacitance.h"
#include "cdm/properties/SEScalarElectricCurrent.h"
#include "cdm/properties/SEScalarElectricInductance.h"
#include "cdm/properties/SEScalarElectricResistance.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarHeatInductance.h"
#include "cdm/properties/SEScalarHeatResistance.h"

#define CIRCUIT_PATH_TEMPLATE typename FluxScalar, typename ResistanceScalar, typename CapacitanceScalar, typename InductanceScalar, typename PotentialScalar, typename QuantityScalar, \
                              typename FluxUnit, typename ResistanceUnit, typename CapacitanceUnit, typename InductanceUnit, typename PotentialUnit, typename QuantityUnit

#define CIRCUIT_PATH_TYPES FluxScalar, ResistanceScalar, CapacitanceScalar, InductanceScalar, PotentialScalar, QuantityScalar, \
                           FluxUnit, ResistanceUnit, CapacitanceUnit, InductanceUnit, PotentialUnit, QuantityUnit

#define ELECTRICAL_CIRCUIT_PATH SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge, \
                                ElectricCurrentUnit, ElectricResistanceUnit, ElectricCapacitanceUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit 

#define FLUID_CIRCUIT_PATH SEScalarVolumePerTime, SEScalarPressureTimePerVolume, SEScalarVolumePerPressure, SEScalarPressureTimeSquaredPerVolume, SEScalarPressure, SEScalarVolume, \
                           VolumePerTimeUnit, PressureTimePerVolumeUnit, VolumePerPressureUnit, PressureTimeSquaredPerVolumeUnit, PressureUnit, VolumeUnit

#define THERMAL_CIRCUIT_PATH SEScalarPower, SEScalarHeatResistance, SEScalarHeatCapacitance, SEScalarHeatInductance, SEScalarTemperature, SEScalarEnergy, \
                             PowerUnit, HeatResistanceUnit, HeatCapacitanceUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit


template<CIRCUIT_PATH_TEMPLATE>
class SECircuitPath : public Loggable
{
  friend class PBCircuit;//friend the serialization class
  template<typename NodeType, typename PathType> friend class SECircuit;
protected:
  SECircuitPath(SECircuitNode<CIRCUIT_NODE_TYPES>& src, SECircuitNode<CIRCUIT_NODE_TYPES>& tgt, const std::string& name);
public:
  virtual ~SECircuitPath();

  virtual void Clear();

  virtual std::string GetName() const;

  virtual bool IsPartOfBlackBox() const;
  virtual void SetPartOfBlackBox(bool b);

  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetSourceNode() const;
  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetTargetNode() const;

  virtual bool HasFlux() const;
  virtual FluxScalar& GetFlux();
  virtual bool HasNextFlux() const;
  virtual FluxScalar& GetNextFlux();

  virtual bool HasFluxSource() const;
  virtual FluxScalar& GetFluxSource();
  virtual bool HasNextFluxSource() const;
  virtual FluxScalar& GetNextFluxSource();
  virtual bool HasFluxSourceBaseline() const;
  virtual FluxScalar& GetFluxSourceBaseline();
  virtual void RemoveFluxSource();

  virtual void RemoveImposedFlux();
  virtual bool IsFluxImposed() const;
  virtual void ImposeFlux(const FluxScalar& s);
  virtual void ImposeFlux(double v, const FluxUnit& unit);

  virtual bool HasResistance() const;
  virtual ResistanceScalar& GetResistance();
  virtual bool HasNextResistance() const;
  virtual ResistanceScalar& GetNextResistance();
  virtual bool HasResistanceBaseline() const;
  virtual ResistanceScalar& GetResistanceBaseline();
  virtual void RemoveResistance();

  virtual bool HasCapacitance() const;
  virtual CapacitanceScalar& GetCapacitance();
  virtual bool HasNextCapacitance() const;
  virtual CapacitanceScalar& GetNextCapacitance();
  virtual bool HasCapacitanceBaseline() const;
  virtual CapacitanceScalar& GetCapacitanceBaseline();
  virtual void RemoveCapacitance();

  virtual bool HasInductance() const;
  virtual InductanceScalar& GetInductance();
  virtual bool HasNextInductance() const;
  virtual InductanceScalar& GetNextInductance();
  virtual bool HasInductanceBaseline() const;
  virtual InductanceScalar& GetInductanceBaseline();
  virtual void RemoveInductance();

  virtual bool HasPotentialSource() const;
  virtual PotentialScalar& GetPotentialSource();
  virtual bool HasNextPotentialSource() const;
  virtual PotentialScalar& GetNextPotentialSource();
  virtual bool HasPotentialSourceBaseline() const;
  virtual PotentialScalar& GetPotentialSourceBaseline();
  virtual void RemovePotentialSource();

  virtual bool HasValveBreakdownPotential() const;
  virtual PotentialScalar& GetValveBreakdownPotential();

  virtual eGate GetSwitch() const;
  virtual void SetSwitch(eGate state);
  virtual void FlipSwitch();
  virtual bool HasSwitch() const;
  virtual void InvalidateSwitch();
  virtual eGate GetNextSwitch() const;
  virtual void SetNextSwitch(eGate state);
  virtual void FlipNextSwitch();
  virtual bool HasNextSwitch() const;
  virtual void InvalidateNextSwitch();
  virtual void RemoveSwitch();

  virtual eGate GetValve() const;
  virtual void SetValve(eGate state);
  virtual void FlipValve();
  virtual bool HasValve() const;
  virtual void InvalidateValve();
  virtual eGate GetNextValve() const;
  virtual void SetNextValve(eGate state);
  virtual void FlipNextValve();
  virtual bool HasNextValve() const;
  virtual void InvalidateNextValve();
  virtual void RemoveValve();

  virtual eGate GetPolarizedState() const;
  virtual void SetPolarizedState(eGate state);
  virtual void FlipPolarizedState();
  virtual bool HasPolarizedState() const;
  virtual void InvalidatePolarizedState();
  virtual eGate GetNextPolarizedState() const;
  virtual void SetNextPolarizedState(eGate state);
  virtual void FlipNextPolarizedState();
  virtual bool HasNextPolarizedState() const;
  virtual void InvalidateNextPolarizedState();

  virtual bool HasValidElements() const;
  virtual unsigned short NumberOfElements() const { return m_NumElements; }
  virtual unsigned short NumberOfNextElements() const { return m_NumNextElements; }

protected:
  std::string                         m_Name;
  SECircuitNode<CIRCUIT_NODE_TYPES>&  m_SourceNode;
  SECircuitNode<CIRCUIT_NODE_TYPES>&  m_TargetNode;
  mutable unsigned short              m_NumElements;
  mutable unsigned short              m_NumNextElements;
  bool                                m_IsPartOfBlackBox = false;
  /////////////////////////
  // Valves and Switches //
  /////////////////////////
  eGate                        m_Switch;
  eGate                        m_NextSwitch;
  eGate                        m_Valve;
  eGate                        m_NextValve;
  eGate                        m_PolarizedState;
  eGate                        m_NextPolarizedState;

  ResistanceScalar*            m_Resistance;
  ResistanceScalar*            m_NextResistance;
  ResistanceScalar*            m_ResistanceBaseline;
                 
  CapacitanceScalar*           m_Capacitance;
  CapacitanceScalar*           m_NextCapacitance;
  CapacitanceScalar*           m_CapacitanceBaseline;

  InductanceScalar*            m_Inductance;
  InductanceScalar*            m_NextInductance;
  InductanceScalar*            m_InductanceBaseline;

  FluxScalar*                  m_Flux;
  FluxScalar*                  m_NextFlux;
  FluxScalar*                  m_FluxSource;
  FluxScalar*                  m_NextFluxSource;
  FluxScalar*                  m_FluxSourceBaseline;
  eBlackBox_Property_Type      m_FluxType = eBlackBox_Property_Type::Calculate;
                   
  PotentialScalar*             m_PotentialSource;
  PotentialScalar*             m_NextPotentialSource;
  PotentialScalar*             m_PotentialSourceBaseline;
  PotentialScalar*             m_ValveBreakdownPotential;
};



#include "cdm/circuit/SECircuitPath.hxx"
