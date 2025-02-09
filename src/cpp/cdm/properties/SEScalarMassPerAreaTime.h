/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL MassPerAreaTimeUnit : public CCompoundUnit
{
public:
  MassPerAreaTimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~MassPerAreaTimeUnit() {}

  // Please use static units below
  MassPerAreaTimeUnit(const MassPerAreaTimeUnit&) = delete;
  MassPerAreaTimeUnit& operator= (const MassPerAreaTimeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const MassPerAreaTimeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerAreaTimeUnit g_Per_cm2_s;
};

class CDM_DECL SEScalarMassPerAreaTime : public SEScalarQuantity<MassPerAreaTimeUnit>
{
public:
  SEScalarMassPerAreaTime() {}
  virtual ~SEScalarMassPerAreaTime() {}
};

