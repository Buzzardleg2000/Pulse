/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"

const VolumePerPressureUnit VolumePerPressureUnit::L_Per_Pa("L/Pa");
const VolumePerPressureUnit VolumePerPressureUnit::L_Per_cmH2O("L/cmH2O");
const VolumePerPressureUnit VolumePerPressureUnit::mL_Per_cmH2O("mL/cmH2O");
const VolumePerPressureUnit VolumePerPressureUnit::m3_Per_Pa("m^3/Pa");
const VolumePerPressureUnit VolumePerPressureUnit::mL_Per_mmHg("mL/mmHg");

bool VolumePerPressureUnit::IsValidUnit(const std::string& unit)
{
  if (L_Per_Pa.GetString().compare(unit) == 0)
    return true;
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (m3_Per_Pa.GetString().compare(unit) == 0)
    return true;
  if (mL_Per_mmHg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumePerPressureUnit& VolumePerPressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (L_Per_Pa.GetString().compare(unit) == 0)
    return L_Per_Pa;
  if (L_Per_cmH2O.GetString().compare(unit) == 0)
    return L_Per_cmH2O;
  if (mL_Per_cmH2O.GetString().compare(unit) == 0)
    return mL_Per_cmH2O;
  if (m3_Per_Pa.GetString().compare(unit) == 0)
    return m3_Per_Pa;
  if (mL_Per_mmHg.GetString().compare(unit) == 0)
    return mL_Per_mmHg;
  throw CommonDataModelException(unit + " is not a valid VolumePerPressure unit");
}

template class SEScalarQuantity<VolumePerPressureUnit>;
