/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"

#include "cdm/compartment/substances/SELiquidSubstanceQuantity.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartmentLink.h"
#include "cdm/substance/SESubstance.h"

#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarInversePressure.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarMassPerAmount.h"
#include "cdm/utils/GeneralMath.h"

SELiquidSubstanceQuantity::SELiquidSubstanceQuantity(SESubstance& sub, SELiquidCompartment& compartment) : SESubstanceQuantity(sub), m_Compartment(compartment)
{
  m_Concentration = nullptr;
  m_Mass = nullptr;
  m_MassCleared = nullptr;
  m_MassDeposited = nullptr;
  m_MassExcreted = nullptr;
  m_Molarity = nullptr;
  m_PartialPressure = nullptr;
  m_Saturation = nullptr;

  m_Hb = nullptr;
  m_HbO2 = nullptr;
  m_HbCO2 = nullptr;
  m_HbO2CO2 = nullptr;
  m_HbCO = nullptr;

  m_isO2 = false;
  m_isCO2 = false;
  m_isCO = false;
  if (sub.GetName() == "Oxygen")
    m_isO2 = true;
  else if (sub.GetName() == "CarbonMonoxide")
    m_isCO = true;
  else if (sub.GetName() == "CarbonDioxide")
    m_isCO2 = true;

  if (m_Substance.GetState() != eSubstance_State::Gas)
    GetPartialPressure().SetReadOnly(true);// Cannot have a partial pressure of a non gas
}

SELiquidSubstanceQuantity::~SELiquidSubstanceQuantity()
{
  SAFE_DELETE(m_Concentration);
  SAFE_DELETE(m_Mass);
  SAFE_DELETE(m_MassCleared);
  SAFE_DELETE(m_MassDeposited);
  SAFE_DELETE(m_MassExcreted);
  SAFE_DELETE(m_Molarity);
  SAFE_DELETE(m_PartialPressure);
  SAFE_DELETE(m_Saturation);
}

void SELiquidSubstanceQuantity::Clear()
{
  FORCE_INVALIDATE_PROPERTY(m_Concentration);
  FORCE_INVALIDATE_PROPERTY(m_Mass);
  FORCE_INVALIDATE_PROPERTY(m_MassCleared);
  FORCE_INVALIDATE_PROPERTY(m_MassDeposited);
  FORCE_INVALIDATE_PROPERTY(m_MassExcreted);
  FORCE_INVALIDATE_PROPERTY(m_Molarity);
  FORCE_INVALIDATE_PROPERTY(m_PartialPressure);
  FORCE_INVALIDATE_PROPERTY(m_Saturation);
  m_Children.clear();
}

std::string SELiquidSubstanceQuantity::GetCompartmentName()
{
  return m_Compartment.GetName();
}

void SELiquidSubstanceQuantity::SetToZero()
{
  auto& c = GetConcentration();
  if (c.HasUnit())
    c.SetValue(0, *c.GetUnit());
  else
    c.SetValue(0, MassPerVolumeUnit::mg_Per_mL);

  auto& m = GetMass();
  if (m.HasUnit())
    m.SetValue(0, *m.GetUnit());
  else
    m.SetValue(0, MassUnit::mg);

  auto& mol = GetMolarity();
  if (mol.HasUnit())
    mol.SetValue(0, *mol.GetUnit());
  else
    mol.SetValue(0, AmountPerVolumeUnit::mmol_Per_mL);

  if (m_Substance.GetState() == eSubstance_State::Gas)
  {
    auto& pp = GetPartialPressure();
    if (pp.HasUnit())
      pp.SetValue(0, *pp.GetUnit());
    else
      pp.SetValue(0, PressureUnit::mmHg);
  }

  if (m_isO2 || m_isCO || m_isCO2)
    GetSaturation().SetValue(0);

  auto& mc = GetMassCleared();
  if (mc.HasUnit())
    mc.SetValue(0, *mc.GetUnit());
  else
    mc.SetValue(0, MassUnit::mg);

  auto& md = GetMassDeposited();
  if (md.HasUnit())
    md.SetValue(0, *md.GetUnit());
  else
    md.SetValue(0, MassUnit::mg);

  auto& me = GetMassExcreted();
  if (me.HasUnit())
    me.SetValue(0, *me.GetUnit());
  else
    me.SetValue(0, MassUnit::mg);
}

const SEScalar* SELiquidSubstanceQuantity::GetScalar(const std::string& name)
{
  if (name.compare("Concentration") == 0)
    return &GetConcentration();
  if (name.compare("Mass") == 0)
    return &GetMass();
  if (name.compare("MassCleared") == 0)
    return &GetMassCleared();
  if (name.compare("MassDeposited") == 0)
    return &GetMassDeposited();
  if (name.compare("MassExcreted") == 0)
    return &GetMassExcreted();
  if (name.compare("Molarity") == 0)
    return &GetMolarity();
  if (name.compare("PartialPressure") == 0)
    return &GetPartialPressure();
  if (name.compare("Saturation") == 0)
    return &GetSaturation();
  return nullptr;
}

void SELiquidSubstanceQuantity::Balance(BalanceLiquidBy by)
{  
  SEScalarVolume& volume = m_Compartment.GetVolume();
  if (!volume.IsValid())
  {
    Clear();
    return;
  }  
  if (!m_Children.empty())
  {
    for (SELiquidSubstanceQuantity* child : m_Children)
      child->Balance(by);
    return;
  }
  switch (by)
  {
    case BalanceLiquidBy::Concentration:
    {
      if (!GetConcentration().IsValid() || GetConcentration().IsInfinity())
        Fatal("Cannot Balance by Concentration if it's invalid or set to Infinity");
      if (!volume.IsValid() || volume.IsInfinity())
        GetMass().SetValue(std::numeric_limits<double>::infinity(), MassUnit::ug);
      else
        if(!GeneralMath::CalculateMass(volume, GetConcentration(), GetMass(), m_Logger))
          Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      if (m_Substance.GetState() == eSubstance_State::Gas)
        if(!GeneralMath::CalculatePartialPressureInLiquid(m_Substance, GetConcentration(), GetPartialPressure(), m_Logger))
          Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      double molarity_mmol_Per_mL = GetMass(MassUnit::ug) / m_Substance.GetMolarMass(MassPerAmountUnit::ug_Per_mmol) / volume.GetValue(VolumeUnit::mL);
      GetMolarity().SetValue(molarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
      break;
    }
    case BalanceLiquidBy::Mass:
    {
      if (!volume.IsValid() || volume.IsInfinity() || !GetMass().IsValid() ||GetMass().IsInfinity())
        Fatal("Cannot balance by Mass if volume or mass is invalid or set to Infinity");
      if(!GeneralMath::CalculateConcentration(GetMass(),volume,GetConcentration(), m_Logger))
        Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      if (m_Substance.GetState() == eSubstance_State::Gas)
        if(!GeneralMath::CalculatePartialPressureInLiquid(m_Substance, GetConcentration(), GetPartialPressure(), m_Logger))
          Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      double molarity_mmol_Per_mL = GetMass(MassUnit::ug) / m_Substance.GetMolarMass(MassPerAmountUnit::ug_Per_mmol) / volume.GetValue(VolumeUnit::mL);
      GetMolarity().SetValue(molarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
      break;
    }
    case BalanceLiquidBy::Molarity:
    {
      if (!volume.IsValid() || volume.IsInfinity() || !GetMolarity().IsValid() || GetMolarity().IsInfinity())
        Fatal("Cannot balance by Molarity if volume or molarity is invalid or set to Infinity");
      double mass_ug = GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) *  m_Substance.GetMolarMass(MassPerAmountUnit::ug_Per_mmol) * volume.GetValue(VolumeUnit::mL);
      GetMass().SetValue(mass_ug, MassUnit::ug);
      if(!GeneralMath::CalculateConcentration(GetMass(), volume, GetConcentration(), m_Logger))
        Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      if (m_Substance.GetState() == eSubstance_State::Gas)
        if(!GeneralMath::CalculatePartialPressureInLiquid(m_Substance, GetConcentration(), GetPartialPressure(), m_Logger))
          Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      break;
    }
    case BalanceLiquidBy::PartialPressure:
    {
    if (m_Substance.GetState() != eSubstance_State::Gas)
      Fatal("Cannot balance by Partial Pressure if substance is not a gas");
    if (!volume.IsValid() || volume.IsInfinity() || !GetPartialPressure().IsValid() || GetPartialPressure().IsInfinity())
        Fatal("Cannot balance by Partial Pressure if volume or partial pressure is invalid or set to Infinity");
      double partialPressure_atm = GetPartialPressure(PressureUnit::atm);
      double density_ug_Per_mL = m_Substance.GetDensity(MassPerVolumeUnit::ug_Per_mL);
      double mass_ug = partialPressure_atm * m_Substance.GetSolubilityCoefficient(InversePressureUnit::Inverse_atm) * density_ug_Per_mL * volume.GetValue(VolumeUnit::mL);
      GetMass().SetValue(mass_ug, MassUnit::ug);
      if(!GeneralMath::CalculateConcentration(GetMass(), volume, GetConcentration(), m_Logger))
        Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
      double molarity_mmol_Per_mL = GetMass(MassUnit::ug) / m_Substance.GetMolarMass(MassPerAmountUnit::ug_Per_mmol) / volume.GetValue(VolumeUnit::mL);
      GetMolarity().SetValue(molarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
      break;
    }    
  }
  // Note we do not set saturation, that is done by the acid/base binding after transport
}

bool SELiquidSubstanceQuantity::HasConcentration() const
{
  if (!m_Children.empty())
  {
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasConcentration())
        return true;
    return false;
  }
  return (m_Concentration == nullptr) ? false : m_Concentration->IsValid();
}
SEScalarMassPerVolume& SELiquidSubstanceQuantity::GetConcentration()
{
  if (m_Concentration == nullptr)
    m_Concentration = new SEScalarMassPerVolume();
  if (!m_Children.empty())
  {
    m_Concentration->SetReadOnly(false);
    if (HasMass() && m_Compartment.HasVolume())
      m_Concentration->SetValue(GetMass(MassUnit::mg) / m_Compartment.GetVolume(VolumeUnit::mL), MassPerVolumeUnit::mg_Per_mL);
    else
      m_Concentration->Invalidate();
    m_Concentration->SetReadOnly(true);
  }
  return *m_Concentration;
}
double SELiquidSubstanceQuantity::GetConcentration(const MassPerVolumeUnit& unit) const
{
  if (!m_Children.empty())
  {
    if (!HasMass() || !m_Compartment.HasVolume())
      return SEScalar::dNaN();
    return Convert(GetMass(MassUnit::mg) / m_Compartment.GetVolume(VolumeUnit::mL), MassPerVolumeUnit::mg_Per_mL, unit);
  }
  if (m_Concentration == nullptr)
    return SEScalar::dNaN();
  return m_Concentration->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasMass() const
{
  if (!m_Children.empty())
  {
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasMass())
        return true;
    return false;
  }
  return (m_Mass == nullptr) ? false : m_Mass->IsValid();
}
SEScalarMass& SELiquidSubstanceQuantity::GetMass()
{
  if (m_Mass == nullptr)
    m_Mass = new SEScalarMass();
  if (!m_Children.empty())
  {
    m_Mass->SetReadOnly(false);
    m_Mass->Invalidate();
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasMass())
        m_Mass->Increment(child->GetMass());
    m_Mass->SetReadOnly(true);
  }
  return *m_Mass;
}
double SELiquidSubstanceQuantity::GetMass(const MassUnit& unit) const
{
  if (!m_Children.empty())
  {
    double mass = 0;
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasMass())
        mass += child->GetMass(unit);
    return mass;
  }
  if (m_Mass == nullptr)
    return SEScalar::dNaN();
  return m_Mass->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasMassCleared() const
{
  return (m_MassCleared == nullptr) ? false : m_MassCleared->IsValid();
}
SEScalarMass& SELiquidSubstanceQuantity::GetMassCleared()
{
  if (m_MassCleared == nullptr)
    m_MassCleared = new SEScalarMass();
  return *m_MassCleared;
}
double SELiquidSubstanceQuantity::GetMassCleared(const MassUnit& unit) const
{
  if (m_MassCleared == nullptr)
    return SEScalar::dNaN();
  return m_MassCleared->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasMassDeposited() const
{
  return (m_MassDeposited == nullptr) ? false : m_MassDeposited->IsValid();
}
SEScalarMass& SELiquidSubstanceQuantity::GetMassDeposited()
{
  if (m_MassDeposited == nullptr)
    m_MassDeposited = new SEScalarMass();
  return *m_MassDeposited;
}
double SELiquidSubstanceQuantity::GetMassDeposited(const MassUnit& unit) const
{
  if (m_MassDeposited == nullptr)
    return SEScalar::dNaN();
  return m_MassDeposited->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasMassExcreted() const
{
  return (m_MassExcreted == nullptr) ? false : m_MassCleared->IsValid();
}
SEScalarMass& SELiquidSubstanceQuantity::GetMassExcreted()
{
  if (m_MassExcreted == nullptr)
    m_MassExcreted = new SEScalarMass();
  return *m_MassExcreted;
}
double SELiquidSubstanceQuantity::GetMassExcreted(const MassUnit& unit) const
{
  if (m_MassExcreted == nullptr)
    return SEScalar::dNaN();
  return m_MassExcreted->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasMolarity() const
{
  if (!m_Children.empty())
  {
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasMolarity())
        return true;
    return false;
  }
  return (m_Molarity == nullptr) ? false : m_Molarity->IsValid();
}
SEScalarAmountPerVolume& SELiquidSubstanceQuantity::GetMolarity()
{
  if (m_Molarity == nullptr)
    m_Molarity = new SEScalarAmountPerVolume();
  if (!m_Children.empty())
  {
    m_Molarity->SetReadOnly(false);
    if (!HasMass() || !m_Compartment.HasVolume())
      m_Molarity->Invalidate();
    else
    {
      double molarity_mmol_Per_mL = GetMass(MassUnit::mg) / m_Substance.GetMolarMass(MassPerAmountUnit::mg_Per_mmol) / m_Compartment.GetVolume(VolumeUnit::mL);
      m_Molarity->SetValue(molarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    }
    m_Molarity->SetReadOnly(true);
  }
  return *m_Molarity;
}
double SELiquidSubstanceQuantity::GetMolarity(const AmountPerVolumeUnit& unit) const
{
  if (!m_Children.empty())
  {
    if (!HasMass() || !m_Compartment.HasVolume())
      return SEScalar::dNaN();
    double molarity_mmol_Per_mL = GetMass(MassUnit::mg) / m_Substance.GetMolarMass(MassPerAmountUnit::mg_Per_mmol) / m_Compartment.GetVolume(VolumeUnit::mL);
    return Convert(molarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL, unit);
  }
  if (m_Molarity == nullptr)
    return SEScalar::dNaN();
  return m_Molarity->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasPartialPressure() const
{
  if (!m_Children.empty())
  {
    for (SELiquidSubstanceQuantity* child : m_Children)
      if (child->HasPartialPressure())
        return true;
    return false;
  }
  return (m_PartialPressure == nullptr) ? false : m_PartialPressure->IsValid();
}
SEScalarPressure& SELiquidSubstanceQuantity::GetPartialPressure()
{
  if (m_PartialPressure == nullptr)
    m_PartialPressure = new SEScalarPressure();
  if (!m_Children.empty())
  {
    m_PartialPressure->SetReadOnly(false);
    if (HasConcentration())
    {
      if (!GeneralMath::CalculatePartialPressureInLiquid(m_Substance, GetConcentration(), *m_PartialPressure, m_Logger))
        Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
    }
    else
      m_PartialPressure->Invalidate();
    m_PartialPressure->SetReadOnly(true);
  }
  return *m_PartialPressure;
}
double SELiquidSubstanceQuantity::GetPartialPressure(const PressureUnit& unit) const
{
  if (!m_Children.empty())
  {
    if (!HasConcentration())
      return SEScalar::dNaN();
    SEScalarMassPerVolume concentration;
    SEScalarPressure      partialPressure;
    concentration.SetValue(GetConcentration(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
    if(!GeneralMath::CalculatePartialPressureInLiquid(m_Substance, concentration, partialPressure))
      Error("  Compartment : " + m_Compartment.GetName() + ", Substance : " + m_Substance.GetName());
    return partialPressure.GetValue(unit);
  }
  if (m_PartialPressure == nullptr)
    return SEScalar::dNaN();
  return m_PartialPressure->GetValue(unit);
}

bool SELiquidSubstanceQuantity::HasSaturation() const
{
  if (!m_isO2 && !m_isCO2 && !m_isCO)
    return false;

  if (m_Hb == nullptr ||
      m_HbO2 == nullptr ||
      m_HbCO2 == nullptr ||
      m_HbO2CO2 == nullptr)
  {
    Error("Must have Hemoglobins in the engine to get Saturation values");
    return false;
  }
  if (m_isCO && m_HbCO == nullptr)
  {
    Error("Must have HbCO in the engine to get CO Saturation values");
    return false;
  }

  if (!m_Children.empty())
  {
    SELiquidSubstanceQuantity* subQ;
    for (SELiquidCompartment* cmpt : m_Compartment.m_Children)
    {
      if(!cmpt->HasVolume())
        return false;
      subQ = cmpt->GetSubstanceQuantity(*m_Hb);
      if (subQ == nullptr)
        return false;
      if (!subQ->HasMolarity())
        return false;
      subQ = cmpt->GetSubstanceQuantity(*m_HbO2);
      if (subQ == nullptr)
        return false;
      if (!subQ->HasMolarity())
        return false;
      subQ = cmpt->GetSubstanceQuantity(*m_HbCO2);
      if (subQ == nullptr)
        return false;
      if (!subQ->HasMolarity())
        return false;
      subQ = cmpt->GetSubstanceQuantity(*m_HbO2CO2);
      if (subQ == nullptr)
        return false;
      if (m_isCO)
      {
        subQ = cmpt->GetSubstanceQuantity(*m_HbCO);
        if (subQ == nullptr)
          return false;
      }
      if (!subQ->HasMolarity())
        return false;
    }
    return true;
  }
  return (m_Saturation == nullptr) ? false : m_Saturation->IsValid();
}
SEScalar0To1& SELiquidSubstanceQuantity::GetSaturation()
{
  if (!m_isO2 && !m_isCO2 && !m_isCO)
    Error("Only Oxygen, CarbonMonoxide, and CarbonDioxide have Saturation");
  if (m_Saturation == nullptr)
    m_Saturation = new SEScalar0To1();
  if (!m_Children.empty())
  {
    m_Saturation->SetReadOnly(false);
    m_Saturation->SetValue(const_cast<const SELiquidSubstanceQuantity*>(this)->GetSaturation());
    m_Saturation->SetReadOnly(true);
  }
  return *m_Saturation;
}
double SELiquidSubstanceQuantity::GetSaturation() const
{
  if (!m_isO2 && !m_isCO2 && !m_isCO)
  {
    Error("Only Oxygen, CarbonMonoxide, and CarbonDioxide have Saturation");
    return SEScalar::dNaN();
  }
  if (!m_Children.empty())
  {
    if (!HasSaturation())
      return SEScalar::dNaN();
    double Hb_mmol = 0;
    double HbO2_mmol = 0;
    double HbCO2_mmol = 0;
    double HbO2CO2_mmol = 0;
    double HbCO_mmol = 0;
    for (SELiquidCompartment* cmpt : m_Compartment.m_Children)
    {
      if (!cmpt->HasVolume())
        continue;
      // Could speed this up by looping the cmpt subQ's and doing if checks against its sub
      double volume_mL = cmpt->GetVolume(VolumeUnit::mL);
      Hb_mmol += cmpt->GetSubstanceQuantity(*m_Hb)->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL)*volume_mL;
      HbO2_mmol += cmpt->GetSubstanceQuantity(*m_HbO2)->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL)*volume_mL;
      HbCO2_mmol += cmpt->GetSubstanceQuantity(*m_HbCO2)->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL)*volume_mL;
      HbO2CO2_mmol += cmpt->GetSubstanceQuantity(*m_HbO2CO2)->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL)*volume_mL;

      SELiquidSubstanceQuantity* CO_subQ = cmpt->GetSubstanceQuantity(*m_HbCO);
      if (CO_subQ != nullptr)
        HbCO_mmol += CO_subQ->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL)*volume_mL;
    }
    if (m_isO2)
      return (HbO2_mmol + HbO2CO2_mmol)  / (Hb_mmol + HbO2_mmol + HbCO2_mmol + HbO2CO2_mmol + HbCO_mmol);
    else if (m_isCO2)
      return (HbCO2_mmol + HbO2CO2_mmol) / (Hb_mmol + HbO2_mmol + HbCO2_mmol + HbO2CO2_mmol + HbCO_mmol);
    else if (m_isCO)
      return HbCO_mmol / (Hb_mmol + HbO2_mmol + HbCO2_mmol + HbO2CO2_mmol + HbCO_mmol);
  }
  if (m_Saturation == nullptr)
    return SEScalar::dNaN();
  return m_Saturation->GetValue();
}
void SELiquidSubstanceQuantity::SetHemoglobins(const SESubstance& Hb, const SESubstance& HbO2, const SESubstance& HbCO2, const SESubstance& HbO2CO2, const SESubstance& HbCO)
{
  if (!m_isO2 && !m_isCO2 && !m_isCO)
    Error("Only Oxygen, CarbonDioxide, and CarbonMonoxide have Saturation, why are you setting Hemoglobins?");
  m_Hb      = &Hb;
  m_HbO2    = &HbO2;
  m_HbCO2   = &HbCO2;
  m_HbO2CO2 = &HbO2CO2;
  m_HbCO    = &HbCO;
  for (SELiquidSubstanceQuantity* child : m_Children)
    child->SetHemoglobins(Hb,HbO2,HbCO2,HbO2CO2,HbCO);
}

void SELiquidSubstanceQuantity::AddChild(SELiquidSubstanceQuantity& subQ)
{
  if (!Contains(m_Children, subQ))
    m_Children.push_back(&subQ);
}