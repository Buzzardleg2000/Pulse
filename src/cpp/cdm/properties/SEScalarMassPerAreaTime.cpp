/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarMassPerAreaTime.h"

const MassPerAreaTimeUnit MassPerAreaTimeUnit::g_Per_cm2_s("g/cm^2 s");

bool MassPerAreaTimeUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_cm2_s.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerAreaTimeUnit& MassPerAreaTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_cm2_s.GetString().compare(unit) == 0)
    return g_Per_cm2_s;
  throw CommonDataModelException(unit + " is not a valid MassPerAreaTime unit");
}

template class SEScalarQuantity<MassPerAreaTimeUnit>;
