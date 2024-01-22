/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"

AmountPerVolumeUnit AmountPerVolumeUnit::mEq_Per_L("mEq/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_L("mol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mol_Per_mL("mol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_L("mmol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::mmol_Per_mL("mmol/mL");
AmountPerVolumeUnit AmountPerVolumeUnit::pmol_Per_L("pmol/L");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_L("ct/L");
AmountPerVolumeUnit AmountPerVolumeUnit::ct_Per_uL("ct/uL");

bool AmountPerVolumeUnit::IsValidUnit(const std::string& unit)
{
  if (mEq_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (pmol_Per_L.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return true;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AmountPerVolumeUnit& AmountPerVolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if (mEq_Per_L.GetString().compare(unit) == 0)
    return mEq_Per_L;
  if (mol_Per_L.GetString().compare(unit) == 0)
    return mol_Per_L;
  if (mol_Per_mL.GetString().compare(unit) == 0)
    return mol_Per_mL;
  if (mmol_Per_L.GetString().compare(unit) == 0)
    return mmol_Per_L;
  if (mmol_Per_mL.GetString().compare(unit) == 0)
    return mmol_Per_mL;
  if (pmol_Per_L.GetString().compare(unit) == 0)
    return pmol_Per_L;
  if (ct_Per_L.GetString().compare(unit) == 0)
    return ct_Per_L;
  if (ct_Per_uL.GetString().compare(unit) == 0)
    return ct_Per_uL;
  throw CommonDataModelException(unit + " is not a valid AmountPerVolume unit");
}

template class SEScalarQuantity<AmountPerVolumeUnit>;
