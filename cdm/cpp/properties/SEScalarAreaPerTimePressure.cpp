/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarAreaPerTimePressure.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

const AreaPerTimePressureUnit AreaPerTimePressureUnit::m2_Per_s_mmHg("m^2/s mmHg");
const AreaPerTimePressureUnit AreaPerTimePressureUnit::cm2_Per_s_mmHg("cm^2/s mmHg");
const AreaPerTimePressureUnit AreaPerTimePressureUnit::m2_Per_min_mmHg("m^2/min mmHg");
const AreaPerTimePressureUnit AreaPerTimePressureUnit::cm2_Per_min_mmHg("cm^2/min mmHg");

bool AreaPerTimePressureUnit::IsValidUnit(const std::string& unit)
{
  if (m2_Per_s_mmHg.GetString().compare(unit) == 0)
    return true;
  if (cm2_Per_s_mmHg.GetString().compare(unit) == 0)
    return true;
  if (m2_Per_min_mmHg.GetString().compare(unit) == 0)
    return true;
  if (cm2_Per_min_mmHg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const AreaPerTimePressureUnit& AreaPerTimePressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (m2_Per_s_mmHg.GetString().compare(unit) == 0)
    return m2_Per_s_mmHg;
  if (cm2_Per_s_mmHg.GetString().compare(unit) == 0)
    return cm2_Per_s_mmHg;
  if (m2_Per_min_mmHg.GetString().compare(unit) == 0)
    return m2_Per_min_mmHg;
  if (cm2_Per_min_mmHg.GetString().compare(unit) == 0)
    return cm2_Per_min_mmHg;
  std::stringstream err;
  err << unit << " is not a valid AreaPerTimePressure unit";
  throw CommonDataModelException(err.str());
}

void SEScalarAreaPerTimePressure::Load(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  SEScalarAreaPerTimePressure::Serialize(src, dst);
}
void SEScalarAreaPerTimePressure::Serialize(const cdm::ScalarAreaPerTimePressureData& src, SEScalarAreaPerTimePressure& dst)
{
  SEUnitScalar::Serialize(src.scalarareapertimepressure(), dst);
}

cdm::ScalarAreaPerTimePressureData* SEScalarAreaPerTimePressure::Unload(const SEScalarAreaPerTimePressure& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarAreaPerTimePressureData* dst = new cdm::ScalarAreaPerTimePressureData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarAreaPerTimePressure::Serialize(const SEScalarAreaPerTimePressure& src, cdm::ScalarAreaPerTimePressureData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarareapertimepressure());
}