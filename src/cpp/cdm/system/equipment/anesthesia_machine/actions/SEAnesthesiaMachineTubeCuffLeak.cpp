/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineTubeCuffLeak::SEAnesthesiaMachineTubeCuffLeak(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineTubeCuffLeak::~SEAnesthesiaMachineTubeCuffLeak()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineTubeCuffLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineTubeCuffLeak::Copy(const SEAnesthesiaMachineTubeCuffLeak& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineTubeCuffLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineTubeCuffLeak::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineTubeCuffLeak::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineTubeCuffLeak::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineTubeCuffLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineTubeCuffLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineTubeCuffLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}
