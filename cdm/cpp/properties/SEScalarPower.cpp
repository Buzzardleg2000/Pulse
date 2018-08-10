/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarPower.h"
#include "bind/cdm/Properties.pb.h"

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
  std::stringstream err;
  err << unit << " is not a valid Power unit";
  throw CommonDataModelException(err.str());
}

void SEScalarPower::Load(const cdm::ScalarPowerData& src, SEScalarPower& dst)
{
  SEScalarPower::Serialize(src, dst);
}
void SEScalarPower::Serialize(const cdm::ScalarPowerData& src, SEScalarPower& dst)
{
  SEUnitScalar::Serialize(src.scalarpower(), dst);
}

cdm::ScalarPowerData* SEScalarPower::Unload(const SEScalarPower& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPowerData* dst = new cdm::ScalarPowerData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarPower::Serialize(const SEScalarPower& src, cdm::ScalarPowerData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarpower());
}