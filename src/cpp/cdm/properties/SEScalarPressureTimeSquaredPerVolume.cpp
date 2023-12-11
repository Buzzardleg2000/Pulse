/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPressureTimeSquaredPerVolume.h"

const PressureTimeSquaredPerVolumeUnit PressureTimeSquaredPerVolumeUnit::mmHg_s2_Per_mL("mmHg s^2/mL");
const PressureTimeSquaredPerVolumeUnit PressureTimeSquaredPerVolumeUnit::mmHg_s2_Per_L("mmHg s^2/L");
const PressureTimeSquaredPerVolumeUnit PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_mL("cmH2O s^2/mL");
const PressureTimeSquaredPerVolumeUnit PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L("cmH2O s^2/L");
const PressureTimeSquaredPerVolumeUnit PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3("Pa s^2/m^3");

bool PressureTimeSquaredPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressureTimeSquaredPerVolumeUnit& PressureTimeSquaredPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return mmHg_s2_Per_mL;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return mmHg_s2_Per_L;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_mL;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_L;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return Pa_s2_Per_m3;
  throw CommonDataModelException(unit + " is not a valid PressureTimeSquaredPerVolume unit");
}

template class SEScalarQuantity<PressureTimeSquaredPerVolumeUnit>;
