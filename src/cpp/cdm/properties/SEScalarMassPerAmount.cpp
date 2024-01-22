/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEScalarMassPerAmount.h"

const MassPerAmountUnit MassPerAmountUnit::g_Per_ct("g/ct");
const MassPerAmountUnit MassPerAmountUnit::g_Per_mol("g/mol");
const MassPerAmountUnit MassPerAmountUnit::g_Per_mmol("g/mmol");
const MassPerAmountUnit MassPerAmountUnit::g_Per_umol("g/umol");
const MassPerAmountUnit MassPerAmountUnit::mg_Per_mol("mg/mol");
const MassPerAmountUnit MassPerAmountUnit::mg_Per_mmol("mg/mmol");
const MassPerAmountUnit MassPerAmountUnit::kg_Per_mol("kg/mol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_mmol("ug/mmol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_mol("ug/mol");
const MassPerAmountUnit MassPerAmountUnit::ug_Per_ct("ug/ct");
const MassPerAmountUnit MassPerAmountUnit::pg_Per_ct("pg/ct");

bool MassPerAmountUnit::IsValidUnit(const std::string& unit)
{
  if (g_Per_ct.GetString().compare(unit) == 0)
    return true;
  if (g_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (g_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (g_Per_umol.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (mg_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (kg_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_mmol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_mol.GetString().compare(unit) == 0)
    return true;
  if (ug_Per_ct.GetString().compare(unit) == 0)
    return true;
  if (pg_Per_ct.GetString().compare(unit) == 0)
    return true;
  return false;
}

const MassPerAmountUnit& MassPerAmountUnit::GetCompoundUnit(const std::string& unit)
{
  if (g_Per_ct.GetString().compare(unit) == 0)
    return g_Per_ct;
  if (g_Per_mol.GetString().compare(unit) == 0)
    return g_Per_mol;
  if (g_Per_mmol.GetString().compare(unit) == 0)
    return g_Per_mmol;
  if (g_Per_umol.GetString().compare(unit) == 0)
    return g_Per_umol;
  if (mg_Per_mol.GetString().compare(unit) == 0)
    return mg_Per_mol;
  if (mg_Per_mmol.GetString().compare(unit) == 0)
    return mg_Per_mmol;
  if (kg_Per_mol.GetString().compare(unit) == 0)
    return kg_Per_mol;
  if (ug_Per_mmol.GetString().compare(unit) == 0)
    return ug_Per_mmol;
  if (ug_Per_mol.GetString().compare(unit) == 0)
    return ug_Per_mol;
  if (ug_Per_ct.GetString().compare(unit) == 0)
    return ug_Per_ct;
  if (pg_Per_ct.GetString().compare(unit) == 0)
    return pg_Per_ct;
  throw CommonDataModelException(unit + " is not a valid MassPerAmount unit");
}

template class SEScalarQuantity<MassPerAmountUnit>;
