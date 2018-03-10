/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarHeatCapacitance.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

const HeatCapacitanceUnit HeatCapacitanceUnit::J_Per_K("J/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kJ_Per_K("kJ/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_K("kcal/K");
const HeatCapacitanceUnit HeatCapacitanceUnit::kcal_Per_C("kcal/degC");

bool HeatCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return true;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatCapacitanceUnit& HeatCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (J_Per_K.GetString().compare(unit) == 0)
    return J_Per_K;
  if (kJ_Per_K.GetString().compare(unit) == 0)
    return kJ_Per_K;
  if (kcal_Per_K.GetString().compare(unit) == 0)
    return kcal_Per_K;
  if (kcal_Per_C.GetString().compare(unit) == 0)
    return kcal_Per_C;
  std::stringstream err;
  err << unit << " is not a valid HeatCapacitance unit";
  throw CommonDataModelException(err.str());
}

void SEScalarHeatCapacitance::Load(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  SEScalarHeatCapacitance::Serialize(src, dst);
}
void SEScalarHeatCapacitance::Serialize(const cdm::ScalarHeatCapacitanceData& src, SEScalarHeatCapacitance& dst)
{
  SEUnitScalar::Serialize(src.scalarheatcapacitance(), dst);
}

cdm::ScalarHeatCapacitanceData* SEScalarHeatCapacitance::Unload(const SEScalarHeatCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatCapacitanceData* dst = new cdm::ScalarHeatCapacitanceData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarHeatCapacitance::Serialize(const SEScalarHeatCapacitance& src, cdm::ScalarHeatCapacitanceData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarheatcapacitance());
}