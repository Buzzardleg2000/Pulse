/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"
#include "cdm/PhysiologyEnginePool.h"

class CDM_DECL PulseEnginePool : public SEPhysiologyEnginePool
{
public:
  PulseEnginePool(size_t poolSize = 0, const std::string& dataDir = "./", Logger* logger = nullptr);
  ~PulseEnginePool() = default;

  virtual  SEPhysiologyEnginePoolEngine* CreateEngine(int id, eModelType type);

protected:
  using SEPhysiologyEnginePool::CreateEngine;
  virtual void AllocateEngine(SEPhysiologyEnginePoolEngine& pe) override;

  eModelType m_creationType;
};
