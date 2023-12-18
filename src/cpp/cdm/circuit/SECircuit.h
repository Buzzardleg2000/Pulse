/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitNode.h"
#include "cdm/circuit/SECircuitPath.h"

#define CIRCUIT_TEMPLATE typename NodeType, typename PathType
#define CIRCUIT_TYPES NodeType,PathType

template<CIRCUIT_TEMPLATE>
class SECircuit : public Loggable
{
  friend class PBCircuit;//friend the serialization class
public:

  SECircuit(const std::string& name, Logger* logger);
  virtual ~SECircuit();

  virtual void Clear(); //clear memory

  virtual std::string GetName() const;

  virtual bool HasReferenceNode() const;

  // Nodes //
  virtual void AddNode(NodeType& node);
  virtual void ForceAddNode(NodeType& node);// No check for node with the same name
  virtual bool HasNode(NodeType& node);
  virtual bool HasNode(const std::string& name);
  virtual NodeType* GetNode(const std::string& name);
  virtual const NodeType* GetNode(const std::string& name) const;
  virtual const std::vector<NodeType*>& GetNodes() const;
  virtual void RemoveNode(const NodeType& node);
  virtual void RemoveNode(const std::string& name);

  // Paths //
  virtual void AddPath(PathType& node);
  virtual void ForceAddPath(PathType& node);// No check for path with the same name
  virtual bool HasPath(PathType& node);
  virtual bool HasPath(const std::string& name);
  virtual PathType* GetPath(const std::string& name);
  virtual const PathType* GetPath(const std::string& name) const;
  virtual const std::vector<PathType*>& GetPaths() const;
  virtual void RemovePath(const PathType& path);
  virtual void RemovePath(const std::string& name);
  virtual const std::vector<PathType*>& GetValvePaths();
  virtual const std::vector<PathType*>& GetPolarizedElementPaths();
  
  virtual const std::vector<PathType*>* GetTargetPaths(const NodeType& node) const;
  virtual const std::vector<PathType*>* GetSourcePaths(const NodeType& node) const;
  virtual const std::vector<PathType*>* GetConnectedPaths(const NodeType& node) const;

  virtual void StateChange();
  virtual void SetNextAndCurrentFromBaselines();

protected:
  std::string  m_Name;
  std::stringstream m_ss;
  
  std::vector<NodeType*>               m_Nodes;
  std::vector<PathType*>               m_Paths;
   
  std::map<const NodeType*, std::vector<PathType*>*> m_TargetPathMap;
  std::map<const NodeType*, std::vector<PathType*>*> m_SourcePathMap;
  std::map<const NodeType*, std::vector<PathType*>*> m_ConnectedPathMap;

  std::vector<PathType*>            m_ValvePaths;
  std::vector<PathType*>            m_PolarizedElementPaths;
};

#include "cdm/circuit/SECircuit.hxx"
