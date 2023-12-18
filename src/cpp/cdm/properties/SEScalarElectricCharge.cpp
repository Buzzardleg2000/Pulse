/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarElectricCharge.h"

const ElectricChargeUnit ElectricChargeUnit::C("C");

bool ElectricChargeUnit::IsValidUnit(const std::string& unit)
{
  if (C.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricChargeUnit& ElectricChargeUnit::GetCompoundUnit(const std::string& unit)
{
  if (C.GetString().compare(unit) == 0)
    return C;
  throw CommonDataModelException(unit + " is not a valid ElectricCharge unit");
}

template class SEScalarQuantity<ElectricChargeUnit>;
