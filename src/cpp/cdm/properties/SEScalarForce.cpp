/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarForce.h"

const ForceUnit ForceUnit::N("N");
const ForceUnit ForceUnit::lbf("lbf");
const ForceUnit ForceUnit::dyn("dyn");

bool ForceUnit::IsValidUnit(const std::string& unit)
{
  if (N.GetString().compare(unit) == 0)
    return true;
  if (lbf.GetString().compare(unit) == 0)
    return true;
  if (dyn.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ForceUnit& ForceUnit::GetCompoundUnit(const std::string& unit)
{
  if (N.GetString().compare(unit) == 0)
    return N;
  if (lbf.GetString().compare(unit) == 0)
    return lbf;
  if (dyn.GetString().compare(unit) == 0)
    return dyn;
  throw CommonDataModelException(unit + " is not a valid Force unit");
}

template class SEScalarQuantity<ForceUnit>;
