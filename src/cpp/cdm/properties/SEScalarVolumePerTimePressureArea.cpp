/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolumePerTimePressureArea.h"

const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2("mL/min mmHg m^2");
const VolumePerTimePressureAreaUnit VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2("mL/s mmHg m^2");

bool VolumePerTimePressureAreaUnit::IsValidUnit(const std::string& unit)
{
  if (mL_Per_min_mmHg_m2.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_s_mmHg_m2.GetString().compare(unit) == 0)
    return true; 
  return false;
}

const VolumePerTimePressureAreaUnit& VolumePerTimePressureAreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (mL_Per_min_mmHg_m2.GetString().compare(unit) == 0)
    return mL_Per_min_mmHg_m2;
  if (mL_Per_s_mmHg_m2.GetString().compare(unit) == 0)
    return mL_Per_s_mmHg_m2;  
  throw CommonDataModelException(unit + " is not a valid VolumePerTimePressureArea unit");
}

template class SEScalarQuantity<VolumePerTimePressureAreaUnit>;
