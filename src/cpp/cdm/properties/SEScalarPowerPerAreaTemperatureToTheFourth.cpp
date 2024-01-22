/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h"

const PowerPerAreaTemperatureToTheFourthUnit PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4("W/ m^2 K^4");

bool PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(const std::string& unit)
{
  if (W_Per_m2_K4.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PowerPerAreaTemperatureToTheFourthUnit& PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(const std::string& unit)
{
  if (W_Per_m2_K4.GetString().compare(unit) == 0)
    return W_Per_m2_K4;
  throw CommonDataModelException(unit + " is not a valid PowerPerAreaTemperatureToTheFourth unit");
}

template class SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;
