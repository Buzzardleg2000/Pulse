/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarLength.h"

const LengthUnit LengthUnit::m("m");
const LengthUnit LengthUnit::cm("cm");
const LengthUnit LengthUnit::mm("mm");
const LengthUnit LengthUnit::um("um");
const LengthUnit LengthUnit::in("in");
const LengthUnit LengthUnit::ft("ft");

bool LengthUnit::IsValidUnit(const std::string& unit)
{
  if (m.GetString().compare(unit) == 0)
    return true;
  if (cm.GetString().compare(unit) == 0)
    return true;
  if (mm.GetString().compare(unit) == 0)
    return true;
  if (um.GetString().compare(unit) == 0)
    return true;
  if (in.GetString().compare(unit) == 0)
    return true;
  if (ft.GetString().compare(unit) == 0)
    return true;
  return false;
}

const LengthUnit& LengthUnit::GetCompoundUnit(const std::string& unit)
{
  if (m.GetString().compare(unit) == 0)
    return m;
  if (cm.GetString().compare(unit) == 0)
    return cm;
  if (mm.GetString().compare(unit) == 0)
    return mm;
  if (um.GetString().compare(unit) == 0)
    return um;
  if (in.GetString().compare(unit) == 0)
    return in;
  if (ft.GetString().compare(unit) == 0)
    return ft;
  throw CommonDataModelException(unit + " is not a valid Length unit");
}

template class SEScalarQuantity<LengthUnit>;
