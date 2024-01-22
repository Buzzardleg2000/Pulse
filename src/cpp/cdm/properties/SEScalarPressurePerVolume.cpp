/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"

const PressurePerVolumeUnit PressurePerVolumeUnit::mmHg_Per_mL("mmHg/mL");
const PressurePerVolumeUnit PressurePerVolumeUnit::cmH2O_Per_mL("cmH2O/mL");
const PressurePerVolumeUnit PressurePerVolumeUnit::cmH2O_Per_L("cmH2O/L");

bool PressurePerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressurePerVolumeUnit& PressurePerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return mmHg_Per_mL;
  if (cmH2O_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_Per_mL;
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return cmH2O_Per_L;
  throw CommonDataModelException(unit + " is not a valid PressurePerVolume unit");
}

template class SEScalarQuantity<PressurePerVolumeUnit>;
