/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDefs.h"
#include "cdm/circuit/SECircuitManager.h"

#include "cdm/circuit/fluid/SEFluidCircuit.h"
#include "cdm/circuit/fluid/SEFluidCircuitNode.h"
#include "cdm/circuit/fluid/SEFluidCircuitPath.h"

#include "cdm/circuit/thermal/SEThermalCircuit.h"
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"

#include "cdm/circuit/electrical/SEElectricalCircuit.h"
#include "cdm/circuit/electrical/SEElectricalCircuitNode.h"
#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"


SECircuitManager::SECircuitManager(Logger* logger) : Loggable(logger)
{

}
SECircuitManager::~SECircuitManager()
{
  Clear();
}

void SECircuitManager::Clear()
{
  m_ElectricalLedger.Clear();
  m_FluidLedger.Clear();
  m_ThermalLedger.Clear();
}


void SECircuitManager::SetReadOnly(bool b)
{
  SetReadOnlyElectrical(b);
  SetReadOnlyFluid(b);
  SetReadOnlyThermal(b); 
}

void SECircuitManager::SetReadOnlyElectrical(bool b)
{
  for (auto nItr : m_ElectricalLedger.nodes)
  {
    if(nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if(nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);
    
    if(nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if(nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_ElectricalLedger.paths)
  {
    if (pItr.second->HasCapacitanceBaseline())
    {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}

void SECircuitManager::SetReadOnlyFluid(bool b)
{
  for (auto nItr : m_FluidLedger.nodes)
  {
    if (nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if (nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);

    if (nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if (nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_FluidLedger.paths)
  {
    if (pItr.second->HasCapacitanceBaseline())
    {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}

void SECircuitManager::SetReadOnlyThermal(bool b)
{
  for (auto nItr : m_ThermalLedger.nodes)
  {
    if (nItr.second->HasQuantityBaseline())
      nItr.second->GetQuantityBaseline().SetReadOnly(b);
    if (nItr.second->HasQuantity())
      nItr.second->GetQuantity().SetReadOnly(b);

    if (nItr.second->HasPotential())
      nItr.second->GetPotential().SetReadOnly(b);
    if (nItr.second->HasNextPotential())
      nItr.second->GetNextPotential().SetReadOnly(b);
  }
  for (auto pItr : m_ThermalLedger.paths)
  {
    if (pItr.second->HasCapacitanceBaseline())
    {
      if (pItr.second->GetSourceNode().HasNextQuantity())
        pItr.second->GetSourceNode().GetNextQuantity().SetReadOnly(b);
      if (pItr.second->GetTargetNode().HasNextQuantity())
        pItr.second->GetTargetNode().GetNextQuantity().SetReadOnly(b);
    }
    if (pItr.second->HasFlux())
      pItr.second->GetFlux().SetReadOnly(b);
    if (pItr.second->HasNextFlux())
      pItr.second->GetNextFlux().SetReadOnly(b);
    // Lock all the current values
    if (pItr.second->HasFluxSource())
      pItr.second->GetFluxSource().SetReadOnly(b);
    if (pItr.second->HasResistance())
      pItr.second->GetResistance().SetReadOnly(b);
    if (pItr.second->HasCapacitance())
      pItr.second->GetCapacitance().SetReadOnly(b);
    if (pItr.second->HasInductance())
      pItr.second->GetInductance().SetReadOnly(b);
    if (pItr.second->HasPotentialSource())
      pItr.second->GetPotentialSource().SetReadOnly(b);
  }
}

SEElectricalCircuitNode&       SECircuitManager::CreateElectricalNode(const std::string& name) { return CreateNode<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
void                           SECircuitManager::DeleteElectricalNode(const std::string& name) { DeleteNode<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
bool                           SECircuitManager::HasElectricalNode(const std::string& name) const { return HasNode<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
SEElectricalCircuitNode*       SECircuitManager::GetElectricalNode(const std::string& name) { return GetNode<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
const SEElectricalCircuitNode* SECircuitManager::GetElectricalNode(const std::string& name) const { return GetNode<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
SEElectricalCircuitPath&       SECircuitManager::CreateElectricalPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name) { return CreatePath<ELECTRICAL_LEDGER_TYPES>(src, tgt, name, m_ElectricalLedger); }
void                           SECircuitManager::DeleteElectricalPath(const std::string& name) { DeletePath<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
bool                           SECircuitManager::HasElectricalPath(const std::string& name) const { return HasPath<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
SEElectricalCircuitPath*       SECircuitManager::GetElectricalPath(const std::string& name) { return GetPath<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
const SEElectricalCircuitPath* SECircuitManager::GetElectricalPath(const std::string& name) const { return GetPath<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
SEElectricalCircuit&           SECircuitManager::CreateElectricalCircuit(const std::string& name) { return CreateCircuit<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
void                           SECircuitManager::DeleteElectricalCircuit(const std::string& name) { DeleteCircuit<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
bool                           SECircuitManager::HasElectricalCircuit(const std::string& name) const { return HasCircuit<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
SEElectricalCircuit*           SECircuitManager::GetElectricalCircuit(const std::string& name) { return GetCircuit<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }
const SEElectricalCircuit*     SECircuitManager::GetElectricalCircuit(const std::string& name) const { return GetCircuit<ELECTRICAL_LEDGER_TYPES>(name, m_ElectricalLedger); }

SEFluidCircuitNode&            SECircuitManager::CreateFluidNode(const std::string& name) { return CreateNode<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
void                           SECircuitManager::DeleteFluidNode(const std::string& name) { DeleteNode<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
bool                           SECircuitManager::HasFluidNode(const std::string& name) const { return HasNode<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
SEFluidCircuitNode*            SECircuitManager::GetFluidNode(const std::string& name) { return GetNode<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
const SEFluidCircuitNode*      SECircuitManager::GetFluidNode(const std::string& name) const { return GetNode<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
SEFluidCircuitPath&            SECircuitManager::CreateFluidPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name) { return CreatePath<FLUID_LEDGER_TYPES>(src, tgt, name, m_FluidLedger); }
void                           SECircuitManager::DeleteFluidPath(const std::string& name) { DeletePath<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
bool                           SECircuitManager::HasFluidPath(const std::string& name) const { return HasPath<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
SEFluidCircuitPath*            SECircuitManager::GetFluidPath(const std::string& name) { return GetPath<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
const SEFluidCircuitPath*      SECircuitManager::GetFluidPath(const std::string& name) const { return GetPath<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
SEFluidCircuit&                SECircuitManager::CreateFluidCircuit(const std::string& name) { return CreateCircuit<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
void                           SECircuitManager::DeleteFluidCircuit(const std::string& name) { DeleteCircuit<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
bool                           SECircuitManager::HasFluidCircuit(const std::string& name) const { return HasCircuit<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
SEFluidCircuit*                SECircuitManager::GetFluidCircuit(const std::string& name) { return GetCircuit<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }
const SEFluidCircuit*          SECircuitManager::GetFluidCircuit(const std::string& name) const { return GetCircuit<FLUID_LEDGER_TYPES>(name, m_FluidLedger); }

SEThermalCircuitNode&          SECircuitManager::CreateThermalNode(const std::string& name) { return CreateNode<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
void                           SECircuitManager::DeleteThermalNode(const std::string& name) { DeleteNode<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
bool                           SECircuitManager::HasThermalNode(const std::string& name) const { return HasNode<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
SEThermalCircuitNode*          SECircuitManager::GetThermalNode(const std::string& name) { return GetNode<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
const SEThermalCircuitNode*    SECircuitManager::GetThermalNode(const std::string& name) const { return GetNode<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
SEThermalCircuitPath&          SECircuitManager::CreateThermalPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name) { return CreatePath<THERMAL_LEDGER_TYPES>(src, tgt, name, m_ThermalLedger); }
void                           SECircuitManager::DeleteThermalPath(const std::string& name) { DeletePath<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
bool                           SECircuitManager::HasThermalPath(const std::string& name) const { return HasPath<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
SEThermalCircuitPath*          SECircuitManager::GetThermalPath(const std::string& name) { return GetPath<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
const SEThermalCircuitPath*    SECircuitManager::GetThermalPath(const std::string& name) const { return GetPath<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
SEThermalCircuit&              SECircuitManager::CreateThermalCircuit(const std::string& name) { return CreateCircuit<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
void                           SECircuitManager::DeleteThermalCircuit(const std::string& name) { DeleteCircuit<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
bool                           SECircuitManager::HasThermalCircuit(const std::string& name) const { return HasCircuit<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
SEThermalCircuit*              SECircuitManager::GetThermalCircuit(const std::string& name) { return GetCircuit<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
const SEThermalCircuit*        SECircuitManager::GetThermalCircuit(const std::string& name) const { return GetCircuit<THERMAL_LEDGER_TYPES>(name, m_ThermalLedger); }
