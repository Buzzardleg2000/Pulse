/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPressureTimePerArea.h"

const PressureTimePerAreaUnit PressureTimePerAreaUnit::mmHg_Per_mL_m2("mmHg/mL m^2");
const PressureTimePerAreaUnit PressureTimePerAreaUnit::cmH2O_Per_mL_m2("cmH2O/mL m^2");

bool PressureTimePerAreaUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_Per_mL_m2.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_Per_mL_m2.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressureTimePerAreaUnit& PressureTimePerAreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_Per_mL_m2.GetString().compare(unit) == 0)
    return mmHg_Per_mL_m2;
  if (cmH2O_Per_mL_m2.GetString().compare(unit) == 0)
    return cmH2O_Per_mL_m2;
  throw CommonDataModelException(unit + " is not a valid PressureTimePerArea unit");
}

template class SEScalarQuantity<PressureTimePerAreaUnit>;
