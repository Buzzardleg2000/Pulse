/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/SESystem.h"

class CDM_DECL SEImmuneSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEImmuneSystem(Logger* logger);
  virtual ~SEImmuneSystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);
  
protected:

  
};