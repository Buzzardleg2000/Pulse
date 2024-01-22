/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarVolume.h"

const VolumeUnit VolumeUnit::L("L");
const VolumeUnit VolumeUnit::dL("dL");
const VolumeUnit VolumeUnit::mL("mL");
const VolumeUnit VolumeUnit::uL("uL");
const VolumeUnit VolumeUnit::m3("m^3");

bool VolumeUnit::IsValidUnit(const std::string& unit)
{
  if (L.GetString().compare(unit) == 0)
    return true;
  if (dL.GetString().compare(unit) == 0)
    return true;
  if (mL.GetString().compare(unit) == 0)
    return true;
  if (uL.GetString().compare(unit) == 0)
    return true;
  if (m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const VolumeUnit& VolumeUnit::GetCompoundUnit(const std::string& unit)
{
  if(L.GetString().compare(unit)==0)
    return L;
  if(mL.GetString().compare(unit)==0)
    return mL;
  if(m3.GetString().compare(unit)==0)
    return m3;
  if (dL.GetString().compare(unit) == 0)
    return dL;
  if (uL.GetString().compare(unit) == 0)
    return uL;
  throw CommonDataModelException(unit + " is not a valid Volume unit");
}

template class SEScalarQuantity<VolumeUnit>;
