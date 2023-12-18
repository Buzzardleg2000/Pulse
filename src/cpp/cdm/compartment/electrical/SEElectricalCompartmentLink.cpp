/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/electrical/SEElectricalCompartment.h"
#include "cdm/compartment/electrical/SEElectricalCompartmentLink.h"
#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"
#include "cdm/properties/SEScalarElectricCurrent.h"

SEElectricalCompartmentLink::SEElectricalCompartmentLink(SEElectricalCompartment& src, SEElectricalCompartment & tgt, const std::string& name) : SECompartmentLink(name,src.GetLogger()), m_SourceCmpt(src), m_TargetCmpt(tgt)
{
  m_Current = nullptr;
  m_Path = nullptr;
}
SEElectricalCompartmentLink::~SEElectricalCompartmentLink()
{

}

void SEElectricalCompartmentLink::Clear()
{
  m_Path = nullptr;
  SAFE_DELETE(m_Current);
}

const SEScalar* SEElectricalCompartmentLink::GetScalar(const std::string& name)
{
  if (name.compare("Current") == 0)
    return &GetCurrent();
  return nullptr;
}

bool SEElectricalCompartmentLink::HasCurrent() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextCurrent();
  return m_Current == nullptr ? false : m_Current->IsValid();
}
SEScalarElectricCurrent& SEElectricalCompartmentLink::GetCurrent()
{
  if (m_Path != nullptr)
    return m_Path->GetNextCurrent();
  if (m_Current == nullptr)
    m_Current = new SEScalarElectricCurrent();
  return *m_Current;
}
double SEElectricalCompartmentLink::GetCurrent(const ElectricCurrentUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextCurrent(unit);
  if (m_Current == nullptr)
    return SEScalar::dNaN();
  return m_Current->GetValue(unit);
}
