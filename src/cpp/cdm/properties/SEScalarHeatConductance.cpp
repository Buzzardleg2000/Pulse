/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarHeatConductance.h"

const HeatConductanceUnit HeatConductanceUnit::W_Per_K("W/K");
const HeatConductanceUnit HeatConductanceUnit::W_Per_C("W/degC");
const HeatConductanceUnit HeatConductanceUnit::kcal_Per_K_s("kcal/K s");
const HeatConductanceUnit HeatConductanceUnit::kcal_Per_C_s("kcal/degC s");

bool HeatConductanceUnit::IsValidUnit(const std::string& unit)
{
  if (W_Per_K.GetString().compare(unit) == 0)
    return true;
  if (W_Per_C.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_K_s.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_C_s.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatConductanceUnit& HeatConductanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (W_Per_K.GetString().compare(unit) == 0)
    return W_Per_K;
  if (W_Per_C.GetString().compare(unit) == 0)
    return W_Per_C;
  if (kcal_Per_K_s.GetString().compare(unit) == 0)
    return kcal_Per_K_s;
  if (kcal_Per_C_s.GetString().compare(unit) == 0)
    return kcal_Per_C_s;
  throw CommonDataModelException(unit + " is not a valid HeatConductance unit");
}

template class SEScalarQuantity<HeatConductanceUnit>;
