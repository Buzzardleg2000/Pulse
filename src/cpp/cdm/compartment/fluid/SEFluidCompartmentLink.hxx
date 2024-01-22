/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::SEFluidCompartmentLink(CompartmentType& src, CompartmentType & tgt, const std::string& name) : SECompartmentLink(name, src.GetLogger()), m_SourceCmpt(src), m_TargetCmpt(tgt), m_SourceVertex(src), m_TargetVertex(tgt)
{
  m_Flow = nullptr;
  m_Path = nullptr;
  
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::~SEFluidCompartmentLink()
{

}

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
void SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::Clear()
{
  m_Path = nullptr;
  SAFE_DELETE(m_Flow);
}

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
const SEScalar* SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetScalar(const std::string& name)
{
  if (name.compare("Flow") == 0)
    return &GetFlow();
  return nullptr;
}

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
bool SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::HasCompliance() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextCompliance();
  return false;
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
double SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextCompliance(unit);
  return SEScalar::dNaN();
}

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
bool SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::HasFlow() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextFlow();
  return m_Flow == nullptr ? false : m_Flow->IsValid();
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
SEScalarVolumePerTime& SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetFlow()
{
  if (m_Path != nullptr)
    return m_Path->GetNextFlow();
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
double SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextFlow(unit);
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}


template<FLUID_COMPARTMENT_LINK_TEMPLATE>
bool SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::HasResistance() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextResistance();
  return false;
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
double SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>::GetResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextResistance(unit);
  return SEScalar::dNaN();
}
