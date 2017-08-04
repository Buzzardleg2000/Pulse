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
#include "scenario/SECondition.h"
#include "bind/cdm/EnvironmentConditions.pb.h"

class DLL_DECL SEEnvironmentCondition : public SECondition
{
public:

  SEEnvironmentCondition();
  virtual ~SEEnvironmentCondition();

  virtual void Clear();

  virtual bool IsValid() const;

  /** Create a new condition based on the binding object, load that data into the new condition, and return said condition */
  static SEEnvironmentCondition* Load(const cdm::AnyEnvironmentConditionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyEnvironmentConditionData* Unload(const SEEnvironmentCondition& condition);
protected:
  static void Serialize(const cdm::EnvironmentConditionData& src, SEEnvironmentCondition& dst);
  static void Serialize(const SEEnvironmentCondition& src, cdm::EnvironmentConditionData& dst);

public:
  virtual void ToString(std::ostream &str) const = 0;
};  
