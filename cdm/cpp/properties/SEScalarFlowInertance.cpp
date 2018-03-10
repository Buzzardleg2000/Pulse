/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarFlowInertance.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

const FlowInertanceUnit FlowInertanceUnit::mmHg_s2_Per_mL("mmHg s^2/mL");
const FlowInertanceUnit FlowInertanceUnit::mmHg_s2_Per_L("mmHg s^2/L");
const FlowInertanceUnit FlowInertanceUnit::cmH2O_s2_Per_mL("cmH2O s^2/mL");
const FlowInertanceUnit FlowInertanceUnit::cmH2O_s2_Per_L("cmH2O s^2/L");
const FlowInertanceUnit FlowInertanceUnit::Pa_s2_Per_m3("Pa s^2/m^3");

bool FlowInertanceUnit::IsValidUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowInertanceUnit& FlowInertanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (mmHg_s2_Per_mL.GetString().compare(unit) == 0)
    return mmHg_s2_Per_mL;
  if (mmHg_s2_Per_L.GetString().compare(unit) == 0)
    return mmHg_s2_Per_L;
  if (cmH2O_s2_Per_mL.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_mL;
  if (cmH2O_s2_Per_L.GetString().compare(unit) == 0)
    return cmH2O_s2_Per_L;
  if (Pa_s2_Per_m3.GetString().compare(unit) == 0)
    return Pa_s2_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowInertance unit";
  throw CommonDataModelException(err.str());
}

void SEScalarFlowInertance::Load(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst)
{
  SEScalarFlowInertance::Serialize(src, dst);
}
void SEScalarFlowInertance::Serialize(const cdm::ScalarFlowInertanceData& src, SEScalarFlowInertance& dst)
{
  SEUnitScalar::Serialize(src.scalarflowinertance(), dst);
}

cdm::ScalarFlowInertanceData* SEScalarFlowInertance::Unload(const SEScalarFlowInertance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarFlowInertanceData* dst = new cdm::ScalarFlowInertanceData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarFlowInertance::Serialize(const SEScalarFlowInertance& src, cdm::ScalarFlowInertanceData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarflowinertance());
}