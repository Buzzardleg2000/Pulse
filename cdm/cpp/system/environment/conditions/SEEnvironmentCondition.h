/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "scenario/SECondition.h"
CDM_BIND_DECL(EnvironmentConditionData)
CDM_BIND_DECL(AnyEnvironmentConditionData)

class CDM_DECL SEEnvironmentCondition : public SECondition
{
public:

  SEEnvironmentCondition();
  virtual ~SEEnvironmentCondition();

  virtual void Clear();

  virtual bool IsValid() const = 0;
  virtual bool IsActive() const = 0;

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
