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
#include "patient/actions/SEChestCompression.h"

class DLL_DECL SEChestCompressionForceScale : public SEChestCompression
{
public:
  SEChestCompressionForceScale();
  virtual ~SEChestCompressionForceScale();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst);
  static cdm::ChestCompressionForceScaleData* Unload(const SEChestCompressionForceScale& src);
protected:
  static void Serialize(const cdm::ChestCompressionForceScaleData& src, SEChestCompressionForceScale& dst);
  static void Serialize(const SEChestCompressionForceScale& src, cdm::ChestCompressionForceScaleData& dst);

public:

  virtual bool HasForceScale() const;
  virtual SEScalar0To1& GetForceScale();

  virtual bool HasForcePeriod() const;
  virtual SEScalarTime& GetForcePeriod();

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*           m_ForceScale;
  SEScalarTime*               m_ForcePeriod;
};