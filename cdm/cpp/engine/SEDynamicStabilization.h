/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEEngineStabilization.h"
class SEDynamicStabilizationEngineConvergence;

class CDM_DECL SEDynamicStabilization : public SEEngineStabilization
{
  friend class PBEngine;//friend the serialization class
public:
  SEDynamicStabilization(Logger* logger);
  virtual ~SEDynamicStabilization();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

  virtual bool StabilizeRestingState(PhysiologyEngine& engine);
  virtual bool StabilizeFeedbackState(PhysiologyEngine& engine);
  virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions);

  virtual SEDynamicStabilizationEngineConvergence& GetRestingConvergence();
  virtual const SEDynamicStabilizationEngineConvergence& GetRestingConvergence() const;

  virtual bool HasFeedbackConvergence() const;
  virtual SEDynamicStabilizationEngineConvergence& GetFeedbackConvergence();
  virtual const SEDynamicStabilizationEngineConvergence* GetFeedbackConvergence() const;

  virtual bool HasConditionConvergence(const std::string& name) const;
  virtual void RemoveConditionConvergence(const std::string& name);
  virtual SEDynamicStabilizationEngineConvergence& GetConditionConvergence(const std::string& name);
  virtual const SEDynamicStabilizationEngineConvergence* GetConditionConvergence(const std::string& name) const;
  virtual const std::map<std::string, SEDynamicStabilizationEngineConvergence*>& GetConditionConvergence() const;

protected:

  virtual bool Stabilize(PhysiologyEngine& engine, const SEDynamicStabilizationEngineConvergence& criteria);

  SEDynamicStabilizationEngineConvergence* m_RestingConvergence;
  SEDynamicStabilizationEngineConvergence* m_FeedbackConvergence;
  std::map<std::string, SEDynamicStabilizationEngineConvergence*> m_ConditionConvergence;

  bool Merge();
  SEDynamicStabilizationEngineConvergence* m_MergedConditions;
  std::map<std::string, SEDynamicStabilizationEngineConvergence*> m_ActiveConditions;
};

