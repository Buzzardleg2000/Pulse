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

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL VolumePerTimeMassUnit : public CCompoundUnit
{
public:
  VolumePerTimeMassUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~VolumePerTimeMassUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeMassUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeMassUnit L_Per_s_g;
  static const VolumePerTimeMassUnit mL_Per_s_g;
  static const VolumePerTimeMassUnit mL_Per_min_kg;
  static const VolumePerTimeMassUnit mL_Per_s_kg;
  static const VolumePerTimeMassUnit uL_Per_min_kg;
};

class CDM_DECL SEScalarVolumePerTimeMass : public SEScalarQuantity<VolumePerTimeMassUnit>
{
public:
  SEScalarVolumePerTimeMass() {}
  virtual ~SEScalarVolumePerTimeMass() {}

  static void Load(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst);
  static cdm::ScalarVolumePerTimeMassData* Unload(const SEScalarVolumePerTimeMass& src);
protected:
  static void Serialize(const cdm::ScalarVolumePerTimeMassData& src, SEScalarVolumePerTimeMass& dst);
  static void Serialize(const SEScalarVolumePerTimeMass& src, cdm::ScalarVolumePerTimeMassData& dst);
};
