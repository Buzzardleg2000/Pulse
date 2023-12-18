/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarHeatResistance.h"

const HeatResistanceUnit HeatResistanceUnit::K_Per_W("K/W");
const HeatResistanceUnit HeatResistanceUnit::C_Per_W("degC/W");
const HeatResistanceUnit HeatResistanceUnit::K_s_Per_kcal("K s/kcal");
const HeatResistanceUnit HeatResistanceUnit::C_s_Per_kcal("degC s/kcal");

bool HeatResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return true;
  if (C_Per_W.GetString().compare(unit) == 0)
    return true;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatResistanceUnit& HeatResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return K_Per_W;
  if (C_Per_W.GetString().compare(unit) == 0)
    return C_Per_W;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return K_s_Per_kcal;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return C_s_Per_kcal;
  throw CommonDataModelException(unit + " is not a valid HeatResistance unit");
}

template class SEScalarQuantity<HeatResistanceUnit>;
