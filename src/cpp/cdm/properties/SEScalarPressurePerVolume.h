/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL PressurePerVolumeUnit : public CCompoundUnit
{
public:
  PressurePerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressurePerVolumeUnit() {}

  // Please use static units below
  PressurePerVolumeUnit(const PressurePerVolumeUnit&) = delete;
  PressurePerVolumeUnit& operator= (const PressurePerVolumeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const PressurePerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const PressurePerVolumeUnit mmHg_Per_mL;
  static const PressurePerVolumeUnit cmH2O_Per_mL;
  static const PressurePerVolumeUnit cmH2O_Per_L;
};

class CDM_DECL SEScalarPressurePerVolume : public SEScalarQuantity<PressurePerVolumeUnit>
{
public:
  SEScalarPressurePerVolume() {}
  virtual ~SEScalarPressurePerVolume() {}
};
