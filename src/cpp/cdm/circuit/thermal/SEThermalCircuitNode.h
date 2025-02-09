/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitNode.h"
class SEThermalBlackBox;
class SEThermalCircuitPath;

class CDM_DECL SEThermalCircuitNode : public SECircuitNode<THERMAL_CIRCUIT_NODE>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;  
protected:
  SEThermalCircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SEThermalCircuitNode();

  void Clear() override; //clear memory

  virtual bool HasTemperature() const;
  virtual SEScalarTemperature& GetTemperature();
  virtual double GetTemperature(const TemperatureUnit& unit) const;

  virtual bool HasNextTemperature() const;
  virtual SEScalarTemperature& GetNextTemperature();
  virtual double GetNextTemperature(const TemperatureUnit& unit) const;


  virtual bool HasHeat() const;
  virtual SEScalarEnergy& GetHeat();
  virtual double GetHeat(const EnergyUnit& unit) const;

  virtual bool HasNextHeat() const;
  virtual SEScalarEnergy& GetNextHeat();
  virtual double GetNextHeat(const EnergyUnit& unit) const;

  virtual bool HasHeatBaseline() const;
  virtual SEScalarEnergy& GetHeatBaseline();
  virtual double GetHeatBaseline(const EnergyUnit& unit) const;

  SEThermalCircuitNode* GetBlackBoxSourceNode() const;
  SEThermalCircuitNode* GetBlackBoxTargetNode() const;
  void SetBlackBoxSourceTargetNodes(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt);

  SEThermalCircuitPath* GetBlackBoxSourcePath() const;
  SEThermalCircuitPath* GetBlackBoxTargetPath() const;
  void SetBlackBoxSourceTargetPaths(SEThermalCircuitPath& src, SEThermalCircuitPath& tgt);

protected:
  SEThermalCircuitNode* m_BlackBoxSourceNode=nullptr;// Only set on middle nodes
  SEThermalCircuitNode* m_BlackBoxTargetNode=nullptr;// Only set on middle nodes
  SEThermalCircuitPath* m_BlackBoxSourcePath = nullptr;// Only set on middle nodes
  SEThermalCircuitPath* m_BlackBoxTargetPath = nullptr;// Only set on middle nodes
};
