/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL OsmolarityUnit : public CCompoundUnit
{
public:
  OsmolarityUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~OsmolarityUnit() {}

  // Please use static units below
  OsmolarityUnit(const OsmolarityUnit&) = delete;
  OsmolarityUnit& operator= (const OsmolarityUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const OsmolarityUnit& GetCompoundUnit(const std::string& unit);

  static const OsmolarityUnit Osm_Per_L;
  static const OsmolarityUnit mOsm_Per_L;
};

class CDM_DECL SEScalarOsmolarity : public SEScalarQuantity<OsmolarityUnit>
{
public:
  SEScalarOsmolarity() {}
  virtual ~SEScalarOsmolarity() {}
};
