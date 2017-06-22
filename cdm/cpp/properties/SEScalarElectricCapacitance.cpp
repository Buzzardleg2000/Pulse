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
#include "properties/SEScalarElectricCapacitance.h"

const ElectricCapacitanceUnit ElectricCapacitanceUnit::F("F");

bool ElectricCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricCapacitanceUnit& ElectricCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return F;
  std::stringstream err;
  err << unit << " is not a valid ElectricCapacitance unit";
  throw CommonDataModelException(err.str());
}

void SEScalarElectricCapacitance::Load(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  SEScalarElectricCapacitance::Serialize(src, dst);
}
void SEScalarElectricCapacitance::Serialize(const cdm::ScalarElectricCapacitanceData& src, SEScalarElectricCapacitance& dst)
{
  SEScalarQuantity<ElectricCapacitanceUnit>::Serialize(src.scalarelectriccapacitance(), dst);
}

cdm::ScalarElectricCapacitanceData* SEScalarElectricCapacitance::Unload(const SEScalarElectricCapacitance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricCapacitanceData* dst = new cdm::ScalarElectricCapacitanceData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarElectricCapacitance::Serialize(const SEScalarElectricCapacitance& src, cdm::ScalarElectricCapacitanceData& dst)
{
  SEScalarQuantity<ElectricCapacitanceUnit>::Serialize(src, *dst.mutable_scalarelectriccapacitance());
}