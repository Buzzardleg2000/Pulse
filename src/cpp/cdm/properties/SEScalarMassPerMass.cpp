/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarMassPerMass.h"

const MassPerMassUnit MassPerMassUnit::ug_Per_kg("ug/kg");
const MassPerMassUnit MassPerMassUnit::mg_Per_g("mg/g");

bool MassPerMassUnit::IsValidUnit(const std::string& unit)
{
  if (ug_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_g.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerMassUnit& MassPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if (ug_Per_kg.GetString().compare(unit) == 0)
    return ug_Per_kg;
  if (mg_Per_g.GetString().compare(unit) == 0)
    return mg_Per_g;
  throw CommonDataModelException(unit + " is not a valid MassPerMass unit");
}

template class SEScalarQuantity<MassPerMassUnit>;
