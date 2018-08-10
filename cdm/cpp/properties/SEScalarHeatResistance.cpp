/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarHeatResistance.h"
#include "bind/cdm/Properties.pb.h"

const HeatResistanceUnit HeatResistanceUnit::K_Per_W("K/W");
const HeatResistanceUnit HeatResistanceUnit::C_Per_W("degC/W");
const HeatResistanceUnit HeatResistanceUnit::K_s_Per_kcal("K s/kcal");
const HeatResistanceUnit HeatResistanceUnit::C_s_Per_kcal("degC s/kcal");

bool HeatResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return true;
  if (C_Per_W.GetString().compare(unit) == 0)
    return true;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return true;
  return false;
}

const HeatResistanceUnit& HeatResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (K_Per_W.GetString().compare(unit) == 0)
    return K_Per_W;
  if (C_Per_W.GetString().compare(unit) == 0)
    return C_Per_W;
  if (K_s_Per_kcal.GetString().compare(unit) == 0)
    return K_s_Per_kcal;
  if (C_s_Per_kcal.GetString().compare(unit) == 0)
    return C_s_Per_kcal;
  std::stringstream err;
  err << unit << " is not a valid HeatResistance unit";
  throw CommonDataModelException(err.str());
}

void SEScalarHeatResistance::Load(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  SEScalarHeatResistance::Serialize(src, dst);
}
void SEScalarHeatResistance::Serialize(const cdm::ScalarHeatResistanceData& src, SEScalarHeatResistance& dst)
{
  SEUnitScalar::Serialize(src.scalarheatresistance(), dst);
}

cdm::ScalarHeatResistanceData* SEScalarHeatResistance::Unload(const SEScalarHeatResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarHeatResistanceData* dst = new cdm::ScalarHeatResistanceData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarHeatResistance::Serialize(const SEScalarHeatResistance& src, cdm::ScalarHeatResistanceData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarheatresistance());
}