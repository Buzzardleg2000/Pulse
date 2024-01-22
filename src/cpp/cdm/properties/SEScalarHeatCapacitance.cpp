/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"

const HeatCapacitanceUnit HeatCapacitanceUnit::J_Per_K("J/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kJ_Per_K("kJ/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_K("kcal/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_C("kcal/degC");

bool HeatCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatCapacitanceUnit& HeatCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return J_Per_K;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return kJ_Per_K;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return kcal_Per_K;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return kcal_Per_C;
  throw CommonDataModelException(unit + " is not a valid HeatCapacitance unit");
}

template class SEScalarQuantity<HeatCapacitanceUnit>;
