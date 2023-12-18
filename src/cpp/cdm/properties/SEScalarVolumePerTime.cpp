/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

const VolumePerTimeUnit VolumePerTimeUnit::L_Per_s("L/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_s("mL/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_day("mL/day");
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_day("L/day");
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_hr("L/hr");
const VolumePerTimeUnit VolumePerTimeUnit::L_Per_min("L/min");
const VolumePerTimeUnit VolumePerTimeUnit::m3_Per_s("m^3/s");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_min("mL/min");
const VolumePerTimeUnit VolumePerTimeUnit::mL_Per_hr("mL/hr");

bool VolumePerTimeUnit::IsValidUnit(const std::string& unit)
{
  if (L_Per_s.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_s.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_day.GetString().compare(unit) == 0)
    return true;
  if (L_Per_day.GetString().compare(unit) == 0)
    return true;
  if (L_Per_hr.GetString().compare(unit) == 0)
    return true;
  if (L_Per_min.GetString().compare(unit) == 0)
    return true;
  if (m3_Per_s.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_min.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_hr.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumePerTimeUnit& VolumePerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (L_Per_s.GetString().compare(unit) == 0)
    return L_Per_s;
  if (mL_Per_s.GetString().compare(unit) == 0)
    return mL_Per_s;
  if (mL_Per_day.GetString().compare(unit) == 0)
    return mL_Per_day;
  if (L_Per_day.GetString().compare(unit) == 0)
    return L_Per_day;
  if (L_Per_hr.GetString().compare(unit) == 0)
    return L_Per_hr;
  if (L_Per_min.GetString().compare(unit) == 0)
    return L_Per_min;
  if (m3_Per_s.GetString().compare(unit) == 0)
    return m3_Per_s;
  if (mL_Per_min.GetString().compare(unit) == 0)
    return mL_Per_min;
  if (mL_Per_hr.GetString().compare(unit) == 0)
    return mL_Per_hr;
  throw CommonDataModelException(unit + " is not a valid VolumePerTime unit");
}

template class SEScalarQuantity<VolumePerTimeUnit>;
