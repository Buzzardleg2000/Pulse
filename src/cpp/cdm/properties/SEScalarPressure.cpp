/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarPressure.h"

const PressureUnit PressureUnit::Pa("Pa");
const PressureUnit PressureUnit::mmHg("mmHg");
const PressureUnit PressureUnit::cmH2O("cmH2O");
const PressureUnit PressureUnit::psi("psi");
const PressureUnit PressureUnit::atm("atm");
const PressureUnit PressureUnit::mbar("mbar");

bool PressureUnit::IsValidUnit(const std::string& unit)
{
  if (Pa.GetString().compare(unit) == 0)
    return true;
  if (mmHg.GetString().compare(unit) == 0)
    return true;
  if (cmH2O.GetString().compare(unit) == 0)
    return true;
  if (psi.GetString().compare(unit) == 0)
    return true;
  if (atm.GetString().compare(unit) == 0)
    return true;
  if (mbar.GetString().compare(unit) == 0)
    return true;
  return false;
}

const PressureUnit& PressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (Pa.GetString().compare(unit) == 0)
    return Pa;
  if (mmHg.GetString().compare(unit) == 0)
    return mmHg;
  if (cmH2O.GetString().compare(unit) == 0)
    return cmH2O;
  if (psi.GetString().compare(unit) == 0)
    return psi;
  if (atm.GetString().compare(unit) == 0)
    return atm;
  if (mbar.GetString().compare(unit) == 0)
    return mbar;
  throw CommonDataModelException(unit + " is not a valid Pressure unit");
}

template class SEScalarQuantity<PressureUnit>;

