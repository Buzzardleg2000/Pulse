/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#define ZERO_APPROX 1e-10
#define OPEN_RESISTANCE 1e100

template<CIRCUIT_TEMPLATE>
SECircuit<CIRCUIT_TYPES>::SECircuit(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
}

template<CIRCUIT_TEMPLATE>
SECircuit<CIRCUIT_TYPES>::~SECircuit()
{
  Clear();
}

template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::Clear()
{
  for (auto& itr : m_TargetPathMap)
    delete itr.second;
  for (auto& itr : m_SourcePathMap)
    delete itr.second;
  for (auto& itr : m_ConnectedPathMap)
    delete itr.second;
  m_Nodes.clear();
  m_Paths.clear();
  m_TargetPathMap.clear();
  m_SourcePathMap.clear();
  m_ConnectedPathMap.clear();
  m_ValvePaths.clear();
  m_PolarizedElementPaths.clear();
}

template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::StateChange()
{
  // Cache target and source paths to save lots of time later
  for (auto& itr : m_TargetPathMap)
    delete itr.second;
  for (auto& itr : m_SourcePathMap)
    delete itr.second;
  for (auto& itr : m_ConnectedPathMap)
    delete itr.second;
  m_TargetPathMap.clear();
  m_SourcePathMap.clear();
  m_ConnectedPathMap.clear();
  m_ValvePaths.clear();
  m_PolarizedElementPaths.clear();

  for (PathType* p : m_Paths)
  {
    if (!p->HasValidElements())
    {
      m_ss << p->GetName() << " has invalid elements";
      Fatal(m_ss);
    }

    //There should never be a NextFlow value set on a path
    //Flow sources are defined using NextFlowSource
    /* if (p->HasNextFlux())
    {
    m_ss << p->GetName() << " has flux and will be ignored and overwritten. Use a flux source.";
    Warning(m_ss);
    }*/

    // Cache what paths are connected to what nodes
    NodeType* nTgt = &p->GetTargetNode();
    NodeType* nSrc = &p->GetSourceNode();
    std::vector<PathType*>* tgtPaths = m_TargetPathMap[nTgt];
    std::vector<PathType*>* srcPaths = m_SourcePathMap[nSrc];
    std::vector<PathType*>* conSrcPaths = m_ConnectedPathMap[nSrc];
    std::vector<PathType*>* conTgtPaths = m_ConnectedPathMap[nTgt];
    if (tgtPaths == nullptr)
    {
      tgtPaths = new std::vector<PathType*>();
      m_TargetPathMap[nTgt] = tgtPaths;
    }
    if (srcPaths == nullptr)
    {
      srcPaths = new std::vector<PathType*>();
      m_SourcePathMap[nSrc] = srcPaths;
    }
    if (conTgtPaths == nullptr)
    {
      conTgtPaths = new std::vector<PathType*>();
      m_ConnectedPathMap[nTgt] = conTgtPaths;
    }
    if (conSrcPaths == nullptr)
    {
      conSrcPaths = new std::vector<PathType*>();
      m_ConnectedPathMap[nSrc] = conSrcPaths;
    }

    //Make sure we didn't accidentally define something twice
    if (std::find(m_ConnectedPathMap[nSrc]->begin(), m_ConnectedPathMap[nSrc]->end(), p) != m_ConnectedPathMap[nSrc]->end() ||
      std::find(m_ConnectedPathMap[nTgt]->begin(), m_ConnectedPathMap[nTgt]->end(), p) != m_ConnectedPathMap[nTgt]->end())
    {
      m_ss << p->GetName() << " is defined multiple times.";
      Fatal(m_ss);
    }

    m_TargetPathMap[nTgt]->push_back(p);
    m_SourcePathMap[nSrc]->push_back(p);
    m_ConnectedPathMap[nSrc]->push_back(p);
    m_ConnectedPathMap[nTgt]->push_back(p);

    // Cache Valves
    if (p->HasNextValve())
      m_ValvePaths.push_back(p);
    if (p->HasNextPolarizedState())
      m_PolarizedElementPaths.push_back(p);
  }

  // Check blackbox nodes
  for (NodeType* n : m_Nodes)
  {
    if (n->IsBlackBoxMiddle())
    {
      auto paths = GetConnectedPaths(*n);
      if(paths->size() != 2)
      {
        Fatal(n->GetName() + " has invalid black box configuration, must have only 2 paths associated");
      }
    }
  }

  if ((m_ValvePaths.size()+m_PolarizedElementPaths.size()) > 64)
  {
    Fatal("There are too many assumed state options.  The Circuit solver can only handle up to 64 Diodes and Polar Elements in a single circuit (i.e. ~1.8e19 possible combinations).");
  }
}

template<CIRCUIT_TEMPLATE>
std::string SECircuit<CIRCUIT_TYPES>::GetName() const
{
  return m_Name;
}

template<CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasReferenceNode() const
{
    for (NodeType* n : m_Nodes)
    {
        if (n->IsReferenceNode())
        {
            return true;
        }
    }
  return false;
}

template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::AddNode(NodeType& node)
{
  if (!Contains(this->m_Nodes, node))
    this->m_Nodes.push_back(&node);
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::ForceAddNode(NodeType& node)
{
  this->m_Nodes.push_back(&node);
}
template<CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasNode(NodeType& node)
{
  return Contains(m_Nodes, node);
}
template<CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasNode(const std::string& name)
{
  return GetNode(name) != nullptr;
}
template<CIRCUIT_TEMPLATE>
NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const std::string& name)
{
  for (NodeType* n : m_Nodes)
  {
    if (n->GetName() == name)
      return n;
  }
  return nullptr;
}
template<CIRCUIT_TEMPLATE>
const NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const std::string& name) const
{
  for (NodeType* n : m_Nodes)
  {
    if (n->GetName() == name)
      return n;
  }
  return nullptr;
}
template<CIRCUIT_TEMPLATE>
const std::vector<NodeType*>& SECircuit<CIRCUIT_TYPES>::GetNodes() const
{
  return m_Nodes;
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemoveNode(const NodeType& node)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes)
  {
    if (n == &node)
    {
      m_Nodes.erase(m_Nodes.begin() + i);
      return;
    }
    i++;
  }
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemoveNode(const std::string& name)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes)
  { 
    if(n->GetName()==name)
    {
      m_Nodes.erase(m_Nodes.begin()+i);
      return;
    }
    i++;
  } 
}

template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::AddPath(PathType& path)
{
  if (!Contains(m_Paths, path))
    m_Paths.push_back(&path);
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::ForceAddPath(PathType& path)
{
  m_Paths.push_back(&path);
}
template<CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasPath(PathType& path)
{
  return Contains(m_Paths, path);
}
template<CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasPath(const std::string& name)
{
  return GetPath(name) != nullptr;
}
template<CIRCUIT_TEMPLATE>
PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const std::string& name)
{
  for (PathType* p : m_Paths)
  {
    if(p->GetName() == name)
      return p;
  }
  return nullptr;
}
template<CIRCUIT_TEMPLATE>
const PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const std::string& name) const
{
  for (PathType* p : m_Paths)
  {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetPaths() const
{
  return m_Paths;
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemovePath(const PathType& path)
{
  size_t i = 0;
  for (PathType* p : m_Paths)
  { 
    if(p==&path)
    {
      m_Paths.erase(m_Paths.begin()+i);
      return;
    }
    i++;
  } 
}
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemovePath(const std::string& name)
{
  size_t i = 0;
  for (PathType* p : m_Paths)
  {
    if (p->GetName() == name)
    {
      m_Paths.erase(m_Paths.begin() + i);
      return;
    }
    i++;
  }
}
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetValvePaths()
{
  return m_ValvePaths;
}
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetPolarizedElementPaths()
{
  return m_PolarizedElementPaths;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all target paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the target node. Returns a vector of the paths.
//--------------------------------------------------------------------------------------------------
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetTargetPaths(const NodeType& node) const
{
  auto itr = m_TargetPathMap.find(&node);
  if (itr == m_TargetPathMap.end())
  {
    Error("Circuit does not have path information for node : " + node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all source paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the source node. Returns a vector of the paths.
//--------------------------------------------------------------------------------------------------
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetSourcePaths(const NodeType& node) const
{
  auto itr = m_SourcePathMap.find(&node);
  if (itr == m_SourcePathMap.end())
  {
    Error("Circuit does not have path information for node : " + node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all source paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the source or target node. Returns a vector of the paths.
//--------------------------------------------------------------------------------------------------
template<CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetConnectedPaths(const NodeType& node) const
{
  auto itr = m_ConnectedPathMap.find(&node);
  if (itr == m_ConnectedPathMap.end())
  {
    Error("Circuit does not have path information for node : " + node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set all Current and Next values to the Baseline values for all Elements.
//--------------------------------------------------------------------------------------------------
template<CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::SetNextAndCurrentFromBaselines()
{
  for(PathType* p : m_Paths)
  {
    if (p->HasResistanceBaseline())
    {
      p->GetResistance().Copy(p->GetResistanceBaseline());
      p->GetNextResistance().Copy(p->GetResistanceBaseline());
    }

    if (p->HasCapacitanceBaseline())
    {
      p->GetCapacitance().Copy(p->GetCapacitanceBaseline());
      p->GetNextCapacitance().Copy(p->GetCapacitanceBaseline());
    }

    if (p->HasInductanceBaseline())
    {
      p->GetInductance().Copy(p->GetInductanceBaseline());
      p->GetNextInductance().Copy(p->GetInductanceBaseline());
    }

    if (p->HasFluxSourceBaseline())
    {
      p->GetFluxSource().Copy(p->GetFluxSourceBaseline());
      p->GetNextFluxSource().Copy(p->GetFluxSourceBaseline());
    }

    if (p->HasPotentialSourceBaseline())
    {
      p->GetPotentialSource().Copy(p->GetPotentialSourceBaseline());
      p->GetNextPotentialSource().Copy(p->GetPotentialSourceBaseline());
    }

    if (p->HasSwitch())
      p->SetNextSwitch(p->GetSwitch());
    if (p->HasValve())
      p->SetNextValve(p->GetValve());
  }

  for (NodeType* n : m_Nodes)
  {
    if (n->HasQuantityBaseline())
    {
      n->GetQuantity().Copy(n->GetQuantityBaseline());
      n->GetNextQuantity().Copy(n->GetQuantityBaseline());
    }
  }
}

#include "cdm/circuit/electrical/SEElectricalCircuit.h"
template class SECircuit< SEElectricalCircuitNode, SEElectricalCircuitPath>;
#include "cdm/circuit/fluid/SEFluidCircuit.h"
template class SECircuit< SEFluidCircuitNode, SEFluidCircuitPath>;
#include "cdm/circuit/thermal/SEThermalCircuit.h"
template class SECircuit< SEThermalCircuitNode, SEThermalCircuitPath>;