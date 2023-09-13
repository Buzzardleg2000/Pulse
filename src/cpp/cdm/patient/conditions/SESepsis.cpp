/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SESepsis.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SESepsis::SESepsis(Logger* logger) : SEPatientCondition(logger)
{
  m_InfectionSeverity = nullptr;
  m_ProgressionSeverity = nullptr;
}

SESepsis::~SESepsis()
{
  SAFE_DELETE(m_InfectionSeverity);
  SAFE_DELETE(m_ProgressionSeverity);
}

void SESepsis::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_InfectionSeverity);
  INVALIDATE_PROPERTY(m_ProgressionSeverity);
}

void SESepsis::Copy(const SESepsis& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SESepsis::IsValid() const
{
  return HasInfectionSeverity() || HasProgressionSeverity();
}
bool SESepsis::IsActive() const
{
  if (!IsValid())
    return false;
  if (HasInfectionSeverity() && GetInfectionSeverity() > 0)
    return true;
  if (HasProgressionSeverity() && GetProgressionSeverity() > 0)
    return true;
  return false;
}

bool SESepsis::HasInfectionSeverity() const
{
  return m_InfectionSeverity == nullptr ? false : m_InfectionSeverity->IsValid();
}

SEScalar0To1& SESepsis::GetInfectionSeverity()
{
  if (m_InfectionSeverity == nullptr)
    m_InfectionSeverity = new SEScalar0To1();
  return *m_InfectionSeverity;
}
double SESepsis::GetInfectionSeverity() const
{
  if (m_InfectionSeverity == nullptr)
    return SEScalar::dNaN();
  return m_InfectionSeverity->GetValue();
}

bool SESepsis::HasProgressionSeverity() const
{
  return m_ProgressionSeverity == nullptr ? false : m_ProgressionSeverity->IsValid();
}

SEScalar0To1& SESepsis::GetProgressionSeverity()
{
  if (m_ProgressionSeverity == nullptr)
    m_ProgressionSeverity = new SEScalar0To1();
  return *m_ProgressionSeverity;
}
double SESepsis::GetProgressionSeverity() const
{
  if (m_ProgressionSeverity == nullptr)
    return SEScalar::dNaN();
  return m_ProgressionSeverity->GetValue();
}
