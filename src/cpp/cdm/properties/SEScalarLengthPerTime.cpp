/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarLengthPerTime.h"

const LengthPerTimeUnit LengthPerTimeUnit::m_Per_s("m/s");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_s("cm/s");
const LengthPerTimeUnit LengthPerTimeUnit::m_Per_min("m/min");
const LengthPerTimeUnit LengthPerTimeUnit::cm_Per_min("cm/min");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_s("ft/s");
const LengthPerTimeUnit LengthPerTimeUnit::ft_Per_min("ft/min");
const LengthPerTimeUnit LengthPerTimeUnit::km_Per_hr("km/hr");

bool LengthPerTimeUnit::IsValidUnit(const std::string& unit)
{
  if (m_Per_s.GetString().compare(unit) == 0)
    return true;
  if (cm_Per_s.GetString().compare(unit) == 0)
    return true;
  if (m_Per_min.GetString().compare(unit) == 0)
    return true;
  if (cm_Per_min.GetString().compare(unit) == 0)
    return true;
  if (ft_Per_s.GetString().compare(unit) == 0)
    return true;
  if (ft_Per_min.GetString().compare(unit) == 0)
    return true;
  if (km_Per_hr.GetString().compare(unit) == 0)
    return true;
  return false;
}

const LengthPerTimeUnit& LengthPerTimeUnit::GetCompoundUnit(const std::string& unit)
{
  if (m_Per_s.GetString().compare(unit) == 0)
    return m_Per_s;
  if (cm_Per_s.GetString().compare(unit) == 0)
    return cm_Per_s;
  if (m_Per_min.GetString().compare(unit) == 0)
    return m_Per_min;
  if (cm_Per_min.GetString().compare(unit) == 0)
    return cm_Per_min;
  if (ft_Per_s.GetString().compare(unit) == 0)
    return ft_Per_s;
  if (ft_Per_min.GetString().compare(unit) == 0)
    return ft_Per_min;
  if (km_Per_hr.GetString().compare(unit) == 0)
    return km_Per_hr;
  throw CommonDataModelException(unit + " is not a valid LengthPerTime unit");
}

template class SEScalarQuantity<LengthPerTimeUnit>;
