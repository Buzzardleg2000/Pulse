/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarInversePressure.h"

const InversePressureUnit InversePressureUnit::Inverse_Pa("1/Pa");
const InversePressureUnit InversePressureUnit::Inverse_mmHg("1/mmHg");
const InversePressureUnit InversePressureUnit::Inverse_cmH2O("1/cmH2O");
const InversePressureUnit InversePressureUnit::Inverse_atm("1/atm");

bool InversePressureUnit::IsValidUnit(const std::string& unit)
{
  if (Inverse_Pa.GetString().compare(unit) == 0)
    return true;
  if (Inverse_mmHg.GetString().compare(unit) == 0)
    return true;
  if (Inverse_cmH2O.GetString().compare(unit) == 0)
    return true;
  if (Inverse_atm.GetString().compare(unit) == 0)
    return true;
  return false;
}

const InversePressureUnit& InversePressureUnit::GetCompoundUnit(const std::string& unit)
{
  if (Inverse_Pa.GetString().compare(unit) == 0)
    return Inverse_Pa;
  if (Inverse_mmHg.GetString().compare(unit) == 0)
    return Inverse_mmHg;
  if (Inverse_cmH2O.GetString().compare(unit) == 0)
    return Inverse_cmH2O;
  if (Inverse_atm.GetString().compare(unit) == 0)
    return Inverse_atm;
  throw CommonDataModelException(unit + " is not a valid InversePressure unit");
}

template class SEScalarQuantity<InversePressureUnit>;
