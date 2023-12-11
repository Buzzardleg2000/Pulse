/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"

const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::cmH2O_s_Per_L("cmH2O s/L");
const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::cmH2O_s_Per_mL("cmH2O s/mL");
const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::mmHg_s_Per_mL("mmHg s/mL");
const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::mmHg_min_Per_mL("mmHg min/mL");
const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::mmHg_min_Per_L("mmHg min/L");
const PressureTimePerVolumeUnit PressureTimePerVolumeUnit::Pa_s_Per_m3("Pa s/m^3");

bool PressureTimePerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (cmH2O_s_Per_L.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_s_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_min_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_min_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Pa_s_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressureTimePerVolumeUnit& PressureTimePerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (cmH2O_s_Per_L.GetString().compare(unit) == 0)
    return cmH2O_s_Per_L;
  if (cmH2O_s_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_s_Per_mL;
  if (mmHg_s_Per_mL.GetString().compare(unit) == 0)
    return mmHg_s_Per_mL;
  if (mmHg_min_Per_mL.GetString().compare(unit) == 0)
    return mmHg_min_Per_mL;
  if (mmHg_min_Per_L.GetString().compare(unit) == 0)
    return mmHg_min_Per_L;
  if (Pa_s_Per_m3.GetString().compare(unit) == 0)
    return Pa_s_Per_m3;
  throw CommonDataModelException(unit + " is not a valid PressureTimePerVolume unit");
}

template class SEScalarQuantity<PressureTimePerVolumeUnit>;
