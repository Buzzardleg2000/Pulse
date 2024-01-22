/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

template<CIRCUIT_PATH_TEMPLATE>
SECircuitPath<CIRCUIT_PATH_TYPES>::SECircuitPath(SECircuitNode<CIRCUIT_NODE_TYPES>& src, SECircuitNode<CIRCUIT_NODE_TYPES>& tgt, const std::string& name) : Loggable(src.GetLogger()), m_Name(name), m_SourceNode(src), m_TargetNode(tgt)
{
  m_Resistance = nullptr;
  m_NextResistance = nullptr;
  m_ResistanceBaseline = nullptr;
  m_Capacitance = nullptr;
  m_NextCapacitance = nullptr;
  m_CapacitanceBaseline = nullptr;
  m_Inductance = nullptr;
  m_NextInductance = nullptr;
  m_InductanceBaseline = nullptr;
  m_Flux = nullptr;
  m_NextFlux = nullptr;
  m_FluxSource = nullptr;
  m_NextFluxSource = nullptr;
  m_FluxSourceBaseline = nullptr;
  m_PotentialSource = nullptr;
  m_NextPotentialSource = nullptr;
  m_PotentialSourceBaseline = nullptr;
  m_ValveBreakdownPotential = nullptr;
  m_Switch = eGate::NullGate;
  m_Valve = eGate::NullGate;
  m_NextSwitch = eGate::NullGate;
  m_NextValve = eGate::NullGate;
  m_NextPolarizedState = eGate::NullGate;
  m_PolarizedState = eGate::NullGate;

  m_NumElements = 0;
  m_NumNextElements = 0;

  if (&m_SourceNode == &m_TargetNode)
  {
    std::stringstream ss;
    ss << "Path " << GetName() << " has the same Source and Target Nodes.  They cannot be the same Node.";
    Fatal(ss);
  }
}

template<CIRCUIT_PATH_TEMPLATE>
SECircuitPath<CIRCUIT_PATH_TYPES>::~SECircuitPath()
{
  Clear();
}

template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::Clear()
{
  m_Switch = eGate::NullGate;
  m_Valve = eGate::NullGate;
  m_NextSwitch = eGate::NullGate;
  m_NextValve = eGate::NullGate;
  m_NextPolarizedState = eGate::NullGate;
  m_PolarizedState = eGate::NullGate;
  SAFE_DELETE(m_Resistance);
  SAFE_DELETE(m_NextResistance);
  SAFE_DELETE(m_ResistanceBaseline);
  SAFE_DELETE(m_Capacitance);
  SAFE_DELETE(m_NextCapacitance);
  SAFE_DELETE(m_CapacitanceBaseline);
  SAFE_DELETE(m_Inductance);
  SAFE_DELETE(m_NextInductance);
  SAFE_DELETE(m_InductanceBaseline);
  SAFE_DELETE(m_Flux);
  SAFE_DELETE(m_NextFlux);
  SAFE_DELETE(m_FluxSource);
  SAFE_DELETE(m_NextFluxSource);
  SAFE_DELETE(m_FluxSourceBaseline);
  SAFE_DELETE(m_PotentialSource);
  SAFE_DELETE(m_NextPotentialSource);
  SAFE_DELETE(m_PotentialSourceBaseline);
  SAFE_DELETE(m_ValveBreakdownPotential);
}

template<CIRCUIT_PATH_TEMPLATE>
std::string SECircuitPath<CIRCUIT_PATH_TYPES>::GetName() const
{
  return m_Name;
}

template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::IsPartOfBlackBox() const
{
  return m_IsPartOfBlackBox;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetPartOfBlackBox(bool b)
{
  m_IsPartOfBlackBox = b;
}

template<CIRCUIT_PATH_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>& SECircuitPath<CIRCUIT_PATH_TYPES>::GetSourceNode() const
{
  return m_SourceNode;
}
template<CIRCUIT_PATH_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>& SECircuitPath<CIRCUIT_PATH_TYPES>::GetTargetNode() const
{
  return m_TargetNode;
}

/////////////////////////
// Valves and Switches //
/////////////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValidElements() const
{
  unsigned short numRCI = (HasResistance() ? 1 : 0) +
    (HasCapacitance() ? 1 : 0) +
    (HasInductance() ? 1 : 0);
  unsigned short numNextRCI = (HasNextResistance() ? 1 : 0) +
    (HasNextCapacitance() ? 1 : 0) +
    (HasNextInductance() ? 1 : 0);
  unsigned short numSrcState = (HasSwitch() ? 1 : 0) +
    (HasValve() ? 1 : 0) +
    (HasFluxSource() ? 1 : 0) +
    (HasPotentialSource() ? 1 : 0);
  unsigned short numNextSrcState = (HasNextSwitch() ? 1 : 0) +
    (HasNextValve() ? 1 : 0) +
    (HasNextFluxSource() ? 1 : 0) +
    (HasNextPotentialSource() ? 1 : 0);
  m_NumElements = numRCI + numSrcState;
  m_NumNextElements = numNextRCI + numNextSrcState;
  if (m_NumElements > 1 || m_NumNextElements > 1)
  {
    Error("There is only one element allowed per path.");
    return false;
  }
  if (HasPolarizedState())
  {
    if (numRCI == 0)
    {
      Error("You must have a Resistance, Capacitance or Inductance on the path if you have a polarized state.");
      return false;
    }
    if (numSrcState > 0)
    {
      Error("You cannot have a Switch, Valve, FluxSource or PotentialSource on a path with a polarized state.");
      return false;
    }
  }
  if (HasNextPolarizedState())
  {
    if (numNextRCI == 0)
    {
      Error("You must have a Resistance, Capacitance or Inductance on the path if you have a polarized state.");
      return false;
    }
    if (numNextSrcState > 0)
    {
      Error("You cannot have a Switch, Valve, FluxSource or PotentialSource on a path with a polarized state.");
      return false;
    }
  }
  if (HasValveBreakdownPotential())
  {
    if (!HasValve())
    {
      Error("You cannot have a ValveBreakdownPotential if you do not have a valve.");
      return false;
    }
    if (m_ValveBreakdownPotential->IsNegative())
    {
      Error("A negative Breakdown Potential is not allowed. Consider adding a Valve with reversed polarity.");
      return false;
    }
  }
  return true;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetSwitch() const
{
  return m_Switch;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetSwitch(eGate state)
{
  m_Switch = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipSwitch()
{
  m_Switch = (m_Switch == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasSwitch() const
{
  return m_Switch == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateSwitch()
{
  m_Switch = eGate::NullGate;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextSwitch() const
{
  return m_NextSwitch;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextSwitch(eGate state)
{
  m_NextSwitch = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextSwitch()
{
  m_NextSwitch = (m_NextSwitch == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextSwitch() const
{
  return m_NextSwitch == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextSwitch()
{
  m_NextSwitch = eGate::NullGate;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveSwitch()
{
  m_Switch = eGate::NullGate;
  m_NextSwitch = eGate::NullGate;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetValve() const
{
  return m_Valve;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetValve(eGate state)
{
  m_Valve = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipValve()
{
  m_Valve = (m_Valve == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValve() const
{
  return m_Valve == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateValve()
{
  m_Valve = eGate::NullGate;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextValve() const
{
  return m_NextValve;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextValve(eGate state)
{
  m_NextValve = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextValve()
{
  m_NextValve = (m_NextValve == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextValve() const
{
  return m_NextValve == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextValve()
{
  m_NextValve = eGate::NullGate;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveValve()
{
  m_Valve = eGate::NullGate;
  m_NextValve = eGate::NullGate;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextPolarizedState() const
{
  return m_NextPolarizedState;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextPolarizedState(eGate state)
{
  m_NextPolarizedState = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextPolarizedState()
{
  m_NextPolarizedState = (m_NextPolarizedState == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextPolarizedState() const
{
  return m_NextPolarizedState == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextPolarizedState()
{
  m_NextPolarizedState = eGate::NullGate;
}

template<CIRCUIT_PATH_TEMPLATE>
eGate SECircuitPath<CIRCUIT_PATH_TYPES>::GetPolarizedState() const
{
  return m_PolarizedState;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetPolarizedState(eGate state)
{
  m_PolarizedState = state;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipPolarizedState()
{
  m_PolarizedState = (m_PolarizedState == eGate::Open) ? eGate::Closed : eGate::Open;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPolarizedState() const
{
  return m_PolarizedState == eGate::NullGate ? false : true;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidatePolarizedState()
{
  m_PolarizedState = eGate::NullGate;
}


////////////////
// Resistance //
////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasResistance() const
{
  return m_Resistance == nullptr ? false : m_Resistance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetResistance()
{
  if (m_Resistance == nullptr)
    m_Resistance = new ResistanceScalar();
  return *m_Resistance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextResistance() const
{
  return m_NextResistance == nullptr ? false : m_NextResistance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextResistance()
{
  if (m_NextResistance == nullptr)
    m_NextResistance = new ResistanceScalar();
  return *m_NextResistance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasResistanceBaseline() const
{
  return m_ResistanceBaseline == nullptr ? false : m_ResistanceBaseline->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetResistanceBaseline()
{
  if (m_ResistanceBaseline == nullptr)
    m_ResistanceBaseline = new ResistanceScalar();
  return *m_ResistanceBaseline;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveResistance()
{
  if (m_Resistance != nullptr)
    m_Resistance->ForceInvalidate();
  if (m_NextResistance != nullptr)
    m_NextResistance->ForceInvalidate();
  if (m_ResistanceBaseline != nullptr)
    m_ResistanceBaseline->ForceInvalidate();
}

///////////////////////
// Capacitance Types //
///////////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasCapacitance() const
{
  return m_Capacitance == nullptr ? false : m_Capacitance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetCapacitance()
{
  if (m_Capacitance == nullptr)
    m_Capacitance = new CapacitanceScalar();
  return *m_Capacitance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextCapacitance() const
{
  return m_NextCapacitance == nullptr ? false : m_NextCapacitance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextCapacitance()
{
  if (m_NextCapacitance == nullptr)
    m_NextCapacitance = new CapacitanceScalar();
  return *m_NextCapacitance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasCapacitanceBaseline() const
{
  return m_CapacitanceBaseline == nullptr ? false : m_CapacitanceBaseline->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetCapacitanceBaseline()
{
  if (m_CapacitanceBaseline == nullptr)
    m_CapacitanceBaseline = new CapacitanceScalar();
  return *m_CapacitanceBaseline;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveCapacitance()
{
  if (m_Capacitance != nullptr)
    m_Capacitance->ForceInvalidate();
  if (m_NextCapacitance != nullptr)
    m_NextCapacitance->ForceInvalidate();
  if (m_CapacitanceBaseline != nullptr)
    m_CapacitanceBaseline->ForceInvalidate();
}

//////////////////////
// Inductance Types //
//////////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasInductance() const
{
  return m_Inductance == nullptr ? false : m_Inductance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetInductance()
{
  if (m_Inductance == nullptr)
    m_Inductance = new InductanceScalar();
  return *m_Inductance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextInductance() const
{
  return m_NextInductance == nullptr ? false : m_NextInductance->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextInductance()
{
  if (m_NextInductance == nullptr)
    m_NextInductance = new InductanceScalar();
  return *m_NextInductance;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasInductanceBaseline() const
{
  return m_InductanceBaseline == nullptr ? false : m_InductanceBaseline->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetInductanceBaseline()
{
  if (m_InductanceBaseline == nullptr)
    m_InductanceBaseline = new InductanceScalar();
  return *m_InductanceBaseline;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveInductance()
{
  if (m_Inductance != nullptr)
    m_Inductance->ForceInvalidate();
  if (m_NextInductance != nullptr)
    m_NextInductance->ForceInvalidate();
  if (m_InductanceBaseline != nullptr)
    m_InductanceBaseline->ForceInvalidate();
}

////////////////
// Flux Types //
////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFlux() const
{
  return m_Flux == nullptr ? false : m_Flux->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFlux()
{
  if (m_Flux == nullptr)
    m_Flux = new FluxScalar();
  return *m_Flux;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextFlux() const
{
  return m_NextFlux == nullptr ? false : m_NextFlux->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextFlux()
{
  if (m_NextFlux == nullptr)
    m_NextFlux = new FluxScalar();
  return *m_NextFlux;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFluxSource() const
{
  return m_FluxSource == nullptr ? false : m_FluxSource->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFluxSource()
{
  if (m_FluxSource == nullptr)
    m_FluxSource = new FluxScalar();
  return *m_FluxSource;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextFluxSource() const
{
  return m_NextFluxSource == nullptr ? false : m_NextFluxSource->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextFluxSource()
{
  if (m_NextFluxSource == nullptr)
    m_NextFluxSource = new FluxScalar();
  return *m_NextFluxSource;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFluxSourceBaseline() const
{
  return m_FluxSourceBaseline == nullptr ? false : m_FluxSourceBaseline->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFluxSourceBaseline()
{
  if (m_FluxSourceBaseline == nullptr)
    m_FluxSourceBaseline = new FluxScalar();
  return *m_FluxSourceBaseline;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveFluxSource()
{
  if (m_FluxSource != nullptr)
    m_FluxSource->ForceInvalidate();
  if (m_NextFluxSource != nullptr)
    m_NextFluxSource->ForceInvalidate();
  if (m_FluxSourceBaseline != nullptr)
    m_FluxSourceBaseline->ForceInvalidate();
}

template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemoveImposedFlux()
{
  m_FluxType = eBlackBox_Property_Type::Calculate;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::IsFluxImposed() const
{
  return m_FluxType == eBlackBox_Property_Type::Imposed;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::ImposeFlux(double v, const FluxUnit& unit)
{
  m_FluxType = eBlackBox_Property_Type::Imposed;
  if (m_NextFlux == nullptr)
    m_NextFlux = new FluxScalar();
  m_NextFlux->SetValue(v, unit);
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::ImposeFlux(const FluxScalar& s)
{
  m_FluxType = eBlackBox_Property_Type::Imposed;
  if (m_NextFlux == nullptr)
    m_NextFlux = new FluxScalar();
  m_NextFlux->Set(s);
}

/////////////////////
// Potential Types //
/////////////////////
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPotentialSource() const
{
  return m_PotentialSource == nullptr ? false : m_PotentialSource->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetPotentialSource()
{
  if (m_PotentialSource == nullptr)
    m_PotentialSource = new PotentialScalar();
  return *m_PotentialSource;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextPotentialSource() const
{
  return m_NextPotentialSource == nullptr ? false : m_NextPotentialSource->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextPotentialSource()
{
  if (m_NextPotentialSource == nullptr)
    m_NextPotentialSource = new PotentialScalar();
  return *m_NextPotentialSource;
}
template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPotentialSourceBaseline() const
{
  return m_PotentialSourceBaseline == nullptr ? false : m_PotentialSourceBaseline->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetPotentialSourceBaseline()
{
  if (m_PotentialSourceBaseline == nullptr)
    m_PotentialSourceBaseline = new PotentialScalar();
  return *m_PotentialSourceBaseline;
}
template<CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::RemovePotentialSource()
{
  if (m_PotentialSource != nullptr)
    m_PotentialSource->ForceInvalidate();
  if (m_NextPotentialSource != nullptr)
    m_NextPotentialSource->ForceInvalidate();
  if (m_PotentialSourceBaseline != nullptr)
    m_PotentialSourceBaseline->ForceInvalidate();
}


template<CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValveBreakdownPotential() const
{
  return m_ValveBreakdownPotential == nullptr ? false : m_ValveBreakdownPotential->IsValid();
}
template<CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetValveBreakdownPotential()
{
  if (m_ValveBreakdownPotential == nullptr)
    m_ValveBreakdownPotential = new PotentialScalar();
  return *m_ValveBreakdownPotential;
}

#include "cdm/circuit/fluid/SEFluidCircuitPath.h"
template class SECircuitPath<FLUID_CIRCUIT_PATH>;
#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"
template class SECircuitPath<ELECTRICAL_CIRCUIT_PATH>;
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"
template class SECircuitPath<THERMAL_CIRCUIT_PATH>;
