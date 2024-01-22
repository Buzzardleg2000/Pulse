/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarElectricPotential.h"

const ElectricPotentialUnit ElectricPotentialUnit::V("V");
const ElectricPotentialUnit ElectricPotentialUnit::mV("mV");

bool ElectricPotentialUnit::IsValidUnit(const std::string& unit)
{
  if (V.GetString().compare(unit) == 0)
    return true;
  if (mV.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricPotentialUnit& ElectricPotentialUnit::GetCompoundUnit(const std::string& unit)
{
  if(V.GetString().compare(unit)==0)
    return V;
  if(mV.GetString().compare(unit)==0)
    return mV;
  throw CommonDataModelException(unit + " is not a valid ElectricPotential unit");
}

template class SEScalarQuantity<ElectricPotentialUnit>;
