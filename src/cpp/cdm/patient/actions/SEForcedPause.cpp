/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEForcedPause.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEForcedPause::SEForcedPause(Logger* logger) : SEConsciousRespirationCommand(logger)
{
  m_Period = nullptr;
}

SEForcedPause::~SEForcedPause()
{
  SAFE_DELETE(m_Period);
}

void SEForcedPause::Clear()
{
  SEConsciousRespirationCommand::Clear();
  INVALIDATE_PROPERTY(m_Period);
}

void SEForcedPause::Copy(const SEForcedPause& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedPause::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasPeriod();
}

bool SEForcedPause::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEForcedPause::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEForcedPause::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEForcedPause::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}
