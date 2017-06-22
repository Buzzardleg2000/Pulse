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

class DLL_DECL SEScalar0To1 : public SEScalar
{

public:
  SEScalar0To1();
  virtual ~SEScalar0To1() {}

  static void Load(const cdm::Scalar0To1Data& src, SEScalar0To1& dst);
  static cdm::Scalar0To1Data* Unload(const SEScalar0To1& src);
protected:
  static void Serialize(const cdm::Scalar0To1Data& src, SEScalar0To1& dst);
  static void Serialize(const SEScalar0To1& src, cdm::Scalar0To1Data& dst);
public:

  double GetValue() const { return SEScalar::GetValue(); }
  double GetValue(const NoUnit& unitless) const { return SEScalar::GetValue(); }
  void SetValue(double d);
  void SetValue(double d, const NoUnit& unitless);
};

