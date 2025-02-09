/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ForceUnit : public CCompoundUnit
{
public:
  ForceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ForceUnit() {}

  // Please use static units below
  ForceUnit(const ForceUnit&) = delete;
  ForceUnit& operator= (const ForceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ForceUnit& GetCompoundUnit(const std::string& unit);

  static const ForceUnit N;
  static const ForceUnit lbf;
  static const ForceUnit dyn;
};

class CDM_DECL SEScalarForce : public SEScalarQuantity<ForceUnit>
{
public:
  SEScalarForce() {}
  virtual ~SEScalarForce() {}
};
