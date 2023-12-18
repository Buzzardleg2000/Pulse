/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#define COMPARTMENT_NODE_TEMPLATE typename NodeType, typename PotentialScalar, typename PotentialUnit, typename QuantityScalar, typename QuantityUnit
#define COMPARTMENT_NODE_TYPES NodeType, PotentialScalar, PotentialUnit, QuantityScalar, QuantityUnit
#define ELECTRICAL_COMPARTMENT_NODE SEElectricalCircuitNode, SEScalarElectricPotential, ElectricPotentialUnit, SEScalarElectricCharge, ElectricChargeUnit
#define FLUID_COMPARTMENT_NODE SEFluidCircuitNode, SEScalarPressure, PressureUnit, SEScalarVolume, VolumeUnit
#define THERMAL_COMPARTMENT_NODE SEThermalCircuitNode, SEScalarTemperature, TemperatureUnit, SEScalarEnergy, EnergyUnit

template<COMPARTMENT_NODE_TEMPLATE>
class SECompartmentNodes : public Loggable
{
  friend class PBCompartment;//friend the serialization class
public:
  
  SECompartmentNodes(Logger* logger);
  virtual ~SECompartmentNodes();

  virtual void Clear();
  virtual void StateChange(); //analyze and cache for optimization

  virtual bool HasMapping() const { return !m_AllNodes.empty(); }
  virtual bool HasMapping(const NodeType& n) const 
  { 
    for (auto mine : m_AllNodes)
      if (&n == mine)
        return true;
    return false;
  }
  virtual const std::vector<NodeType*>& GetNodes() const { return m_AllNodes; }
  virtual const std::vector<NodeType*>& GetQuantityNodes() const { return m_QuantityNodes; }
  virtual void RemoveNode(NodeType& node);
  virtual void MapNode(NodeType& node);

  virtual bool HasQuantity() const { return !m_QuantityNodes.empty(); }
  virtual QuantityScalar& GetQuantity();
  virtual double GetQuantity(const QuantityUnit& unit) const;

  virtual bool HasPotential() const;
  virtual PotentialScalar& GetPotential();
  virtual double GetPotential(const PotentialUnit& unit) const;

protected:
  void SortNode(NodeType& node);

  // I assume all nodes will have a pressure
  std::vector<NodeType*> m_QuantityNodes;
  std::vector<NodeType*> m_AllNodes;//Even nodes without quantity (ex. volume)

  QuantityScalar*  m_Quantity;
  PotentialScalar* m_Potential;
};

#include "cdm/compartment/SECompartmentNodes.hxx"
