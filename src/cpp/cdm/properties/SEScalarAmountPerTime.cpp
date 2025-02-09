/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarAmountPerTime.h"

AmountPerTimeUnit AmountPerTimeUnit::mol_Per_day("mol/day");
AmountPerTimeUnit AmountPerTimeUnit::mol_Per_s("mol/s");
AmountPerTimeUnit AmountPerTimeUnit::umol_Per_s("umol/s");
AmountPerTimeUnit AmountPerTimeUnit::mmol_Per_min("mmol/min");
AmountPerTimeUnit AmountPerTimeUnit::pmol_Per_min("pmol/min");
AmountPerTimeUnit AmountPerTimeUnit::umol_Per_min("umol/min");

bool AmountPerTimeUnit::IsValidUnit(const std::string& unit)
{
  if (mol_Per_day.GetString().compare(unit) == 0)
    return true;
  if (mol_Per_s.GetString().compare(unit) == 0)
    return true;
  if (umol_Per_s.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_min.GetString().compare(unit) == 0)
    return true;
  if (pmol_Per_min.GetString().compare(unit) == 0)
    return true;
  if (umol_Per_min.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AmountPerTimeUnit& AmountPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mol_Per_day.GetString().compare(unit) == 0)
    return mol_Per_day;
  if (mol_Per_s.GetString().compare(unit) == 0)
    return mol_Per_s;
  if (umol_Per_s.GetString().compare(unit) == 0)
    return umol_Per_s;
  if (mmol_Per_min.GetString().compare(unit) == 0)
    return mmol_Per_min;
  if (pmol_Per_min.GetString().compare(unit) == 0)
    return pmol_Per_min;
  if (umol_Per_min.GetString().compare(unit) == 0)
    return umol_Per_min;
  throw CommonDataModelException(unit + " is not a valid  AmountPerTime unit");
}

template class SEScalarQuantity<AmountPerTimeUnit>;
