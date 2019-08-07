/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalar.h"
#include "utils/GeneralMath.h"

unsigned long long int SEScalar::NaN = 
  ((unsigned long long int)255 << (8*7)) + 
  ((unsigned long long int)(255-8) << (8*6)) + 
  ((unsigned long long int)255 << (8*5)) + 
  ((unsigned long long int)255 << (8*4)) +
  ((unsigned long long int)255 << (8*3)) + 
  (255 << (8*2)) + 
  (255 << (8*1)) + 
   255;

double SEScalar::dNaN()
{
  double d;
  *(reinterpret_cast<unsigned long long int *>(&d)) = NaN;
  return d;
}

const NoUnit NoUnit::unitless;

SEScalar::SEScalar() : SEProperty()
{  
  Clear();
}

SEScalar::~SEScalar()
{
  Clear();
}

void SEScalar::Clear()
{
  SEProperty::Clear();  
  m_readOnly = false;
  Invalidate();
}

bool SEScalar::Set(const SEScalar& s)
{
  if (dynamic_cast<const SEUnitScalar*>(&s) != nullptr)
    std::cerr << " HALT ";
  if(!s.IsValid())    
    return false;
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = s.m_value;
  m_isnan = (std::isnan(m_value)) ? true : false;
  m_isinf = (std::isinf(m_value)) ? true : false;  
  return true;
}

void SEScalar::Copy(const SEScalar& s)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = s.m_value;
  m_isnan = s.m_isnan;
  m_isinf = s.m_isinf;
}

void SEScalar::Invalidate()
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_isnan = true;
  m_isinf = false;
  *(reinterpret_cast<unsigned long long int *>(&m_value)) = NaN;
}

bool SEScalar::IsValid() const 
{
  if(m_isnan)
    return false;
  return true;
}

bool SEScalar::IsZero(double limit) const
{
  if (!IsValid())
    return false;
  return SEScalar::IsZero(m_value,limit);
}

bool SEScalar::IsPositive() const
{
  if (!IsValid())
    return false;
  return m_value>0;
}

bool SEScalar::IsNegative() const
{
  if (!IsValid())
    return false;
  return m_value<0;
}

void SEScalar::SetReadOnly(bool b)
{
  m_readOnly = b;
}
bool SEScalar::IsReadOnly() const
{
  return m_readOnly;
}

double SEScalar::GetValue() const
{
  if (!m_isnan)
    return m_value;
  throw CommonDataModelException("Value is NaN");
}

void SEScalar::SetValue(double d)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = d;
  m_isnan = (std::isnan(m_value)) ? true : false;
  m_isinf = (std::isinf(m_value)) ? true : false;
}
void SEScalar::ForceValue(double d)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = d;
  m_isnan = (std::isnan(m_value)) ? true : false;
  m_isinf = (std::isinf(m_value)) ? true : false;
}

double SEScalar::Increment(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    IncrementValue(s.GetValue());
  return m_value;
}

double SEScalar::IncrementValue(double d)
{
  if (!IsValid())
  {
    SetValue(d);
    return d;
  }
  SetValue(m_value+d);
  return m_value;
}

void SEScalar::Average(int cnt)
{
  if (!IsValid() || cnt == 0)
    return;
  m_value /= cnt;
}

bool SEScalar::Equals(const SEScalar& to) const
{
  if (m_isnan && to.m_isnan)
    return true;
  if (m_isnan || to.m_isnan)
    return false;
  if (m_isinf&&to.m_isinf)
    return true;
  if (m_isinf || to.m_isinf)
    return false;
  return std::abs(GeneralMath::PercentDifference(m_value, to.m_value)) <= 1e-15;
}

void SEScalar::ToString(std::ostream &str) const
{
  str<<m_value<<std::flush;
}

bool SEScalar::IsValue(double target, double value)
{
  if (value<(target + 1e-10) && value>(target - 1e-10))
    return true;
  return false;
}

bool SEScalar::IsZero(double d, double limit)
{
  if (d<limit&&d>-limit)
    return true;
  return false;
}

SEGenericScalar::SEGenericScalar(Logger* logger) : Loggable(logger)
{
  m_Scalar = nullptr;
  m_UnitScalar = nullptr;
}

bool SEGenericScalar::HasScalar()
{
  return m_Scalar != nullptr;
}
void SEGenericScalar::SetScalar(const SEScalar& s)
{
  m_Scalar = &s;
  m_UnitScalar = dynamic_cast<const SEUnitScalar*>(m_Scalar);
}

bool SEGenericScalar::IsValid()
{
  return m_UnitScalar == nullptr ? m_Scalar->IsValid() : m_UnitScalar->IsValid();
}

bool SEGenericScalar::IsInfinity()
{
  return m_Scalar->IsInfinity();
}

bool SEGenericScalar::HasUnit()
{
  return m_UnitScalar != nullptr && m_UnitScalar->HasUnit();
}
const CCompoundUnit* SEGenericScalar::GetUnit()
{
  if (m_UnitScalar == nullptr)
    return nullptr;
  return m_UnitScalar->GetUnit();
}
const CCompoundUnit* SEGenericScalar::GetCompoundUnit(const std::string& unit) const
{
  if (m_UnitScalar == nullptr)
    return nullptr;
  return m_UnitScalar->GetCompoundUnit(unit);
}
bool SEGenericScalar::IsValidUnit(const CCompoundUnit& unit) const
{
  if (m_UnitScalar == nullptr)
    return false;
  return false;
}

double SEGenericScalar::GetValue() const
{
  return m_Scalar->GetValue();
}
double SEGenericScalar::GetValue(const CCompoundUnit& unit) const
{
  if (m_UnitScalar == nullptr)
    return SEScalar::dNaN();
  return m_UnitScalar->GetValue(unit);
}

