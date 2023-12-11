/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarEnergyPerMass.h"

const EnergyPerMassUnit EnergyPerMassUnit::J_Per_kg("J/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kJ_Per_kg("kJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::mJ_Per_kg("mJ/kg");
const EnergyPerMassUnit EnergyPerMassUnit::kcal_Per_kg("kcal/kg");

bool EnergyPerMassUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (mJ_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_kg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const EnergyPerMassUnit& EnergyPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_kg.GetString().compare(unit) == 0)
    return J_Per_kg;
  if (kJ_Per_kg.GetString().compare(unit) == 0)
    return kJ_Per_kg;
  if (mJ_Per_kg.GetString().compare(unit) == 0)
    return mJ_Per_kg;
  if (kcal_Per_kg.GetString().compare(unit) == 0)
    return kcal_Per_kg;
  throw CommonDataModelException(unit + " is not a valid EnergyPerMass unit");
}

template class SEScalarQuantity<EnergyPerMassUnit>;
