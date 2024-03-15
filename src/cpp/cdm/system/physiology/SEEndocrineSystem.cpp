/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/physiology/SEEndocrineSystem.h"
#include "cdm/properties/SEScalarAmountPerTime.h"

SEEndocrineSystem::SEEndocrineSystem(Logger* logger) : SESystem(logger)
{
  m_InsulinSynthesisRate = nullptr;
}

SEEndocrineSystem::~SEEndocrineSystem()
{
  SAFE_DELETE(m_InsulinSynthesisRate);
}

void SEEndocrineSystem::Clear()
{
  SESystem::Clear();
  INVALIDATE_PROPERTY(m_InsulinSynthesisRate);
}

const SEScalar* SEEndocrineSystem::GetScalar(const std::string& name)
{
  if (name.compare("InsulinSynthesisRate") == 0)
    return &GetInsulinSynthesisRate();
  return nullptr;
}

bool SEEndocrineSystem::HasInsulinSynthesisRate() const
{
  return m_InsulinSynthesisRate == nullptr ? false : m_InsulinSynthesisRate->IsValid();
}
SEScalarAmountPerTime& SEEndocrineSystem::GetInsulinSynthesisRate()
{
  if (m_InsulinSynthesisRate == nullptr)
    m_InsulinSynthesisRate = new SEScalarAmountPerTime();
  return *m_InsulinSynthesisRate;
}
double SEEndocrineSystem::GetInsulinSynthesisRate(const AmountPerTimeUnit& unit) const
{
  if (m_InsulinSynthesisRate == nullptr)
    return SEScalar::dNaN();
  return m_InsulinSynthesisRate->GetValue(unit);
}
