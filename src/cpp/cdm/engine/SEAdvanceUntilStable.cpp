/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEAdvanceUntilStable.h"

SEAdvanceUntilStable::SEAdvanceUntilStable(Logger* logger) : SEAction(logger)
{
 
}

SEAdvanceUntilStable::~SEAdvanceUntilStable()
{

}

void SEAdvanceUntilStable::Clear()
{
  SEAction::Clear();
}

bool SEAdvanceUntilStable::IsValid() const
{ 
  return HasCriteria();
}

bool SEAdvanceUntilStable::HasCriteria() const
{
  return !(m_Criteria.empty());
}
std::string SEAdvanceUntilStable::GetCriteria() const
{
  return m_Criteria;
}
void SEAdvanceUntilStable::SetCriteria(const std::string& criteria)
{
  m_Criteria = criteria;
  std::replace(m_Criteria.begin(), m_Criteria.end(), '\\', '/');
}
void SEAdvanceUntilStable::InvalidateCriteria()
{
  m_Criteria = "";
}

const SEScalar* SEAdvanceUntilStable::GetScalar(const std::string& name)
{
  return nullptr;
}
