/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarAmountPerMass.h"

AmountPerMassUnit AmountPerMassUnit::ct_Per_g("ct/g");
AmountPerMassUnit AmountPerMassUnit::ct_Per_ug("ct/ug");

bool AmountPerMassUnit::IsValidUnit(const std::string& unit)
{
  if(ct_Per_g.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_ug.GetString().compare(unit) == 0)
    return true;
  return false;
}
const AmountPerMassUnit& AmountPerMassUnit::GetCompoundUnit(const std::string& unit)
{
  if(ct_Per_g.GetString().compare(unit)==0)
    return ct_Per_g;
  if(ct_Per_ug.GetString().compare(unit)==0)
    return ct_Per_ug;
  throw CommonDataModelException(unit + " is not a valid AmountPerMass unit");
}

template class SEScalarQuantity<AmountPerMassUnit>;
