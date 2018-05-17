/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
PROTO_PUSH
#include "bind/cdm/PatientConditions.pb.h"
PROTO_POP
#include "properties/SEScalar0To1.h"

SEChronicRenalStenosis::SEChronicRenalStenosis() : SEPatientCondition()
{
  m_LeftKidneySeverity = nullptr;
  m_RightKidneySeverity = nullptr;
}

SEChronicRenalStenosis::~SEChronicRenalStenosis()
{
  Clear();
}

void SEChronicRenalStenosis::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_LeftKidneySeverity);
  SAFE_DELETE(m_RightKidneySeverity);
}

bool SEChronicRenalStenosis::IsValid() const
{
  return SEPatientCondition::IsValid() && (HasLeftKidneySeverity() || HasRightKidneySeverity());
}

void SEChronicRenalStenosis::Load(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  SEChronicRenalStenosis::Serialize(src, dst);
}
void SEChronicRenalStenosis::Serialize(const cdm::ChronicRenalStenosisData& src, SEChronicRenalStenosis& dst)
{
  SEPatientCondition::Serialize(src.patientcondition(), dst);
  if (src.has_leftkidneyseverity())
    SEScalar0To1::Load(src.leftkidneyseverity(), dst.GetLeftKidneySeverity());
  if (src.has_rightkidneyseverity())
    SEScalar0To1::Load(src.rightkidneyseverity(), dst.GetRightKidneySeverity());
}

cdm::ChronicRenalStenosisData* SEChronicRenalStenosis::Unload(const SEChronicRenalStenosis& src)
{
  cdm::ChronicRenalStenosisData* dst = new cdm::ChronicRenalStenosisData();
  SEChronicRenalStenosis::Serialize(src, *dst);
  return dst;
}
void SEChronicRenalStenosis::Serialize(const SEChronicRenalStenosis& src, cdm::ChronicRenalStenosisData& dst)
{
  SEPatientCondition::Serialize(src, *dst.mutable_patientcondition());
  if (src.HasLeftKidneySeverity())
    dst.set_allocated_leftkidneyseverity(SEScalar0To1::Unload(*src.m_LeftKidneySeverity));
  if (src.HasRightKidneySeverity())
    dst.set_allocated_rightkidneyseverity(SEScalar0To1::Unload(*src.m_RightKidneySeverity));
}

bool SEChronicRenalStenosis::HasLeftKidneySeverity() const
{
  return m_LeftKidneySeverity == nullptr ? false : m_LeftKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetLeftKidneySeverity()
{
  if (m_LeftKidneySeverity == nullptr)
    m_LeftKidneySeverity = new SEScalar0To1();
  return *m_LeftKidneySeverity;
}
double SEChronicRenalStenosis::GetLeftKidneySeverity() const
{
  if (m_LeftKidneySeverity == nullptr)
    return SEScalar::dNaN();
  return m_LeftKidneySeverity->GetValue();
}

bool SEChronicRenalStenosis::HasRightKidneySeverity() const
{
  return m_RightKidneySeverity == nullptr ? false : m_RightKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetRightKidneySeverity()
{
  if (m_RightKidneySeverity == nullptr)
    m_RightKidneySeverity = new SEScalar0To1();
  return *m_RightKidneySeverity;
}
double SEChronicRenalStenosis::GetRightKidneySeverity() const
{
  if (m_RightKidneySeverity == nullptr)
    return SEScalar::dNaN();
  return m_RightKidneySeverity->GetValue();
}

void SEChronicRenalStenosis::ToString(std::ostream &str) const
{
  str << "Patient Condition : Chronic Renal Stenosis";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tLeft Kidney Occlusion 0To1: "; HasLeftKidneySeverity() ? str << *m_LeftKidneySeverity : str << "NaN";
  str << "\n\tRight Kidney Occlusion 0To1: "; HasRightKidneySeverity() ? str << *m_RightKidneySeverity : str << "NaN";
  str << std::flush;
}