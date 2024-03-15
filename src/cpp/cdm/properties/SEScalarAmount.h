/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL AmountUnit : public CCompoundUnit
{
public:
  AmountUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountUnit() {}

  // Please use static units below
  AmountUnit(const AmountUnit&) = delete;
  AmountUnit& operator= (const AmountUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const AmountUnit& GetCompoundUnit(const std::string& unit);

  static const AmountUnit mol;
  static const AmountUnit pmol;
  static const AmountUnit ct;
};

class CDM_DECL SEScalarAmount : public SEScalarQuantity<AmountUnit>
{
public:
  SEScalarAmount() {}
  virtual ~SEScalarAmount() {}
};
