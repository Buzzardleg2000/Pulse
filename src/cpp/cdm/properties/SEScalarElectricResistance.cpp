/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarElectricResistance.h"

const ElectricResistanceUnit ElectricResistanceUnit::Ohm("ohm");

bool ElectricResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (Ohm.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricResistanceUnit& ElectricResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (Ohm.GetString().compare(unit) == 0)
    return Ohm;
  throw CommonDataModelException(unit + " is not a valid ElectricResistance unit");
}

template class SEScalarQuantity<ElectricResistanceUnit>;
