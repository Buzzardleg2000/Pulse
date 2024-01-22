/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarAreaPerTimePressure.h"

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
  throw CommonDataModelException(unit + " is not a valid  AreaPerTimePressure unit");
}

template class SEScalarQuantity<AreaPerTimePressureUnit>;
