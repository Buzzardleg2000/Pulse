/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "scenario/SECondition.h"
#include "substance/SESubstanceManager.h"
#include "bind/cdm/Scenario.pb.h"

SEScenarioInitialParameters::SEScenarioInitialParameters(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
{
  m_Patient = nullptr;
  Clear();
}

SEScenarioInitialParameters::~SEScenarioInitialParameters()
{
  Clear();
}

void SEScenarioInitialParameters::Clear()
{
  m_PatientFile = "";
  SAFE_DELETE(m_Patient);
  DELETE_VECTOR(m_Conditions);
}

void SEScenarioInitialParameters::Load(const cdm::ScenarioData_InitialParametersData& src, SEScenarioInitialParameters& dst)
{
  SEScenarioInitialParameters::Serialize(src, dst);
}
void SEScenarioInitialParameters::Serialize(const cdm::ScenarioData_InitialParametersData& src, SEScenarioInitialParameters& dst)
{
  dst.Clear();
 
  if (src.has_patient())
    SEPatient::Load(src.patient(), dst.GetPatient());
  else
    dst.SetPatientFile(src.patientfile());

  for (int i=0; i<src.anycondition_size(); i++)
  {
    SECondition* c = SECondition::Load(src.anycondition()[i], dst.m_SubMgr);
    if (c != nullptr)
      dst.m_Conditions.push_back(c);
  }
}

cdm::ScenarioData_InitialParametersData* SEScenarioInitialParameters::Unload(const SEScenarioInitialParameters& src)
{
  cdm::ScenarioData_InitialParametersData* dst = new cdm::ScenarioData_InitialParametersData();
  SEScenarioInitialParameters::Serialize(src,*dst);
  return dst;
}
void SEScenarioInitialParameters::Serialize(const SEScenarioInitialParameters& src, cdm::ScenarioData_InitialParametersData& dst)
{
  if (src.HasPatientFile())
    dst.set_patientfile(src.m_PatientFile);
  else if (src.HasPatient())
    dst.set_allocated_patient(SEPatient::Unload(*src.m_Patient));
  for (SECondition* c : src.m_Conditions)
    dst.mutable_anycondition()->AddAllocated(SECondition::Unload(*c));
}

bool SEScenarioInitialParameters::IsValid() const
{
  if(!HasPatientFile() && HasPatient())
    return false;
  return true;
}


std::string SEScenarioInitialParameters::GetPatientFile() const
{
  return m_PatientFile;
}
void SEScenarioInitialParameters::SetPatientFile(const std::string& patientFile)
{
  InvalidatePatient();
  m_PatientFile = patientFile;
}
bool SEScenarioInitialParameters::HasPatientFile() const
{
  return m_PatientFile.empty()?false:true;
}
void SEScenarioInitialParameters::InvalidatePatientFile()
{
  m_PatientFile = "";
}

SEPatient& SEScenarioInitialParameters::GetPatient()
{
  InvalidatePatientFile(); 
  if (m_Patient == nullptr)
    m_Patient = new SEPatient(GetLogger());
  return *m_Patient;
}
const SEPatient* SEScenarioInitialParameters::GetPatient() const 
{
  return m_Patient;
}
void SEScenarioInitialParameters::CopyPatient(const SEPatient& patient)
{
  cdm::PatientData* p = SEPatient::Unload(patient);
  SEPatient::Load(*p, GetPatient());
  delete p;
}
bool SEScenarioInitialParameters::HasPatient() const
{
  return m_Patient != nullptr;
}
void SEScenarioInitialParameters::InvalidatePatient()
{
    SAFE_DELETE(m_Patient);
}

void SEScenarioInitialParameters::CopyCondition(const SECondition& condition)
{
  cdm::AnyConditionData* c = SECondition::Unload(condition);
  m_Conditions.push_back(SECondition::Load(*c,m_SubMgr));
  delete c;
}
const std::vector<SECondition*>& SEScenarioInitialParameters::GetConditions() const
{
  return m_Conditions;
}
