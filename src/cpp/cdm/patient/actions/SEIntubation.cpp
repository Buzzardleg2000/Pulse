/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEIntubation.h"
#include "cdm/io/protobuf/PBPatientActions.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"

SEIntubation::SEIntubation(Logger* logger) : SEPatientAction(logger)
{
  m_Type = eIntubation_Type::Off;
  m_AirwayResistance = nullptr;
  m_Severity = nullptr;
}

SEIntubation::~SEIntubation()
{
  m_Type = eIntubation_Type::Off;
  SAFE_DELETE(m_AirwayResistance);
  SAFE_DELETE(m_Severity);
}

void SEIntubation::Clear()
{
  SEPatientAction::Clear();
  m_Type = eIntubation_Type::Off;
  INVALIDATE_PROPERTY(m_AirwayResistance);
  INVALIDATE_PROPERTY(m_Severity);
}

void SEIntubation::Copy(const SEIntubation& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEIntubation::IsValid() const
{
  return true;
}

bool SEIntubation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return GetType() != eIntubation_Type::Off;
}
void SEIntubation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEIntubation::GetScalar(const std::string& name)
{
  if (name == "AirwayResistance")
    return &GetAirwayResistance();
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

eIntubation_Type SEIntubation::GetType() const
{
  return m_Type;
}
void SEIntubation::SetType(eIntubation_Type Type)
{
  m_Type = Type;
}


bool SEIntubation::HasAirwayResistance() const
{
  return m_AirwayResistance == nullptr ? false : m_AirwayResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEIntubation::GetAirwayResistance()
{
  if (m_AirwayResistance == nullptr)
    m_AirwayResistance = new SEScalarPressureTimePerVolume();
  return *m_AirwayResistance;
}
double SEIntubation::GetAirwayResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_AirwayResistance == nullptr)
    return SEScalar::dNaN();
  return m_AirwayResistance->GetValue(unit);
}

bool SEIntubation::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEIntubation::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEIntubation::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

