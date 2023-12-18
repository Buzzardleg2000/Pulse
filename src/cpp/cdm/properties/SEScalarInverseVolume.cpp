/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarInverseVolume.h"

const InverseVolumeUnit InverseVolumeUnit::Inverse_L("1/L");
const InverseVolumeUnit InverseVolumeUnit::Inverse_mL("1/mL");

bool InverseVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (Inverse_L.GetString().compare(unit) == 0)
    return true;
  if (Inverse_mL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const InverseVolumeUnit& InverseVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (Inverse_L.GetString().compare(unit) == 0)
    return Inverse_L;
  if (Inverse_mL.GetString().compare(unit) == 0)
    return Inverse_mL;
  throw CommonDataModelException(unit + " is not a valid InverseVolume unit");
}

template class SEScalarQuantity<InverseVolumeUnit>;
