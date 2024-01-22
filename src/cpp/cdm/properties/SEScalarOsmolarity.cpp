/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarOsmolarity.h"

const OsmolarityUnit OsmolarityUnit::Osm_Per_L("Osm/L");
const OsmolarityUnit OsmolarityUnit::mOsm_Per_L("mOsm/L");

bool OsmolarityUnit::IsValidUnit(const std::string& unit)
{
  if (Osm_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mOsm_Per_L.GetString().compare(unit) == 0)
    return true;
  return false;
}

const OsmolarityUnit& OsmolarityUnit::GetCompoundUnit(const std::string& unit)
{
  if (Osm_Per_L.GetString().compare(unit) == 0)
    return Osm_Per_L;
  if (mOsm_Per_L.GetString().compare(unit) == 0)
    return mOsm_Per_L;
  throw CommonDataModelException(unit + " is not a valid Osmolarity unit");
}

template class SEScalarQuantity<OsmolarityUnit>;
