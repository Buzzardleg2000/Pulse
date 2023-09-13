/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/physiology/SEImmuneSystem.h"

SEImmuneSystem::SEImmuneSystem(Logger* logger) : SESystem(logger)
{
  
}

SEImmuneSystem::~SEImmuneSystem()
{

}

void SEImmuneSystem::Clear()
{
  SESystem::Clear();

}

const SEScalar* SEImmuneSystem::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}
