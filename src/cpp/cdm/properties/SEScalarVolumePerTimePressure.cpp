/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolumePerTimePressure.h"

const VolumePerTimePressureUnit VolumePerTimePressureUnit::L_Per_s_mmHg("L/s mmHg");
const VolumePerTimePressureUnit VolumePerTimePressureUnit::mL_Per_s_mmHg("mL/s mmHg");
const VolumePerTimePressureUnit VolumePerTimePressureUnit::L_Per_min_mmHg("L/min mmHg");
const VolumePerTimePressureUnit VolumePerTimePressureUnit::mL_Per_min_mmHg("mL/min mmHg");

bool VolumePerTimePressureUnit::IsValidUnit(const std::string& unit)
{
  if (L_Per_s_mmHg.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_s_mmHg.GetString().compare(unit) == 0)
    return true;
  if (L_Per_min_mmHg.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_min_mmHg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumePerTimePressureUnit& VolumePerTimePressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (L_Per_s_mmHg.GetString().compare(unit) == 0)
    return L_Per_s_mmHg;
  if (mL_Per_s_mmHg.GetString().compare(unit) == 0)
    return mL_Per_s_mmHg;
  if (L_Per_min_mmHg.GetString().compare(unit) == 0)
    return L_Per_min_mmHg;
  if (mL_Per_min_mmHg.GetString().compare(unit) == 0)
    return mL_Per_min_mmHg;
  throw CommonDataModelException(unit + " is not a valid VolumePerTimePressure unit");
}

template class SEScalarQuantity<VolumePerTimePressureUnit>;
