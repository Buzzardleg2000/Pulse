/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/circuit/fluid/SEFluidCircuit.h"
#include "cdm/circuit/fluid/SEFluidCircuitNode.h"
#include "cdm/circuit/fluid/SEFluidCircuitPath.h"

#include "cdm/circuit/thermal/SEThermalCircuit.h"
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"

#include "cdm/circuit/electrical/SEElectricalCircuit.h"
#include "cdm/circuit/electrical/SEElectricalCircuitNode.h"
#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"


template<CIRCUIT_LEDGER_TEMPLATE>
void SECircuitLedger<CIRCUIT_LEDGER_TYPES>::Clear()
{
  DELETE_MAP_SECOND(nodes);
  DELETE_MAP_SECOND(paths);
  DELETE_MAP_SECOND(circuits);
}

template<CIRCUIT_LEDGER_TEMPLATE> NodeType& SECircuitManager::CreateNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  NodeType* node = ledger.nodes[name];
  if (node != nullptr)
  {
    Error("Node already exists with name " + name + ", not creating a new node, returning the existing node");
    return *node;
  }
  node = new NodeType(name, GetLogger());
  ledger.nodes[name] = node;
  return *node;
}
template SEFluidCircuitNode& SECircuitManager::CreateNode<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuitNode& SECircuitManager::CreateNode<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuitNode& SECircuitManager::CreateNode<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> void SECircuitManager::DeleteNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path;
  NodeType* node = GetNode(name, ledger);
  if (node == nullptr)
    return;
  // Remove this node from any circuits
  for (auto citr : ledger.circuits)
    citr.second->RemoveNode(*node);
  // Search the paths and delete any paths that has this node
  // And remove those paths from any circuits
  for (auto pitr = ledger.paths.begin(); pitr != ledger.paths.end();)
  {
    if (pitr->second->GetSourceNode().GetName() == name)
    {
      path = pitr->second;
      ledger.paths.erase(pitr++);
      for (auto citr : ledger.circuits)
        citr.second->RemovePath(*path);
      SAFE_DELETE(path);
    }
    else
      pitr++;
  }
  ledger.nodes.erase(node->GetName());
  SAFE_DELETE(node);
}
template void SECircuitManager::DeleteNode<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeleteNode<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeleteNode<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> bool SECircuitManager::HasNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.nodes.find(name) != ledger.nodes.end();
}
template bool SECircuitManager::HasNode<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasNode<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasNode<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;

template<CIRCUIT_LEDGER_TEMPLATE> NodeType* SECircuitManager::GetNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.nodes.find(name);
  if (itr != ledger.nodes.end())
    return itr->second;
  return nullptr;
}
template SEFluidCircuitNode* SECircuitManager::GetNode<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuitNode* SECircuitManager::GetNode<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuitNode* SECircuitManager::GetNode<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> const NodeType* SECircuitManager::GetNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.nodes.find(name);
  if (itr != ledger.nodes.end())
    return itr->second;
  return nullptr;
}
template const SEFluidCircuitNode* SECircuitManager::GetNode<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template const SEElectricalCircuitNode* SECircuitManager::GetNode<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template const SEThermalCircuitNode* SECircuitManager::GetNode<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;

template<CIRCUIT_LEDGER_TEMPLATE> PathType& SECircuitManager::CreatePath(NodeType& src, NodeType& tgt, const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path = ledger.paths[name];
  if (path != nullptr)
  {
    Warning("Path already exists with name " + name + ", not creating a new path, returning the existing path");
    return *path;
  }
  path = new PathType(src, tgt, name);
  ledger.paths[name] = path;
  return *path;
}
template SEFluidCircuitPath& SECircuitManager::CreatePath<FLUID_LEDGER_TYPES>(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuitPath& SECircuitManager::CreatePath<ELECTRICAL_LEDGER_TYPES>(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuitPath& SECircuitManager::CreatePath<THERMAL_LEDGER_TYPES>(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> void SECircuitManager::DeletePath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path = GetPath(name, ledger);
  if (path == nullptr)
    return;
  ledger.paths.erase(name);
  for (auto itr : ledger.circuits)
    itr.second->RemovePath(*path);
  SAFE_DELETE(path);
}
template void SECircuitManager::DeletePath<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeletePath<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeletePath<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> bool SECircuitManager::HasPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.paths.find(name) != ledger.paths.end();
}
template bool SECircuitManager::HasPath<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasPath<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasPath<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;

template<CIRCUIT_LEDGER_TEMPLATE> PathType* SECircuitManager::GetPath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.paths.find(name);
  if (itr != ledger.paths.end())
    return itr->second;
  return nullptr;
}
template SEFluidCircuitPath* SECircuitManager::GetPath<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuitPath* SECircuitManager::GetPath<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuitPath* SECircuitManager::GetPath<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> const PathType* SECircuitManager::GetPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.paths.find(name);
  if (itr != ledger.paths.end())
    return itr->second;
  return nullptr;
}
template const SEFluidCircuitPath* SECircuitManager::GetPath<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template const SEElectricalCircuitPath* SECircuitManager::GetPath<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template const SEThermalCircuitPath* SECircuitManager::GetPath<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;

template<CIRCUIT_LEDGER_TEMPLATE> CircuitType& SECircuitManager::CreateCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  CircuitType* circuit = ledger.circuits[name];
  if (circuit != nullptr)
  {
    Error(" Circuit already exists with name " + name + ", not creating a new circuit, returning the existing circuit");
    return *circuit;
  }
  circuit = new CircuitType(name, *this);
  ledger.circuits[name] = circuit;
  return *circuit;
}
template SEFluidCircuit& SECircuitManager::CreateCircuit<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuit& SECircuitManager::CreateCircuit<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuit& SECircuitManager::CreateCircuit<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> void SECircuitManager::DeleteCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  CircuitType* circuit = GetCircuit(name, ledger);
  if (circuit == nullptr)
    return;
  ledger.circuits.erase(name);
  SAFE_DELETE(circuit);
}
template void SECircuitManager::DeleteCircuit<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeleteCircuit<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template void SECircuitManager::DeleteCircuit<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> bool SECircuitManager::HasCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.circuits.find(name) != ledger.circuits.end();
}
template bool SECircuitManager::HasCircuit<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasCircuit<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template bool SECircuitManager::HasCircuit<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;

template<CIRCUIT_LEDGER_TEMPLATE> CircuitType* SECircuitManager::GetCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.circuits.find(name);
  if (itr != ledger.circuits.end())
    return itr->second;
  return nullptr;
}
template SEFluidCircuit* SECircuitManager::GetCircuit<FLUID_LEDGER_TYPES>(const std::string& name, SECircuitLedger<FLUID_LEDGER_TYPES>& ledger);
template SEElectricalCircuit* SECircuitManager::GetCircuit<ELECTRICAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger);
template SEThermalCircuit* SECircuitManager::GetCircuit<THERMAL_LEDGER_TYPES>(const std::string& name, SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger);

template<CIRCUIT_LEDGER_TEMPLATE> const CircuitType* SECircuitManager::GetCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.circuits.find(name);
  if (itr != ledger.circuits.end())
    return itr->second;
  return nullptr;
}
template const SEFluidCircuit* SECircuitManager::GetCircuit<FLUID_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<FLUID_LEDGER_TYPES>& ledger) const;
template const SEElectricalCircuit* SECircuitManager::GetCircuit<ELECTRICAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<ELECTRICAL_LEDGER_TYPES>& ledger) const;
template const SEThermalCircuit* SECircuitManager::GetCircuit<THERMAL_LEDGER_TYPES>(const std::string& name, const SECircuitLedger<THERMAL_LEDGER_TYPES>& ledger) const;
