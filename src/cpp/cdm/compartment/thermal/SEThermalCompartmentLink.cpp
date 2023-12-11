/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/thermal/SEThermalCompartment.h"
#include "cdm/compartment/thermal/SEThermalCompartmentLink.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"
#include "cdm/properties/SEScalarPower.h"

SEThermalCompartmentLink::SEThermalCompartmentLink(SEThermalCompartment& src, SEThermalCompartment & tgt, const std::string& name) : SECompartmentLink(name,src.GetLogger()), m_SourceCmpt(src), m_TargetCmpt(tgt)
{
  m_HeatTransferRate = nullptr;
  m_Path = nullptr;
}
SEThermalCompartmentLink::~SEThermalCompartmentLink()
{

}

void SEThermalCompartmentLink::Clear()
{
  m_Path = nullptr;
  SAFE_DELETE(m_HeatTransferRate);
}

const SEScalar* SEThermalCompartmentLink::GetScalar(const std::string& name)
{
  if (name.compare("HeatTransferRate") == 0)
    return &GetHeatTransferRate();
  return nullptr;
}

bool SEThermalCompartmentLink::HasHeatTransferRate() const
{
  if (m_Path != nullptr)
    return m_Path->HasNextHeatTransferRate();
  return m_HeatTransferRate == nullptr ? false : m_HeatTransferRate->IsValid();
}
SEScalarPower& SEThermalCompartmentLink::GetHeatTransferRate()
{
  if (m_Path != nullptr)
    return m_Path->GetNextHeatTransferRate();
  if (m_HeatTransferRate == nullptr)
    m_HeatTransferRate = new SEScalarPower();
  return *m_HeatTransferRate;
}
double SEThermalCompartmentLink::GetHeatTransferRate(const PowerUnit& unit) const
{
  if (m_Path != nullptr)
    return m_Path->GetNextHeatTransferRate(unit);
  if (m_HeatTransferRate == nullptr)
    return SEScalar::dNaN();
  return m_HeatTransferRate->GetValue(unit);
}
