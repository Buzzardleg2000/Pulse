/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class SEElectricalCircuit;
class SEElectricalCircuitNode;
class SEElectricalCircuitPath;

#define CIRCUIT_LEDGER_TEMPLATE typename NodeType, typename PathType, typename CircuitType
#define CIRCUIT_LEDGER_TYPES NodeType,PathType,CircuitType

template<CIRCUIT_LEDGER_TEMPLATE>
class SECircuitLedger
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;
protected:
  SECircuitLedger() {};
public:
  virtual ~SECircuitLedger() { Clear(); };
  void Clear();
  std::map<std::string, NodeType*>    nodes;
  std::map<std::string, PathType*>    paths;
  std::map<std::string, CircuitType*> circuits;
};

#define ELECTRICAL_LEDGER_TYPES SEElectricalCircuitNode,SEElectricalCircuitPath,SEElectricalCircuit
#define FLUID_LEDGER_TYPES SEFluidCircuitNode,SEFluidCircuitPath,SEFluidCircuit
#define THERMAL_LEDGER_TYPES SEThermalCircuitNode,SEThermalCircuitPath,SEThermalCircuit

class CDM_DECL SECircuitManager : public Loggable
{
  friend class PBCircuit;//friend the serialization class
public:
  SECircuitManager(Logger* logger);
  virtual ~SECircuitManager();

  virtual void Clear(); //clear memory

  virtual void StateChange() { };

  void SetReadOnly(bool b);
protected:

public:
  virtual SEElectricalCircuitNode&       CreateElectricalNode(const std::string& name);
  virtual void                           DeleteElectricalNode(const std::string& name);
  virtual bool                           HasElectricalNode(const std::string& name) const;
  virtual SEElectricalCircuitNode*       GetElectricalNode(const std::string& name);
  virtual const SEElectricalCircuitNode* GetElectricalNode(const std::string& name) const;
  virtual SEElectricalCircuitPath&       CreateElectricalPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name);
  virtual void                           DeleteElectricalPath(const std::string& name);
  virtual bool                           HasElectricalPath(const std::string& name) const;
  virtual SEElectricalCircuitPath*       GetElectricalPath(const std::string& name);
  virtual const SEElectricalCircuitPath* GetElectricalPath(const std::string& name) const;
  virtual SEElectricalCircuit&           CreateElectricalCircuit(const std::string& name);
  virtual void                           DeleteElectricalCircuit(const std::string& name);
  virtual bool                           HasElectricalCircuit(const std::string& name) const;
  virtual SEElectricalCircuit*           GetElectricalCircuit(const std::string& name);
  virtual const SEElectricalCircuit*     GetElectricalCircuit(const std::string& name) const;
  virtual void                           SetReadOnlyElectrical(bool b);
protected:                        
  SECircuitLedger<SEElectricalCircuitNode,SEElectricalCircuitPath,SEElectricalCircuit> m_ElectricalLedger;

public:
  virtual SEFluidCircuitNode&            CreateFluidNode(const std::string& name);
  virtual void                           DeleteFluidNode(const std::string& name);
  virtual bool                           HasFluidNode(const std::string& name) const;
  virtual SEFluidCircuitNode*            GetFluidNode(const std::string& name);
  virtual const SEFluidCircuitNode*      GetFluidNode(const std::string& name) const;
  virtual SEFluidCircuitPath&            CreateFluidPath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name);
  virtual void                           DeleteFluidPath(const std::string& name);
  virtual bool                           HasFluidPath(const std::string& name) const;
  virtual SEFluidCircuitPath*            GetFluidPath(const std::string& name);
  virtual const SEFluidCircuitPath*      GetFluidPath(const std::string& name) const;
  virtual SEFluidCircuit&                CreateFluidCircuit(const std::string& name);
  virtual void                           DeleteFluidCircuit(const std::string& name);
  virtual bool                           HasFluidCircuit(const std::string& name) const;
  virtual SEFluidCircuit*                GetFluidCircuit(const std::string& name);
  virtual const SEFluidCircuit*          GetFluidCircuit(const std::string& name) const;
  virtual void                           SetReadOnlyFluid(bool b);
protected:
  SECircuitLedger<SEFluidCircuitNode, SEFluidCircuitPath, SEFluidCircuit> m_FluidLedger;

public:
  virtual SEThermalCircuitNode&          CreateThermalNode(const std::string& name);
  virtual void                           DeleteThermalNode(const std::string& name);
  virtual bool                           HasThermalNode(const std::string& name) const;
  virtual SEThermalCircuitNode*          GetThermalNode(const std::string& name);
  virtual const SEThermalCircuitNode*    GetThermalNode(const std::string& name) const;
  virtual SEThermalCircuitPath&          CreateThermalPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);
  virtual void                           DeleteThermalPath(const std::string& name);
  virtual bool                           HasThermalPath(const std::string& name) const;
  virtual SEThermalCircuitPath*          GetThermalPath(const std::string& name);
  virtual const SEThermalCircuitPath*    GetThermalPath(const std::string& name) const;
  virtual SEThermalCircuit&              CreateThermalCircuit(const std::string& name);
  virtual void                           DeleteThermalCircuit(const std::string& name);
  virtual bool                           HasThermalCircuit(const std::string& name) const;
  virtual SEThermalCircuit*              GetThermalCircuit(const std::string& name);
  virtual const SEThermalCircuit*        GetThermalCircuit(const std::string& name) const;
  virtual void                           SetReadOnlyThermal(bool b); ;
protected:
  SECircuitLedger<SEThermalCircuitNode, SEThermalCircuitPath, SEThermalCircuit> m_ThermalLedger;


protected:
  template<CIRCUIT_LEDGER_TEMPLATE> NodeType&          CreateNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> void               DeleteNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> bool               HasNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
  template<CIRCUIT_LEDGER_TEMPLATE> NodeType*          GetNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> const NodeType*    GetNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
  template<CIRCUIT_LEDGER_TEMPLATE> PathType&          CreatePath(NodeType& src, NodeType& tgt, const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> void               DeletePath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> bool               HasPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
  template<CIRCUIT_LEDGER_TEMPLATE> PathType*          GetPath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> const PathType*    GetPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
  template<CIRCUIT_LEDGER_TEMPLATE> CircuitType&       CreateCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> void               DeleteCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> bool               HasCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
  template<CIRCUIT_LEDGER_TEMPLATE> CircuitType*       GetCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger);
  template<CIRCUIT_LEDGER_TEMPLATE> const CircuitType* GetCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const;
};


#include "cdm/circuit/SECircuitManager.hxx"
