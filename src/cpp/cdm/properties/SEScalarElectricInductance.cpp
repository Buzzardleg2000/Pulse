/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarElectricInductance.h"

const ElectricInductanceUnit ElectricInductanceUnit::H("H");

bool ElectricInductanceUnit::IsValidUnit(const std::string& unit)
{
  if (H.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricInductanceUnit& ElectricInductanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (H.GetString().compare(unit) == 0)
    return H;
  throw CommonDataModelException(unit + " is not a valid ElectricInductance unit");
}

template class SEScalarQuantity<ElectricInductanceUnit>;
