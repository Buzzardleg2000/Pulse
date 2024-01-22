/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarFrequency.h"

const FrequencyUnit FrequencyUnit::Per_min("1/min");
const FrequencyUnit FrequencyUnit::Per_s("1/s");
const FrequencyUnit FrequencyUnit::Hz("Hz");

bool FrequencyUnit::IsValidUnit(const std::string& unit)
{
  if (Per_min.GetString().compare(unit) == 0)
    return true;
  if (Per_s.GetString().compare(unit) == 0)
    return true;
  if (Hz.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FrequencyUnit& FrequencyUnit::GetCompoundUnit(const std::string& unit)
{
  if (Per_min.GetString().compare(unit) == 0)
    return Per_min;
  if (Per_s.GetString().compare(unit) == 0)
    return Per_s;
  if (Hz.GetString().compare(unit) == 0)
    return Hz;
  throw CommonDataModelException(unit + " is not a valid Frequency unit");
}

template class SEScalarQuantity<FrequencyUnit>;
