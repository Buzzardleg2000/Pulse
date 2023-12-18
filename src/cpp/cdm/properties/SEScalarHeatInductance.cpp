/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarHeatInductance.h"

const HeatInductanceUnit HeatInductanceUnit::K_s_Per_W("K s/W");

bool HeatInductanceUnit::IsValidUnit(const std::string& unit)
{
  if (K_s_Per_W.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatInductanceUnit& HeatInductanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (K_s_Per_W.GetString().compare(unit) == 0)
    return K_s_Per_W;
  throw CommonDataModelException(unit + " is not a valid HeatInductance unit");
}

template class SEScalarQuantity<HeatInductanceUnit>;
