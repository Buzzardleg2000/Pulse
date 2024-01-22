/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/CommonDefs.h"

#define PULSE_DECL CDM_DECL

#define PULSE_BIND pulse::engine::bind

#define END_NAMESPACE }
#define END_NAMESPACE_EX }}

#define PULSE_BIND_DECL(type) \
  namespace PULSE_BIND { class type##Data; }
  
#define pulse_DECL(type) \
  namespace pulse { class type; }

#include "engine/PulseEngine.h"
