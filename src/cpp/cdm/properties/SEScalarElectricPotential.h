/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ElectricPotentialUnit : public CCompoundUnit
{
public:
  ElectricPotentialUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricPotentialUnit() {}

  // Please use static units below
  ElectricPotentialUnit(const ElectricPotentialUnit&) = delete;
  ElectricPotentialUnit& operator= (const ElectricPotentialUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ElectricPotentialUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricPotentialUnit V;
  static const ElectricPotentialUnit mV;
};

class CDM_DECL SEScalarElectricPotential : public SEScalarQuantity<ElectricPotentialUnit>
{
public:
  SEScalarElectricPotential() {}
  virtual ~SEScalarElectricPotential() {}
};
