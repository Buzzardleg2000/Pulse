/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceManager.h"
PROTO_PUSH
#include "bind/cdm/PatientActions.pb.h"
PROTO_POP
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"

SEMechanicalVentilation::SEMechanicalVentilation() : SEPatientAction()
{
  m_State = cdm::eSwitch::Off;
  m_Flow = nullptr;
  m_Pressure = nullptr;
}

SEMechanicalVentilation::~SEMechanicalVentilation()
{
  Clear();
}

void SEMechanicalVentilation::Clear()
{
  SEPatientAction::Clear();

  m_State = cdm::eSwitch::Off;
  SAFE_DELETE(m_Flow);
  SAFE_DELETE(m_Pressure);

  DELETE_VECTOR(m_GasFractions);
  m_cGasFractions.clear();
  DELETE_VECTOR(m_Aerosols);
  m_cAerosols.clear();
}

bool SEMechanicalVentilation::IsValid() const
{
  if (GetState() == cdm::eSwitch::Off)
    return true;
  else
  {
    if (HasGasFraction())
    {
      double total = 0;
      for (const SESubstanceFraction* sf : m_cGasFractions)
      {
        total += sf->GetFractionAmount();
      }
      if (!SEScalar::IsValue(1, total))
      {
        Error("Mechanical Ventilation Gas fractions do not sum to 1");
        return false;
      }
    }
  }
  if (!HasPressure() && !HasFlow())
  {
    Error("Mechanical Ventilation must have pressure and/or flow");
    return false;
  }
  return true;
}

bool SEMechanicalVentilation::IsActive() const
{
  return GetState() == cdm::eSwitch::On;
}

void SEMechanicalVentilation::Load(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr)
{
  SEMechanicalVentilation::Serialize(src, dst, subMgr);
}
void SEMechanicalVentilation::Serialize(const cdm::MechanicalVentilationData& src, SEMechanicalVentilation& dst, const SESubstanceManager& subMgr)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState(src.state());
  if (src.has_flow())
    SEScalarVolumePerTime::Load(src.flow(), dst.GetFlow());
  if (src.has_pressure())
    SEScalarPressure::Load(src.pressure(), dst.GetPressure());

  dst.m_GasFractions.clear();
  dst.m_cGasFractions.clear();
  SESubstance* sub;
  for (int i = 0; i < src.gasfraction_size(); i++)
  {
    const cdm::SubstanceData_FractionAmountData& sfData = src.gasfraction()[i];
    sub = subMgr.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("MechanicalVentilation substance not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != cdm::eSubstance_State_Gas)
    {
      dst.Error("MechanicalVentilation substance not gas : " + sfData.name());
      continue;
    }
    SESubstanceFraction::Load(sfData, dst.GetGasFraction(*sub));
  }

  for (int i = 0; i < src.aerosol_size(); i++)
  {
    const cdm::SubstanceData_ConcentrationData& scData = src.aerosol()[i];
    sub = subMgr.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an environmental conditions aerosol that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != cdm::eSubstance_State_Liquid && sub->GetState() != cdm::eSubstance_State_Solid)
    {
      dst.Error("Ignoring an environmental conditions aerosol that is not a gas : " + scData.name());
      continue;
    }
    SESubstanceConcentration::Load(scData, dst.GetAerosol(*sub));
  }
}

cdm::MechanicalVentilationData* SEMechanicalVentilation::Unload(const SEMechanicalVentilation& src)
{
  cdm::MechanicalVentilationData* dst = new cdm::MechanicalVentilationData();
  SEMechanicalVentilation::Serialize(src, *dst);
  return dst;
}
void SEMechanicalVentilation::Serialize(const SEMechanicalVentilation& src, cdm::MechanicalVentilationData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state(src.m_State);
  if (src.HasFlow())
    dst.set_allocated_flow(SEScalarVolumePerTime::Unload(*src.m_Flow));
  if (src.HasPressure())
    dst.set_allocated_pressure(SEScalarPressure::Unload(*src.m_Pressure));
  for (SESubstanceFraction *sf : src.m_GasFractions)
    dst.mutable_gasfraction()->AddAllocated(SESubstanceFraction::Unload(*sf));
  for (SESubstanceConcentration *sc : src.m_Aerosols)
    dst.mutable_aerosol()->AddAllocated(SESubstanceConcentration::Unload(*sc));
}

cdm::eSwitch SEMechanicalVentilation::GetState() const
{
  return m_State;
}
void SEMechanicalVentilation::SetState(cdm::eSwitch state)
{
  m_State = (state == cdm::eSwitch::NullSwitch) ? cdm::eSwitch::Off : state;
}

bool SEMechanicalVentilation::HasFlow() const
{
  return m_Flow == nullptr ? false : m_Flow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilation::GetFlow()
{
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
double SEMechanicalVentilation::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}

bool SEMechanicalVentilation::HasPressure() const
{
  return m_Pressure == nullptr ? false : m_Pressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilation::GetPressure()
{
  if (m_Pressure == nullptr)
    m_Pressure = new SEScalarPressure();
  return *m_Pressure;
}
double SEMechanicalVentilation::GetPressure(const PressureUnit& unit) const
{
  if (m_Pressure == nullptr)
    return SEScalar::dNaN();
  return m_Pressure->GetValue(unit);
}

bool SEMechanicalVentilation::HasGasFraction() const
{
  return m_GasFractions.size() == 0 ? false : true;
}
bool SEMechanicalVentilation::HasGasFraction(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_GasFractions)
  {
    if (&s == &sf->GetSubstance())
      return true;
  }
  return false;
}
const std::vector<SESubstanceFraction*>& SEMechanicalVentilation::GetGasFractions()
{
  return m_GasFractions;
}
const std::vector<const SESubstanceFraction*>& SEMechanicalVentilation::GetGasFractions() const
{
  return m_cGasFractions;
}
SESubstanceFraction& SEMechanicalVentilation::GetGasFraction(SESubstance& s)
{
  for (SESubstanceFraction* sf : m_GasFractions)
  {
    if (&s == &sf->GetSubstance())
      return *sf;
  }
  SESubstanceFraction* sf = new SESubstanceFraction(s);
  sf->GetFractionAmount().SetValue(0);
  m_GasFractions.push_back(sf);
  m_cGasFractions.push_back(sf);
  return *sf;
}
const SESubstanceFraction* SEMechanicalVentilation::GetGasFraction(const SESubstance& s) const
{
  const SESubstanceFraction* sf = nullptr;
  for (unsigned int i = 0; i < m_GasFractions.size(); i++)
  {
    sf = m_GasFractions[i];
    if (&s == &sf->GetSubstance())
      return sf;
  }
  return sf;
}
void SEMechanicalVentilation::RemoveGasFraction(const SESubstance& s)
{
  const SESubstanceFraction* sf;
  for (unsigned int i = 0; i < m_GasFractions.size(); i++)
  {
    sf = m_GasFractions[i];
    if (&s == &sf->GetSubstance())
    {
      m_GasFractions.erase(m_GasFractions.begin() + i);
      m_cGasFractions.erase(m_cGasFractions.begin() + i);
      delete sf;
    }
  }
}
void SEMechanicalVentilation::RemoveGasFractions()
{
  DELETE_VECTOR(m_GasFractions);
  m_cGasFractions.clear();
}

bool SEMechanicalVentilation::HasAerosol() const
{
  return m_Aerosols.size() == 0 ? false : true;
}
bool SEMechanicalVentilation::HasAerosol(const SESubstance& substance) const
{
  for (const SESubstanceConcentration* sc : m_Aerosols)
  {
    if (&substance == &sc->GetSubstance())
      return true;
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SEMechanicalVentilation::GetAerosols()
{
  return m_Aerosols;
}
const std::vector<const SESubstanceConcentration*>& SEMechanicalVentilation::GetAerosols() const
{
  return m_cAerosols;
}
SESubstanceConcentration& SEMechanicalVentilation::GetAerosol(SESubstance& substance)
{
  for (SESubstanceConcentration* sc : m_Aerosols)
  {
    if (&substance == &sc->GetSubstance())
      return *sc;
  }
  SESubstanceConcentration* sc = new SESubstanceConcentration(substance);
  sc->GetConcentration().SetValue(0, MassPerVolumeUnit::ug_Per_L);
  m_Aerosols.push_back(sc);
  m_cAerosols.push_back(sc);
  return *sc;
}
const SESubstanceConcentration* SEMechanicalVentilation::GetAerosol(const SESubstance& substance) const
{
  const SESubstanceConcentration* sc = nullptr;
  for (unsigned int i = 0; i<m_Aerosols.size(); i++)
  {
    sc = m_Aerosols[i];
    if (&substance == &sc->GetSubstance())
      return sc;
  }
  return sc;
}
void SEMechanicalVentilation::RemoveAerosol(const SESubstance& substance)
{
  const SESubstanceConcentration* sc;
  for (unsigned int i = 0; i<m_Aerosols.size(); i++)
  {
    sc = m_Aerosols[i];
    if (&substance == &sc->GetSubstance())
    {
      m_Aerosols.erase(m_Aerosols.begin() + i);
      m_cAerosols.erase(m_cAerosols.begin() + i);
      delete sc;
    }
  }
}
void SEMechanicalVentilation::RemoveAerosols()
{
  DELETE_VECTOR(m_Aerosols);
  m_cAerosols.clear();
}

void SEMechanicalVentilation::ToString(std::ostream &str) const
{
  str << "Patient Action : Mechanical Ventilation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;

  str << "\n\tState: " << cdm::eSwitch_Name(GetState());
  str << "\n\tFlow: ";  HasFlow() ? str << *m_Flow : str << "Not Set";
  str << "\n\tPressure: "; HasPressure() ? str << *m_Pressure : str << "Not Set";
  if (HasGasFraction())
  {
    for (const SESubstanceFraction* sf : GetGasFractions())
    {
      str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
    }
  }
  if (HasAerosol())
  {
    for (const SESubstanceConcentration* sc : GetAerosols())
    {
      str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration (mg_Per_L) " << sc->GetConcentration(MassPerVolumeUnit::mg_Per_L);
    }
  }
  str << std::flush;
}