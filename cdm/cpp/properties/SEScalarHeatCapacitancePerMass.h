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

class DLL_DECL HeatCapacitancePerMassUnit : public CCompoundUnit
{
public:
  HeatCapacitancePerMassUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatCapacitancePerMassUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitancePerMassUnit& GetCompoundUnit(const std::string& unit);

  static const HeatCapacitancePerMassUnit J_Per_K_kg;
  static const HeatCapacitancePerMassUnit kJ_Per_K_kg;
  static const HeatCapacitancePerMassUnit kcal_Per_K_kg;
  static const HeatCapacitancePerMassUnit kcal_Per_C_kg;
};

class DLL_DECL SEScalarHeatCapacitancePerMass : public SEScalarQuantity<HeatCapacitancePerMassUnit>
{
public:
  SEScalarHeatCapacitancePerMass() {}
  virtual ~SEScalarHeatCapacitancePerMass() {}

  static void Load(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst);
  static cdm::ScalarHeatCapacitancePerMassData* Unload(const SEScalarHeatCapacitancePerMass& src);
protected:
  static void Serialize(const cdm::ScalarHeatCapacitancePerMassData& src, SEScalarHeatCapacitancePerMass& dst);
  static void Serialize(const SEScalarHeatCapacitancePerMass& src, cdm::ScalarHeatCapacitancePerMassData& dst);
};