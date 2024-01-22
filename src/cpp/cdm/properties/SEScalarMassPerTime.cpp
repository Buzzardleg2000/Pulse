/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarMassPerTime.h"

const MassPerTimeUnit MassPerTimeUnit::g_Per_s("g/s");
const MassPerTimeUnit MassPerTimeUnit::g_Per_min("g/min");
const MassPerTimeUnit MassPerTimeUnit::g_Per_day("g/day");
const MassPerTimeUnit MassPerTimeUnit::mg_Per_s("mg/s");
const MassPerTimeUnit MassPerTimeUnit::mg_Per_min("mg/min");
const MassPerTimeUnit MassPerTimeUnit::ug_Per_s("ug/s");
const MassPerTimeUnit MassPerTimeUnit::kg_Per_s("kg/s");
const MassPerTimeUnit MassPerTimeUnit::ug_Per_min("ug/min");

bool MassPerTimeUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_s.GetString().compare(unit) == 0)
    return true;
  if (g_Per_min.GetString().compare(unit) == 0)
    return true;
  if (g_Per_day.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_s.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_min.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_s.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_s.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_min.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerTimeUnit& MassPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_s.GetString().compare(unit) == 0)
    return g_Per_s;
  if (g_Per_min.GetString().compare(unit) == 0)
    return g_Per_min;
  if (g_Per_day.GetString().compare(unit) == 0)
    return g_Per_day;
  if (mg_Per_s.GetString().compare(unit) == 0)
    return mg_Per_s;
  if (mg_Per_min.GetString().compare(unit) == 0)
    return mg_Per_min;
  if (ug_Per_s.GetString().compare(unit) == 0)
    return ug_Per_s;
  if (kg_Per_s.GetString().compare(unit) == 0)
    return kg_Per_s;
  if (ug_Per_min.GetString().compare(unit) == 0)
    return ug_Per_min;
  throw CommonDataModelException(unit + " is not a valid MassPerTime unit");
}

template class SEScalarQuantity<MassPerTimeUnit>;
