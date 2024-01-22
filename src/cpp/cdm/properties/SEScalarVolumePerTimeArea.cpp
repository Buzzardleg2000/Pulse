/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolumePerTimeArea.h"

const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::mL_Per_min_m2("mL/min m^2");
const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::mL_Per_s_m2("mL/s m^2");
const VolumePerTimeAreaUnit VolumePerTimeAreaUnit::L_Per_min_m2("L/min m^2");

bool VolumePerTimeAreaUnit::IsValidUnit(const std::string& unit)
{
  if (mL_Per_min_m2.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_s_m2.GetString().compare(unit) == 0)
    return true;
  if (L_Per_min_m2.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumePerTimeAreaUnit& VolumePerTimeAreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (mL_Per_min_m2.GetString().compare(unit) == 0)
    return mL_Per_min_m2;
  if (mL_Per_s_m2.GetString().compare(unit) == 0)
    return mL_Per_s_m2;
  if (L_Per_min_m2.GetString().compare(unit) == 0)
    return L_Per_min_m2;
  throw CommonDataModelException(unit + " is not a valid VolumePerTimeArea unit");
}

template class SEScalarQuantity<VolumePerTimeAreaUnit>;
