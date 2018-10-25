/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarFlowResistance.h"

const FlowResistanceUnit FlowResistanceUnit::cmH2O_s_Per_L("cmH2O s/L");
const FlowResistanceUnit FlowResistanceUnit::mmHg_s_Per_mL("mmHg s/mL");
const FlowResistanceUnit FlowResistanceUnit::mmHg_min_Per_mL("mmHg min/mL");
const FlowResistanceUnit FlowResistanceUnit::mmHg_min_Per_L("mmHg min/L");
const FlowResistanceUnit FlowResistanceUnit::Pa_s_Per_m3("Pa s/m^3");

bool FlowResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (cmH2O_s_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmHg_s_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_min_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (mmHg_min_Per_L.GetString().compare(unit) == 0)
    return true;
  if (Pa_s_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowResistanceUnit& FlowResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (cmH2O_s_Per_L.GetString().compare(unit) == 0)
    return cmH2O_s_Per_L;
  if (mmHg_s_Per_mL.GetString().compare(unit) == 0)
    return mmHg_s_Per_mL;
  if (mmHg_min_Per_mL.GetString().compare(unit) == 0)
    return mmHg_min_Per_mL;
  if (mmHg_min_Per_L.GetString().compare(unit) == 0)
    return mmHg_min_Per_L;
  if (Pa_s_Per_m3.GetString().compare(unit) == 0)
    return Pa_s_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowResistance unit";
  throw CommonDataModelException(err.str());
}
