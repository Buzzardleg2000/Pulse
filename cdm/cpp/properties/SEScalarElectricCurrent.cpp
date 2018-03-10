/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarElectricCurrent.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

const ElectricCurrentUnit ElectricCurrentUnit::A("A");

bool ElectricCurrentUnit::IsValidUnit(const std::string& unit)
{
  if (A.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricCurrentUnit& ElectricCurrentUnit::GetCompoundUnit(const std::string& unit)
{
  if (A.GetString().compare(unit) == 0)
    return A;
  std::stringstream err;
  err << unit << " is not a valid ElectricCurrent unit";
  throw CommonDataModelException(err.str());
}

void SEScalarElectricCurrent::Load(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  SEScalarElectricCurrent::Serialize(src, dst);
}
void SEScalarElectricCurrent::Serialize(const cdm::ScalarElectricCurrentData& src, SEScalarElectricCurrent& dst)
{
  SEUnitScalar::Serialize(src.scalarelectriccurrent(), dst);
}

cdm::ScalarElectricCurrentData* SEScalarElectricCurrent::Unload(const SEScalarElectricCurrent& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricCurrentData* dst = new cdm::ScalarElectricCurrentData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarElectricCurrent::Serialize(const SEScalarElectricCurrent& src, cdm::ScalarElectricCurrentData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarelectriccurrent());
}