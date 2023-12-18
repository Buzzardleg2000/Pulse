/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarEnergyPerAmount.h"

const EnergyPerAmountUnit EnergyPerAmountUnit::kcal_Per_mol("kcal/mol");
const EnergyPerAmountUnit EnergyPerAmountUnit::kJ_Per_mol("kJ/mol");

bool EnergyPerAmountUnit::IsValidUnit(const std::string& unit)
{
  if (kcal_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_mol.GetString().compare(unit) == 0)
    return true;
  return false;
}

const EnergyPerAmountUnit& EnergyPerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  if (kcal_Per_mol.GetString().compare(unit) == 0)
    return kcal_Per_mol;
  if (kJ_Per_mol.GetString().compare(unit) == 0)
    return kJ_Per_mol;
  throw CommonDataModelException(unit + " is not a valid EnergyPerAmount unit");
}

template class SEScalarQuantity<EnergyPerAmountUnit>;
