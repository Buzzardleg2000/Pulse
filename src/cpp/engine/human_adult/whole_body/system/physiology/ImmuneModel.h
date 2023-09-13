/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/ImmuneModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL ImmuneModel : public pulse::ImmuneModel
  {
    friend class EngineTest;
  public:
    ImmuneModel(pulse::Data& data) : pulse::ImmuneModel(data) {}
    virtual ~ImmuneModel() = default;
  };
END_NAMESPACE_EX
