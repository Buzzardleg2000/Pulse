/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPower.h"

const PowerUnit PowerUnit::W("W");
const PowerUnit PowerUnit::kcal_Per_s("kcal/s");
const PowerUnit PowerUnit::kcal_Per_hr("kcal/hr");
const PowerUnit PowerUnit::kcal_Per_day("kcal/day");
const PowerUnit PowerUnit::J_Per_s("J/s");
const PowerUnit PowerUnit::BTU_Per_hr("BTU/hr");

bool PowerUnit::IsValidUnit(const std::string& unit)
{
  if (W.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_s.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_hr.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_day.GetString().compare(unit) == 0)
    return true;
  if (J_Per_s.GetString().compare(unit) == 0)
    return true;
  if (BTU_Per_hr.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PowerUnit& PowerUnit::GetCompoundUnit(const std::string& unit)
{
  if (W.GetString().compare(unit) == 0)
    return W;
  if (kcal_Per_s.GetString().compare(unit) == 0)
    return kcal_Per_s;
  if (kcal_Per_hr.GetString().compare(unit) == 0)
    return kcal_Per_hr;
  if (kcal_Per_day.GetString().compare(unit) == 0)
    return kcal_Per_day;
  if (J_Per_s.GetString().compare(unit) == 0)
    return J_Per_s;
  if (BTU_Per_hr.GetString().compare(unit) == 0)
    return BTU_Per_hr;
  throw CommonDataModelException(unit + " is not a valid Power unit");
}

template class SEScalarQuantity<PowerUnit>;
