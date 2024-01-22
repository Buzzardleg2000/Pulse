/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarEnergy.h"

const EnergyUnit EnergyUnit::J("J");
const EnergyUnit EnergyUnit::mJ("mJ");
const EnergyUnit EnergyUnit::kJ("kJ");
const EnergyUnit EnergyUnit::kcal("kcal");

bool EnergyUnit::IsValidUnit(const std::string& unit)
{
  if (J.GetString().compare(unit) == 0)
    return true;
  if (mJ.GetString().compare(unit) == 0)
    return true;
  if (kJ.GetString().compare(unit) == 0)
    return true;
  if (kcal.GetString().compare(unit) == 0)
    return true;
  return false;
}

const EnergyUnit& EnergyUnit::GetCompoundUnit(const std::string& unit)
{
  if (J.GetString().compare(unit) == 0)
    return J;
  if (mJ.GetString().compare(unit) == 0)
    return mJ;
  if (kJ.GetString().compare(unit) == 0)
    return kJ;
  if (kcal.GetString().compare(unit) == 0)
    return kcal;
  throw CommonDataModelException(unit + " is not a valid Energy unit");
}

template class SEScalarQuantity<EnergyUnit>;
