/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarAmount.h"


const AmountUnit AmountUnit::mol("mol");
const AmountUnit AmountUnit::pmol("pmol");
const AmountUnit AmountUnit::ct("ct");

bool AmountUnit::IsValidUnit(const std::string& unit)
{
  if (mol.GetString().compare(unit) == 0)
    return true;
  if (pmol.GetString().compare(unit) == 0)
    return true;
  if (ct.GetString().compare(unit) == 0)
    return true;
  return false;
}
const AmountUnit& AmountUnit::GetCompoundUnit(const std::string& unit)
{
  if(mol.GetString().compare(unit)==0)
    return mol;
  if (pmol.GetString().compare(unit) == 0)
    return pmol;
  if (ct.GetString().compare(unit) == 0)
    return ct;
  throw CommonDataModelException(unit + " is not a valid Amount unit");
}

template class SEScalarQuantity<AmountUnit>;
