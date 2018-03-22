/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuitNode.h"
#include "circuit/SECircuitPath.h"

#define CIRCUIT_TEMPLATE typename CircuitBindType, typename NodeType, typename CircuitNodeBindType, typename PathType, typename CircuitPathBindType
#define CIRCUIT_TYPES CircuitBindType,NodeType,CircuitNodeBindType,PathType,CircuitPathBindType

template<CIRCUIT_TEMPLATE>
class SECircuit : public Loggable
{
public:

  SECircuit(const std::string& name, Logger* logger);
  virtual ~SECircuit();

  virtual void Clear(); //clear memory

  static void Load(const CircuitBindType& src, SECircuit& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  static CircuitBindType* Unload(const SECircuit& src);
protected:
  static void Serialize(const CircuitBindType& src, SECircuit& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  static void Serialize(const SECircuit& src, CircuitBindType& dst);

public:
  virtual std::string GetName() const;

  virtual bool HasReferenceNode() const;

  // Nodes //
  virtual void AddNode(NodeType& node);
  virtual bool HasNode(NodeType& node);
  virtual bool HasNode(const std::string& name);
  virtual NodeType* GetNode(const std::string& name);
  virtual const NodeType* GetNode(const std::string& name) const;
  virtual const std::vector<NodeType*>& GetNodes() const;
  virtual void RemoveNode(const NodeType& node);
  virtual void RemoveNode(const std::string& name);

  // Paths //
  virtual void AddPath(PathType& node);
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
#include "circuit/SECircuit.inl"