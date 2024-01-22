/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/substances/SELiquidSubstanceQuantity.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalar.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

SELiquidCompartment::SELiquidCompartment(const std::string& name, Logger* logger) : SEFluidCompartment(name, logger)
{
  m_Perfusion = nullptr;
  m_pH = nullptr;
  m_WaterVolumeFraction = nullptr;
}
SELiquidCompartment::~SELiquidCompartment()
{
  Clear();
}

void SELiquidCompartment::Clear()
{
  SEFluidCompartment::Clear();
  SAFE_DELETE(m_Perfusion);
  SAFE_DELETE(m_pH);
  SAFE_DELETE(m_WaterVolumeFraction);
  m_Children.clear();
}

const SEScalar* SELiquidCompartment::GetScalar(const std::string& name)
{
  const SEScalar* s = SEFluidCompartment::GetScalar(name);
  if (s != nullptr)
    return s;
  if (name.compare("Perfusion") == 0)
    return &GetPerfusion();
  if (name.compare("PH") == 0)
    return &GetPH(); 
  if (name.compare("WaterVolumeFraction") == 0)
    return &GetWaterVolumeFraction();
  return nullptr;
}

void SELiquidCompartment::StateChange()
{
  m_Leaves.clear();
  FindLeaves<SELiquidCompartment>(*this, m_Leaves);
  m_Nodes.StateChange();
}

void SELiquidCompartment::Balance(BalanceLiquidBy by)
{
  for (SELiquidSubstanceQuantity* subQ : m_SubstanceQuantities)
  {
    if (by == BalanceLiquidBy::PartialPressure && subQ->GetSubstance().GetState() != eSubstance_State::Gas)
      continue;

    //Partial pressures only make sense for gases in liquids
    if(HasVolume())
      subQ->Balance(by);
  }
}

bool SELiquidCompartment::HasPerfusion() const
{
  return m_Perfusion == nullptr ? false : m_Perfusion->IsValid();
}
SEScalarVolumePerTime& SELiquidCompartment::GetPerfusion()
{
  if (m_Perfusion == nullptr)
    m_Perfusion = new SEScalarVolumePerTime();
  if (!m_FluidChildren.empty())
  {
    double v = const_cast<const SELiquidCompartment*>(this)->GetPerfusion(VolumePerTimeUnit::mL_Per_s);
    m_Perfusion->SetReadOnly(false);
    m_Perfusion->SetValue(v, VolumePerTimeUnit::mL_Per_s);
    m_Perfusion->SetReadOnly(true);
  }
  return *m_Perfusion;
}
double SELiquidCompartment::GetPerfusion(const VolumePerTimeUnit& unit) const
{
  if (!m_Children.empty())
  {
    double Perfusion = 0;
    for (SELiquidCompartment* child : m_Children)
      Perfusion += child->GetPerfusion().GetValue(unit);
    return Perfusion;
  }
  if (m_Perfusion == nullptr)
    return SEScalar::dNaN();
  return m_Perfusion->GetValue(unit);
}

bool SELiquidCompartment::HasPH() const
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
SEScalar& SELiquidCompartment::GetPH()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  if (!m_FluidChildren.empty())
  {
    m_pH->SetReadOnly(false);
    m_pH->SetValue(const_cast<const SELiquidCompartment*>(this)->GetPH());
    m_pH->SetReadOnly(true);
  }
  return *m_pH;
}
double SELiquidCompartment::GetPH() const
{
  if (!m_Children.empty())
  {
    double pH = 0;
    for (SELiquidCompartment* child : m_Children)
      pH += pow(10, -child->GetPH().GetValue())*child->GetVolume(VolumeUnit::mL);
    pH = -log10(pH / GetVolume(VolumeUnit::mL));
    return pH;
  }
  if (m_pH == nullptr)
    return SEScalar::dNaN();
  return m_pH->GetValue();
}

bool SELiquidCompartment::HasWaterVolumeFraction() const
{
  return m_WaterVolumeFraction == nullptr ? false : m_WaterVolumeFraction->IsValid();
}
SEScalar0To1& SELiquidCompartment::GetWaterVolumeFraction()
{
  if (m_WaterVolumeFraction == nullptr)
    m_WaterVolumeFraction = new SEScalar0To1();
  if (!m_FluidChildren.empty())
  {
    m_WaterVolumeFraction->SetReadOnly(false);
    m_WaterVolumeFraction->SetValue(const_cast<const SELiquidCompartment*>(this)->GetWaterVolumeFraction());
    m_WaterVolumeFraction->SetReadOnly(true);
  }
  return *m_WaterVolumeFraction;
}
double SELiquidCompartment::GetWaterVolumeFraction() const
{
  if (!m_Children.empty())
  {
    double waterVolume_mL = 0;
    for (SELiquidCompartment* child : m_Children)
      waterVolume_mL += child->GetWaterVolumeFraction().GetValue()*child->GetVolume(VolumeUnit::mL);
    return waterVolume_mL / GetVolume(VolumeUnit::mL);
  }
  if (m_WaterVolumeFraction == nullptr)
    return SEScalar::dNaN();
  return m_WaterVolumeFraction->GetValue();
}

void SELiquidCompartment::AddChild(SELiquidCompartment& child)
{
  if (HasNodeMapping())
  {
    Fatal("You cannont add a child compartment to a compartment mapped to nodes");
    return;
  }
  if (HasChild(child))
    return;
  m_FluidChildren.push_back(&child);
  m_Children.push_back(&child);
  for (SELiquidSubstanceQuantity* subQ : m_SubstanceQuantities)
    subQ->AddChild(child.CreateSubstanceQuantity(subQ->GetSubstance()));
}

SELiquidSubstanceQuantity& SELiquidCompartment::CreateSubstanceQuantity(SESubstance& substance, bool zeroOut)
{
  SELiquidSubstanceQuantity* subQ = GetSubstanceQuantity(substance);
  if (subQ == nullptr)
  {
    subQ = new SELiquidSubstanceQuantity(substance, *this);
    if(zeroOut)
      subQ->SetToZero();
    m_SubstanceQuantities.push_back(subQ);
    m_TransportSubstances.push_back(subQ);
  }
  if (!m_FluidChildren.empty())
  {
    for (SELiquidCompartment* child : m_Children)
      subQ->AddChild(child->CreateSubstanceQuantity(substance));
  }
  return *subQ;
}

template class SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>;
