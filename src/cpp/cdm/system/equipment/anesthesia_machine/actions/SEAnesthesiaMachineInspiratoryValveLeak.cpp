/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineInspiratoryValveLeak::SEAnesthesiaMachineInspiratoryValveLeak(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineInspiratoryValveLeak::~SEAnesthesiaMachineInspiratoryValveLeak()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineInspiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineInspiratoryValveLeak::Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineInspiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineInspiratoryValveLeak::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineInspiratoryValveLeak::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineInspiratoryValveLeak::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineInspiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineInspiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineInspiratoryValveLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}
