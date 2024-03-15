/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/physiology/SEImmuneSystem.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarAmount.h"

SEImmuneSystem::SEImmuneSystem(Logger* logger) : SESystem(logger)
{
  /*
  m_ActivatedPhagocytes = nullptr;
  m_AntiInflammatoryMediators = nullptr;
  m_PathogenCount = nullptr;
  m_TissueDamage = nullptr;
  */
}

SEImmuneSystem::~SEImmuneSystem()
{
  /*
  SAFE_DELETE(m_ActivatedPhagocytes);
  SAFE_DELETE(m_AntiInflammatoryMediators);
  SAFE_DELETE(m_PathogenCount);
  SAFE_DELETE(m_TissueDamage);
  */
}

void SEImmuneSystem::Clear()
{
  SESystem::Clear();
  /*
  INVALIDATE_PROPERTY(m_ActivatedPhagocytes);
  INVALIDATE_PROPERTY(m_AntiInflammatoryMediators);
  INVALIDATE_PROPERTY(m_PathogenCount);
  INVALIDATE_PROPERTY(m_TissueDamage);
  */
}

const SEScalar* SEImmuneSystem::GetScalar(const std::string& name)
{
  /*
  if (name.compare("ActivatedPhagocytes") == 0)
    return &GetActivatedPhagocytes();
  if (name.compare("AntiInflammatoryMediators") == 0)
    return &GetAntiInflammatoryMediators();
  if (name.compare("PathogenCount") == 0)
    return &GetPathogenCount();
  if (name.compare("TissueDamage") == 0)
    return &GetTissueDamage();
  */
  return nullptr;
}
/*
bool SEImmuneSystem::HasActivatedPhagocytes() const
{
  return m_ActivatedPhagocytes == nullptr ? false : m_ActivatedPhagocytes->IsValid();
}
SEScalarAmount& SEImmuneSystem::GetActivatedPhagocytes()
{
  if (m_ActivatedPhagocytes == nullptr)
    m_ActivatedPhagocytes = new SEScalarAmount();
  return *m_ActivatedPhagocytes;
}
double SEImmuneSystem::GetActivatedPhagocytes(const AmountUnit& unit) const
{
  if (m_ActivatedPhagocytes == nullptr)
    return SEScalar::dNaN();
  return m_ActivatedPhagocytes->GetValue(unit);
}

bool SEImmuneSystem::HasAntiInflammatoryMediators() const
{
  return m_AntiInflammatoryMediators == nullptr ? false : m_AntiInflammatoryMediators->IsValid();
}
SEScalarAmount& SEImmuneSystem::GetAntiInflammatoryMediators()
{
  if (m_AntiInflammatoryMediators == nullptr)
    m_AntiInflammatoryMediators = new SEScalarAmount();
  return *m_AntiInflammatoryMediators;
}
double SEImmuneSystem::GetAntiInflammatoryMediators(const AmountUnit& unit) const
{
  if (m_AntiInflammatoryMediators == nullptr)
    return SEScalar::dNaN();
  return m_AntiInflammatoryMediators->GetValue(unit);
}

bool SEImmuneSystem::HasPathogenCount() const
{
  return m_PathogenCount == nullptr ? false : m_PathogenCount->IsValid();
}
SEScalarAmount& SEImmuneSystem::GetPathogenCount()
{
  if (m_PathogenCount == nullptr)
    m_PathogenCount = new SEScalarAmount();
  return *m_PathogenCount;
}
double SEImmuneSystem::GetPathogenCount(const AmountUnit& unit) const
{
  if (m_PathogenCount == nullptr)
    return SEScalar::dNaN();
  return m_PathogenCount->GetValue(unit);
}

bool SEImmuneSystem::HasTissueDamage() const
{
  return m_TissueDamage == nullptr ? false : m_TissueDamage->IsValid();
}
SEScalar0To1& SEImmuneSystem::GetTissueDamage()
{
  if (m_TissueDamage == nullptr)
    m_TissueDamage = new SEScalar0To1();
  return *m_TissueDamage;
}
double SEImmuneSystem::GetTissueDamage() const
{
  if (m_TissueDamage == nullptr)
    return SEScalar::dNaN();
  return m_TissueDamage->GetValue();
}
*/
