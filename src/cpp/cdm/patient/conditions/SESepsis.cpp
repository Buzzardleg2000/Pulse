/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SESepsis.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SESepsis::SESepsis(Logger* logger) : SEPatientCondition(logger)
{
  m_Severity=nullptr;
}

SESepsis::~SESepsis()
{
  SAFE_DELETE(m_Severity);
}

void SESepsis::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SESepsis::Copy(const SESepsis& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SESepsis::IsValid() const
{
  return HasSeverity();
}
bool SESepsis::IsActive() const
{
  if (!IsValid())
    return false;
  return GetSeverity() > 0;
}

bool SESepsis::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}

SEScalar0To1& SESepsis::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SESepsis::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}
