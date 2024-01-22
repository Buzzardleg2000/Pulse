/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarHeatCapacitancePerAmount.h"

const HeatCapacitancePerAmountUnit HeatCapacitancePerAmountUnit::J_Per_K_mol("J/K mol");

bool HeatCapacitancePerAmountUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_K_mol.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatCapacitancePerAmountUnit& HeatCapacitancePerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_K_mol.GetString().compare(unit) == 0)
    return J_Per_K_mol;
  throw CommonDataModelException(unit + " is not a valid HeatCapacitancePerAmount unit");
}

template class SEScalarQuantity<HeatCapacitancePerAmountUnit>;
