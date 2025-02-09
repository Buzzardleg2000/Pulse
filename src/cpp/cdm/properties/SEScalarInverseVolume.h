/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL InverseVolumeUnit : public CCompoundUnit
{
public:
  InverseVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~InverseVolumeUnit() {}

  // Please use static units below
  InverseVolumeUnit(const InverseVolumeUnit&) = delete;
  InverseVolumeUnit& operator= (const InverseVolumeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const InverseVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const InverseVolumeUnit Inverse_L;
  static const InverseVolumeUnit Inverse_mL;
};

class CDM_DECL SEScalarInverseVolume : public SEScalarQuantity<InverseVolumeUnit>
{
public:
  SEScalarInverseVolume() {}
  virtual ~SEScalarInverseVolume() {}
};
