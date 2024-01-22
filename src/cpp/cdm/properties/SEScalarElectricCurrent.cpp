/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarElectricCurrent.h"

const ElectricCurrentUnit ElectricCurrentUnit::A("A");

bool ElectricCurrentUnit::IsValidUnit(const std::string& unit)
{
  if (A.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricCurrentUnit& ElectricCurrentUnit::GetCompoundUnit(const std::string& unit)
{
  if (A.GetString().compare(unit) == 0)
    return A;
  throw CommonDataModelException(unit + " is not a valid ElectricCurrent unit");
}

template class SEScalarQuantity<ElectricCurrentUnit>;
