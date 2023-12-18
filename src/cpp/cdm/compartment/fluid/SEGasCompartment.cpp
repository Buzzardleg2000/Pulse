/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarArea.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/utils/GeneralMath.h"

SEGasCompartment::SEGasCompartment(const std::string& name, Logger* logger) : SEFluidCompartment(name, logger)
{
  m_DiffusionSurfaceArea = nullptr;
  m_Ventilation = nullptr;
  m_VentilationPerfusionRatio = nullptr;
}
SEGasCompartment::~SEGasCompartment()
{
  Clear();
}

void SEGasCompartment::Clear()
{
  SEFluidCompartment::Clear();
  SAFE_DELETE(m_DiffusionSurfaceArea);
  SAFE_DELETE(m_Ventilation);
  SAFE_DELETE(m_VentilationPerfusionRatio);
  m_Children.clear();
}

const SEScalar* SEGasCompartment::GetScalar(const std::string& name)
{
  const SEScalar* s = SEFluidCompartment::GetScalar(name);
  if (s != nullptr)
    return s;
  if (name.compare("DiffusionSurfaceArea") == 0)
    return &GetDiffusionSurfaceArea();
  if (name.compare("Ventilation") == 0)
    return &GetVentilation();
  if (name.compare("VentilationPerfusionRatio") == 0)
    return &GetVentilationPerfusionRatio();
  return nullptr;
}

void SEGasCompartment::StateChange()
{
  m_Leaves.clear();
  FindLeaves<SEGasCompartment>(*this, m_Leaves);
  m_Nodes.StateChange();
}

void SEGasCompartment::Balance(BalanceGasBy by)
{
  if (!m_FluidChildren.empty())
    Fatal("You cannot balance a quantity with children");
  switch (by)
  {
    case BalanceGasBy::Volume:
    {
    //Note: We won't modify the compartment volume, just the fractions
      double totalVolume_mL = 0;
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities())
      {
        if (subQ->HasVolume())
          totalVolume_mL += subQ->GetVolume(VolumeUnit::mL);
      }
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities())
      {
        if (!subQ->HasVolume())
        {
          subQ->Clear();
          continue;
        }
        else
        {
          subQ->GetVolumeFraction().SetValue(totalVolume_mL>0 ? subQ->GetVolume(VolumeUnit::mL) / totalVolume_mL : 0.0);
          subQ->GetVolume().SetValue(subQ->GetVolumeFraction().GetValue() * GetVolume(VolumeUnit::mL), VolumeUnit::mL);
          if(HasPressure())
            GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
        }
      }
      break;
    }
    case BalanceGasBy::VolumeFraction:
    {
      if (!HasVolume())
      {
        for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities())
        {
          subQ->Clear();
          if (HasPressure())
            GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
        }
        return;
      }
      if(GetVolume().IsInfinity())
      {
      for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities())
      {
        subQ->GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
        if (HasPressure())
          GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
      }
    }
      else
      {
        double totalFraction = 0;
        double totalVolume_mL = GetVolume(VolumeUnit::mL);
        for (SEGasSubstanceQuantity* subQ : GetSubstanceQuantities())
        {
          if (!subQ->HasVolumeFraction())
            subQ->Clear();
          else
          {
            totalFraction += subQ->GetVolumeFraction().GetValue();
            subQ->GetVolume().SetValue(subQ->GetVolumeFraction().GetValue() * totalVolume_mL, VolumeUnit::mL);
          }
          if (HasPressure())
            GeneralMath::CalculatePartialPressureInGas(subQ->GetVolumeFraction(), GetPressure(), subQ->GetPartialPressure(), m_Logger);
        }
        if (!SEScalar::IsZero(1-totalFraction, ZERO_APPROX))
          Fatal(GetName() + " Compartment's volume fractions do not sum up to 1");
      }
      break;
    }
  }
}

bool SEGasCompartment::HasDiffusionSurfaceArea() const
{
  return m_DiffusionSurfaceArea == nullptr ? false : m_DiffusionSurfaceArea->IsValid();
}
SEScalarArea& SEGasCompartment::GetDiffusionSurfaceArea()
{
  if (m_DiffusionSurfaceArea == nullptr)
    m_DiffusionSurfaceArea = new SEScalarArea();
  if (!m_FluidChildren.empty())
  {
    double v = const_cast<const SEGasCompartment*>(this)->GetDiffusionSurfaceArea(AreaUnit::cm2);
    m_DiffusionSurfaceArea->SetReadOnly(false);
    m_DiffusionSurfaceArea->SetValue(v, AreaUnit::cm2);
    m_DiffusionSurfaceArea->SetReadOnly(true);
  }
  return *m_DiffusionSurfaceArea;
}
double SEGasCompartment::GetDiffusionSurfaceArea(const AreaUnit& unit) const
{
  if (!m_Children.empty())
  {
    double DiffusionSurfaceArea = 0;
    for (SEGasCompartment* child : m_Children)
      DiffusionSurfaceArea += child->GetDiffusionSurfaceArea().GetValue(unit);
    return DiffusionSurfaceArea;
  }
  if (m_DiffusionSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_DiffusionSurfaceArea->GetValue(unit);
}

bool SEGasCompartment::HasVentilation() const
{
  return m_Ventilation == nullptr ? false : m_Ventilation->IsValid();
}
SEScalarVolumePerTime& SEGasCompartment::GetVentilation()
{
  if (m_Ventilation == nullptr)
    m_Ventilation = new SEScalarVolumePerTime();
  if (!m_FluidChildren.empty())
  {
    double v = const_cast<const SEGasCompartment*>(this)->GetVentilation(VolumePerTimeUnit::mL_Per_s);
    m_Ventilation->SetReadOnly(false);
    m_Ventilation->SetValue(v, VolumePerTimeUnit::mL_Per_s);
    m_Ventilation->SetReadOnly(true);
  }
  return *m_Ventilation;
}
double SEGasCompartment::GetVentilation(const VolumePerTimeUnit& unit) const
{
  if (!m_Children.empty())
  {
    double Ventilation = 0;
    for (SEGasCompartment* child : m_Children)
      Ventilation += child->GetVentilation().GetValue(unit);
    return Ventilation;
  }
  if (m_Ventilation == nullptr)
    return SEScalar::dNaN();
  return m_Ventilation->GetValue(unit);
}

bool SEGasCompartment::HasVentilationPerfusionRatio() const
{
  return m_VentilationPerfusionRatio == nullptr ? false : m_VentilationPerfusionRatio->IsValid();
}
SEScalar& SEGasCompartment::GetVentilationPerfusionRatio()
{
  if (m_VentilationPerfusionRatio == nullptr)
    m_VentilationPerfusionRatio = new SEScalarVolumePerTime();
  if (!m_FluidChildren.empty())
  {
    double v = const_cast<const SEGasCompartment*>(this)->GetVentilationPerfusionRatio();
    m_VentilationPerfusionRatio->SetReadOnly(false);
    m_VentilationPerfusionRatio->SetValue(v);
    m_VentilationPerfusionRatio->SetReadOnly(true);
  }
  return *m_VentilationPerfusionRatio;
}
double SEGasCompartment::GetVentilationPerfusionRatio() const
{
  if (!m_Children.empty())
  {
    double VentilationPerfusionRatio = 0;
    for (SEGasCompartment* child : m_Children)
      VentilationPerfusionRatio += child->GetVentilationPerfusionRatio().GetValue();
    return VentilationPerfusionRatio;
  }
  if (m_VentilationPerfusionRatio == nullptr)
    return SEScalar::dNaN();
  return m_VentilationPerfusionRatio->GetValue();
}

void SEGasCompartment::AddChild(SEGasCompartment& child)
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
  for (SEGasSubstanceQuantity* subQ : m_SubstanceQuantities)
    subQ->AddChild(child.CreateSubstanceQuantity(subQ->GetSubstance()));
}

SEGasSubstanceQuantity& SEGasCompartment::CreateSubstanceQuantity(SESubstance& substance, bool zeroOut)
{
  SEGasSubstanceQuantity* subQ = GetSubstanceQuantity(substance);
  if (subQ == nullptr)
  {
    subQ = new SEGasSubstanceQuantity(substance, *this);
    if(zeroOut)
      subQ->SetToZero();
    m_SubstanceQuantities.push_back(subQ);
    m_TransportSubstances.push_back(subQ);
  }
  if (!m_FluidChildren.empty())
  {
    for (SEGasCompartment* child : m_Children)
      subQ->AddChild(child->CreateSubstanceQuantity(substance));
  }
  return *subQ;
}

template class SEFluidCompartment<SEGasCompartmentLink, SEGasTransportVertex, SEGasTransportSubstance, SEGasSubstanceQuantity>;
