/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SESepsisExacerbation.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SESepsisExacerbation::SESepsisExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_InfectionSeverity =nullptr;
  m_ProgressionSeverity = nullptr;
}

SESepsisExacerbation::~SESepsisExacerbation()
{
  SAFE_DELETE(m_InfectionSeverity);
  SAFE_DELETE(m_ProgressionSeverity);
}

void SESepsisExacerbation::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_InfectionSeverity);
  INVALIDATE_PROPERTY(m_ProgressionSeverity);
}

void SESepsisExacerbation::Copy(const SESepsisExacerbation& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SESepsisExacerbation::IsValid() const
{
  return HasInfectionSeverity() || HasProgressionSeverity();
}
bool SESepsisExacerbation::IsActive() const
{
  if (!IsValid())
    return false;
  if (HasInfectionSeverity() && GetInfectionSeverity() > 0)
    return true;
  if (HasProgressionSeverity() && GetProgressionSeverity() > 0)
    return true;
  return false;
}
void SESepsisExacerbation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SESepsisExacerbation::GetScalar(const std::string& name)
{
  if (name.compare("InfectionSeverity") == 0)
    return &GetInfectionSeverity();
  if (name.compare("ProgressionSeverity") == 0)
    return &GetProgressionSeverity();
  return nullptr;
}

bool SESepsisExacerbation::HasInfectionSeverity() const
{
  return m_InfectionSeverity==nullptr?false:m_InfectionSeverity->IsValid();
}

SEScalar0To1& SESepsisExacerbation::GetInfectionSeverity()
{
  if(m_InfectionSeverity==nullptr)
    m_InfectionSeverity=new SEScalar0To1();
  return *m_InfectionSeverity;
}
double SESepsisExacerbation::GetInfectionSeverity() const
{
  if (m_InfectionSeverity == nullptr)
    return SEScalar::dNaN();
  return m_InfectionSeverity->GetValue();
}

bool SESepsisExacerbation::HasProgressionSeverity() const
{
  return m_ProgressionSeverity == nullptr ? false : m_ProgressionSeverity->IsValid();
}

SEScalar0To1& SESepsisExacerbation::GetProgressionSeverity()
{
  if (m_ProgressionSeverity == nullptr)
    m_ProgressionSeverity = new SEScalar0To1();
  return *m_ProgressionSeverity;
}
double SESepsisExacerbation::GetProgressionSeverity() const
{
  if (m_ProgressionSeverity == nullptr)
    return SEScalar::dNaN();
  return m_ProgressionSeverity->GetValue();
}
