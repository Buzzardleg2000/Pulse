/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarEquivalentWeightPerVolume.h"

const EquivalentWeightPerVolumeUnit EquivalentWeightPerVolumeUnit::Eq_Per_L("Eq/L");
const EquivalentWeightPerVolumeUnit EquivalentWeightPerVolumeUnit::Eq_Per_mL("Eq/mL");
const EquivalentWeightPerVolumeUnit EquivalentWeightPerVolumeUnit::mEq_Per_L("mEq/L");
const EquivalentWeightPerVolumeUnit EquivalentWeightPerVolumeUnit::mEq_Per_mL("mEq/mL");

bool EquivalentWeightPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (Eq_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Eq_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mEq_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mEq_Per_mL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const EquivalentWeightPerVolumeUnit& EquivalentWeightPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (Eq_Per_L.GetString().compare(unit) == 0)
    return Eq_Per_L;
  if (Eq_Per_mL.GetString().compare(unit) == 0)
    return Eq_Per_mL;
  if (mEq_Per_L.GetString().compare(unit) == 0)
    return mEq_Per_L;
  if (mEq_Per_mL.GetString().compare(unit) == 0)
    return mEq_Per_mL;
  throw CommonDataModelException(unit + " is not a valid EquivalentWeightPerVolume unit");
}

template class SEScalarQuantity<EquivalentWeightPerVolumeUnit>;
