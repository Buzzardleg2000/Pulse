/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEBagValveMaskSqueeze::SEBagValveMaskSqueeze(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_SqueezePressure = nullptr;
  m_SqueezeVolume = nullptr;
  m_ExpiratoryPeriod = nullptr;
  m_InspiratoryPeriod = nullptr;
}

SEBagValveMaskSqueeze::~SEBagValveMaskSqueeze()
{
  SAFE_DELETE(m_SqueezePressure);
  SAFE_DELETE(m_SqueezeVolume);
  SAFE_DELETE(m_ExpiratoryPeriod);
  SAFE_DELETE(m_InspiratoryPeriod);
}

void SEBagValveMaskSqueeze::Clear()
{
  SEBagValveMaskAction::Clear();
  INVALIDATE_PROPERTY(m_SqueezePressure);
  INVALIDATE_PROPERTY(m_SqueezeVolume);
  INVALIDATE_PROPERTY(m_ExpiratoryPeriod);
  INVALIDATE_PROPERTY(m_InspiratoryPeriod);
}

void SEBagValveMaskSqueeze::Copy(const SEBagValveMaskSqueeze& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEBagValveMaskSqueeze::IsValid() const
{
  return SEBagValveMaskAction::IsValid() &&
    (HasSqueezePressure() || HasSqueezeVolume()) &&
    HasExpiratoryPeriod() && HasInspiratoryPeriod();
}

bool SEBagValveMaskSqueeze::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskSqueeze::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskSqueeze::GetScalar(const std::string& name)
{
  if (name == "SqueezePressure")
    return &GetSqueezePressure();
  if (name == "SqueezeVolume")
    return &GetSqueezeVolume();
  if (name == "ExpiratoryPeriod")
    return &GetExpiratoryPeriod();
  if (name == "InspiratoryPeriod")
    return &GetInspiratoryPeriod();
  return nullptr;
}

bool SEBagValveMaskSqueeze::HasSqueezePressure() const
{
  return m_SqueezePressure == nullptr ? false : m_SqueezePressure->IsValid();
}
SEScalarPressure& SEBagValveMaskSqueeze::GetSqueezePressure()
{
  if (m_SqueezePressure == nullptr)
    m_SqueezePressure = new SEScalarPressure();
  return *m_SqueezePressure;
}
double SEBagValveMaskSqueeze::GetSqueezePressure(const PressureUnit& unit) const
{
  if (m_SqueezePressure == nullptr)
    return SEScalar::dNaN();
  return m_SqueezePressure->GetValue(unit);
}

bool SEBagValveMaskSqueeze::HasSqueezeVolume() const
{
  return m_SqueezeVolume == nullptr ? false : m_SqueezeVolume->IsValid();
}
SEScalarVolume& SEBagValveMaskSqueeze::GetSqueezeVolume()
{
  if (m_SqueezeVolume == nullptr)
    m_SqueezeVolume = new SEScalarVolume();
  return *m_SqueezeVolume;
}
double SEBagValveMaskSqueeze::GetSqueezeVolume(const VolumeUnit& unit) const
{
  if (m_SqueezeVolume == nullptr)
    return SEScalar::dNaN();
  return m_SqueezeVolume->GetValue(unit);
}

bool SEBagValveMaskSqueeze::HasExpiratoryPeriod() const
{
  return m_ExpiratoryPeriod == nullptr ? false : m_ExpiratoryPeriod->IsValid();
}
SEScalarTime& SEBagValveMaskSqueeze::GetExpiratoryPeriod()
{
  if (m_ExpiratoryPeriod == nullptr)
    m_ExpiratoryPeriod = new SEScalarTime();
  return *m_ExpiratoryPeriod;
}
double SEBagValveMaskSqueeze::GetExpiratoryPeriod(const TimeUnit& unit) const
{
  if (m_ExpiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryPeriod->GetValue(unit);
}

bool SEBagValveMaskSqueeze::HasInspiratoryPeriod() const
{
  return m_InspiratoryPeriod == nullptr ? false : m_InspiratoryPeriod->IsValid();
}
SEScalarTime& SEBagValveMaskSqueeze::GetInspiratoryPeriod()
{
  if (m_InspiratoryPeriod == nullptr)
    m_InspiratoryPeriod = new SEScalarTime();
  return *m_InspiratoryPeriod;
}
double SEBagValveMaskSqueeze::GetInspiratoryPeriod(const TimeUnit& unit) const
{
  if (m_InspiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryPeriod->GetValue(unit);
}

void SEBagValveMaskSqueeze::ToString(std::ostream &str) const
{
  str << "Mechanical Ventilator Squeeze";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSqueezePressure: ";HasSqueezePressure() ? str << *m_SqueezePressure : str << "NaN";
  str << "\n\tSqueezeVolume: "; HasSqueezeVolume() ? str << *m_SqueezeVolume : str << "NaN";
  str << "\n\tExpiratoryPeriod: "; HasExpiratoryPeriod() ? str << *m_ExpiratoryPeriod : str << "NaN";
  str << "\n\tInspiratoryPeriod: "; HasInspiratoryPeriod() ? str << *m_InspiratoryPeriod : str << "NaN";
  str << std::flush;
}
