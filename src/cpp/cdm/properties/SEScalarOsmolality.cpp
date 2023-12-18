/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarOsmolality.h"

const OsmolalityUnit OsmolalityUnit::Osm_Per_kg("Osm/kg");
const OsmolalityUnit OsmolalityUnit::mOsm_Per_kg("mOsm/kg");

bool OsmolalityUnit::IsValidUnit(const std::string& unit)
{
  if (Osm_Per_kg.GetString().compare(unit) == 0)
    return true;
  if (mOsm_Per_kg.GetString().compare(unit) == 0)
    return true;
  return false;
}

const OsmolalityUnit& OsmolalityUnit::GetCompoundUnit(const std::string& unit)
{
  if (Osm_Per_kg.GetString().compare(unit) == 0)
    return Osm_Per_kg;
  if (mOsm_Per_kg.GetString().compare(unit) == 0)
    return mOsm_Per_kg;
  throw CommonDataModelException(unit + " is not a valid Osmolality unit");
}

template class SEScalarQuantity<OsmolalityUnit>;
