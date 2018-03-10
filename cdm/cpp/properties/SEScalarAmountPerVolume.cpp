/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarAmountPerVolume.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_L("mol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_mL("mol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_L("mmol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_mL("mmol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_L("ct/L");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_uL("ct/uL");

bool AmountPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AmountPerVolumeUnit& AmountPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mol_Per_L.GetString().compare(unit) == 0)
    return mol_Per_L;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return mol_Per_mL;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return mmol_Per_L;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return mmol_Per_mL;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return ct_Per_L;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return ct_Per_uL;
  std::stringstream err;
  err << unit << " is not a valid AmountPerVolume unit";
  throw CommonDataModelException(err.str());
}

void SEScalarAmountPerVolume::Load(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  SEScalarAmountPerVolume::Serialize(src, dst);
}
void SEScalarAmountPerVolume::Serialize(const cdm::ScalarAmountPerVolumeData& src, SEScalarAmountPerVolume& dst)
{
  SEUnitScalar::Serialize(src.scalaramountpervolume(), dst);
}

cdm::ScalarAmountPerVolumeData* SEScalarAmountPerVolume::Unload(const SEScalarAmountPerVolume& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAmountPerVolumeData* dst = new cdm::ScalarAmountPerVolumeData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarAmountPerVolume::Serialize(const SEScalarAmountPerVolume& src, cdm::ScalarAmountPerVolumeData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalaramountpervolume());
}