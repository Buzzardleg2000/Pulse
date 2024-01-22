/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarArea.h"

const AreaUnit AreaUnit::cm2("cm^2");
const AreaUnit AreaUnit::m2("m^2");

bool AreaUnit::IsValidUnit(const std::string& unit)
{
  if (cm2.GetString().compare(unit) == 0)
    return true;
  if (m2.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AreaUnit& AreaUnit::GetCompoundUnit(const std::string& unit)
{
  if (cm2.GetString().compare(unit) == 0)
    return cm2;
  if (m2.GetString().compare(unit) == 0)
    return m2;
  throw CommonDataModelException(unit + " is not a valid  Area unit");
}

template class SEScalarQuantity<AreaUnit>;
