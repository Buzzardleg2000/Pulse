/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eSubstanceAdministration_Route {
  Intravenous = 0,
  Epidural,
  Intraosseous,
  Intraarterial,
  Intracardiac,
  Intracerebral,
  Intracerebroventricular,
  Intradermal,
  Intramuscular,
  Subcutaneous
};
extern CDM_DECL const std::string& eSubstanceAdministration_Route_Name(eSubstanceAdministration_Route m);
