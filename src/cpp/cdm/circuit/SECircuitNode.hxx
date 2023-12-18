/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

template<CIRCUIT_NODE_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>::SECircuitNode(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
  m_Potential = nullptr;
  m_NextPotential = nullptr;
  m_Quantity = nullptr;
  m_NextQuantity = nullptr;
  m_QuantityBaseline = nullptr;
}

template<CIRCUIT_NODE_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>::~SECircuitNode()
{
  Clear();
}

template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::Clear()
{
  SAFE_DELETE(m_Potential);
  SAFE_DELETE(m_NextPotential);
  SAFE_DELETE(m_Quantity);
  SAFE_DELETE(m_NextQuantity);
  SAFE_DELETE(m_QuantityBaseline);
}

template<CIRCUIT_NODE_TEMPLATE>
std::string SECircuitNode<CIRCUIT_NODE_TYPES>::GetName() const
{
  return m_Name;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::IsPartOfBlackBox() const
{
  return m_IsPartOfBlackBox;
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::SetPartOfBlackBox(bool b)
{
  m_IsPartOfBlackBox = b;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::IsBlackBoxMiddle() const
{
  return m_IsBlackBoxMiddle;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasPotential() const
{
  return m_Potential == nullptr ? false : m_Potential->IsValid();
}
template<CIRCUIT_NODE_TEMPLATE>
PotentialScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetPotential()
{
  if (m_Potential == nullptr)
    m_Potential = new PotentialScalar();
  return *m_Potential;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasNextPotential() const
{
  return m_NextPotential == nullptr ? false : m_NextPotential->IsValid();
}
template<CIRCUIT_NODE_TEMPLATE>
PotentialScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetNextPotential()
{
  if (m_NextPotential == nullptr)
    m_NextPotential = new PotentialScalar();
  return *m_NextPotential;
}

template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::RemoveImposedPotential()
{
  m_PotentialType = eBlackBox_Property_Type::Calculate;
}
template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::IsPotentialImposed() const
{
  return m_PotentialType == eBlackBox_Property_Type::Imposed;
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::ImposePotential(double v, const PotentialUnit& unit)
{
  m_PotentialType = eBlackBox_Property_Type::Imposed;
  if (m_NextPotential == nullptr)
    m_NextPotential = new PotentialScalar();
  m_NextPotential->ForceValue(v, unit);
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::ImposePotential(const PotentialScalar& s)
{
  m_PotentialType = eBlackBox_Property_Type::Imposed;
  if (m_NextPotential == nullptr)
    m_NextPotential = new PotentialScalar();
  m_NextPotential->Force(s);
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasQuantity() const
{
  return m_Quantity == nullptr ? false : m_Quantity->IsValid();
}
template<CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetQuantity()
{
  if (m_Quantity == nullptr)
    m_Quantity = new QuantityScalar();
  return *m_Quantity;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasNextQuantity() const
{
  return m_NextQuantity == nullptr ? false : m_NextQuantity->IsValid();
}
template<CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetNextQuantity()
{
  if (m_NextQuantity == nullptr)
    m_NextQuantity = new QuantityScalar();
  return *m_NextQuantity;
}

template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasQuantityBaseline() const
{
  return m_QuantityBaseline == nullptr ? false : m_QuantityBaseline->IsValid();
}
template<CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetQuantityBaseline()
{
  if (m_QuantityBaseline == nullptr)
    m_QuantityBaseline = new QuantityScalar();
  return *m_QuantityBaseline;
}

template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::RemoveImposedQuantity()
{
  m_QuantityType = eBlackBox_Property_Type::Calculate;
}
template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::IsQuantityImposed() const
{
  return m_QuantityType == eBlackBox_Property_Type::Imposed;
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::ImposeQuantity(double v, const QuantityUnit& unit)
{
  m_QuantityType = eBlackBox_Property_Type::Imposed;
  m_NextQuantity->ForceValue(v, unit);
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::ImposeQuantity(const QuantityScalar& s)
{
  m_QuantityType = eBlackBox_Property_Type::Imposed;
  m_NextQuantity->Force(s);
}

template<CIRCUIT_NODE_TEMPLATE>
int SECircuitNode<CIRCUIT_NODE_TYPES>::GetCalculatorIndex() const
{
  return m_CalculatorIndex;
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::SetCalculatorIndex(const int index)
{
  m_CalculatorIndex = index;
}

template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::RemoveAsReferenceNode()
{
  m_IsReferenceNode = false;
}
template<CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::SetAsReferenceNode()
{
  m_IsReferenceNode = true;
}
template<CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::IsReferenceNode() const
{
  return m_IsReferenceNode;
}

#include "cdm/circuit/fluid/SEFluidCircuitNode.h"
template class SECircuitNode<FLUID_CIRCUIT_NODE>;
#include "cdm/circuit/electrical/SEElectricalCircuitNode.h"
template class SECircuitNode<ELECTRICAL_CIRCUIT_NODE>;
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"
template class SECircuitNode<THERMAL_CIRCUIT_NODE>;
