/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include "stdafx.h"
#include "properties/SEScalarPowerPerAreaTemperatureToTheFourth.h"

const PowerPerAreaTemperatureToTheFourthUnit PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4("W/ m^2 K^4");

bool PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(const std::string& unit)
{
  if (W_Per_m2_K4.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PowerPerAreaTemperatureToTheFourthUnit& PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(const std::string& unit)
{
  if (W_Per_m2_K4.GetString().compare(unit) == 0)
    return W_Per_m2_K4;
  std::stringstream err;
  err << unit << " is not a valid PowerPerAreaTemperatureToTheFourth unit";
  throw CommonDataModelException(err.str());
}

void SEScalarPowerPerAreaTemperatureToTheFourth::Load(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  SEScalarPowerPerAreaTemperatureToTheFourth::Serialize(src, dst);
}
void SEScalarPowerPerAreaTemperatureToTheFourth::Serialize(const cdm::ScalarPowerPerAreaTemperatureToTheFourthData& src, SEScalarPowerPerAreaTemperatureToTheFourth& dst)
{
  SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::Serialize(src.scalarpowerperareatemperaturetothefourth(), dst);
}

cdm::ScalarPowerPerAreaTemperatureToTheFourthData* SEScalarPowerPerAreaTemperatureToTheFourth::Unload(const SEScalarPowerPerAreaTemperatureToTheFourth& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarPowerPerAreaTemperatureToTheFourthData* dst = new cdm::ScalarPowerPerAreaTemperatureToTheFourthData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarPowerPerAreaTemperatureToTheFourth::Serialize(const SEScalarPowerPerAreaTemperatureToTheFourth& src, cdm::ScalarPowerPerAreaTemperatureToTheFourthData& dst)
{
  SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::Serialize(src, *dst.mutable_scalarpowerperareatemperaturetothefourth());
}