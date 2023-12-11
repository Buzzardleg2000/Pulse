/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarTemperature.h"

const TemperatureUnit TemperatureUnit::F("degF");
const TemperatureUnit TemperatureUnit::C("degC");
const TemperatureUnit TemperatureUnit::K("K");
const TemperatureUnit TemperatureUnit::R("degR");

bool TemperatureUnit::IsValidUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return true;
  if (C.GetString().compare(unit) == 0)
    return true;
  if (K.GetString().compare(unit) == 0)
    return true;
  if (R.GetString().compare(unit) == 0)
    return true;
  return false;
}

const TemperatureUnit& TemperatureUnit::GetCompoundUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return F;
  if (C.GetString().compare(unit) == 0)
    return C;
  if (K.GetString().compare(unit) == 0)
    return K;
  if (R.GetString().compare(unit) == 0)
    return R;
  throw CommonDataModelException(unit + " is not a valid Temperature unit");
}

double SEScalarTemperature::GetValue(const TemperatureUnit& unit) const
{
  if (m_isnan)
    throw CommonDataModelException("Value is NaN");
  if (m_isinf)
    return m_value;
  if (m_unit == &unit)
    return m_value;
  return Convert(m_value, *m_unit, unit);
}

template class SEScalarQuantity<TemperatureUnit>;
