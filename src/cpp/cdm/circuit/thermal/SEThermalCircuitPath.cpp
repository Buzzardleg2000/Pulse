/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"
#include "cdm/properties/SEScalarHeatResistance.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"
#include "cdm/properties/SEScalarHeatInductance.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarEnergy.h"

SEThermalCircuitPath::SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name) :
  SECircuitPath<THERMAL_CIRCUIT_PATH>(src, tgt ,name),
  m_ThermalSourceNode(src), m_ThermalTargetNode(tgt)
{

}

SEThermalCircuitPath::~SEThermalCircuitPath()
{
  Clear();
}

void SEThermalCircuitPath::Clear()
{
  SECircuitPath::Clear();
}


SEThermalCircuitNode& SEThermalCircuitPath::GetSourceNode() const { return m_ThermalSourceNode; }
SEThermalCircuitNode& SEThermalCircuitPath::GetTargetNode() const { return m_ThermalTargetNode; }

////////////////////////////////
// Thermal Resistance Types//
////////////////////////////////

bool SEThermalCircuitPath::HasResistance() const
{
  return SECircuitPath::HasResistance();
}
SEScalarHeatResistance& SEThermalCircuitPath::GetResistance()
{
  return SECircuitPath::GetResistance();
}
double SEThermalCircuitPath::GetResistance(const HeatResistanceUnit& unit) const
{
  if (m_Resistance == nullptr)
    return SEScalar::dNaN();
  return m_Resistance->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextResistance() const
{
  return SECircuitPath::HasNextResistance();
}
SEScalarHeatResistance& SEThermalCircuitPath::GetNextResistance()
{
  return SECircuitPath::GetNextResistance();
}
double SEThermalCircuitPath::GetNextResistance(const HeatResistanceUnit& unit) const
{
  if (m_NextResistance == nullptr)
    return SEScalar::dNaN();
  return m_NextResistance->GetValue(unit);
}
bool SEThermalCircuitPath::HasResistanceBaseline() const
{
  return SECircuitPath::HasResistanceBaseline();
}
SEScalarHeatResistance& SEThermalCircuitPath::GetResistanceBaseline()
{
  return SECircuitPath::GetResistanceBaseline();
}
double SEThermalCircuitPath::GetResistanceBaseline(const HeatResistanceUnit& unit) const
{
  if (m_ResistanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_ResistanceBaseline->GetValue(unit);
}
void SEThermalCircuitPath::RemoveResistance()
{
  SECircuitPath::RemoveResistance();
}

//////////////////////////////////
// Thermal Capacitance Types //
//////////////////////////////////

bool SEThermalCircuitPath::HasCapacitance() const
{
  return SECircuitPath::HasCapacitance();
}
SEScalarHeatCapacitance& SEThermalCircuitPath::GetCapacitance()
{
  return SECircuitPath::GetCapacitance();
}
double SEThermalCircuitPath::GetCapacitance(const HeatCapacitanceUnit& unit) const
{
  if (m_Capacitance == nullptr)
    return SEScalar::dNaN();
  return m_Capacitance->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextCapacitance() const
{
  return SECircuitPath::HasNextCapacitance();
}
SEScalarHeatCapacitance& SEThermalCircuitPath::GetNextCapacitance()
{
  return SECircuitPath::GetNextCapacitance();
}
double SEThermalCircuitPath::GetNextCapacitance(const HeatCapacitanceUnit& unit) const
{
  if (m_NextCapacitance == nullptr)
    return SEScalar::dNaN();
  return m_NextCapacitance->GetValue(unit);
}
bool SEThermalCircuitPath::HasCapacitanceBaseline() const
{
  return SECircuitPath::HasCapacitanceBaseline();
}
SEScalarHeatCapacitance& SEThermalCircuitPath::GetCapacitanceBaseline()
{
  return SECircuitPath::GetCapacitanceBaseline();
}
double SEThermalCircuitPath::GetCapacitanceBaseline(const HeatCapacitanceUnit& unit) const
{
  if (m_CapacitanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_CapacitanceBaseline->GetValue(unit);
}
void SEThermalCircuitPath::RemoveCapacitance()
{
  SECircuitPath::RemoveCapacitance();
}

/////////////////////////////////
// Thermal Inductance Types //
/////////////////////////////////

bool SEThermalCircuitPath::HasInductance() const
{
  return SECircuitPath::HasInductance();
}
SEScalarHeatInductance& SEThermalCircuitPath::GetInductance()
{
  return SECircuitPath::GetInductance();
}
double SEThermalCircuitPath::GetInductance(const HeatInductanceUnit& unit) const
{
  if (m_Inductance == nullptr)
    return SEScalar::dNaN();
  return m_Inductance->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextInductance() const
{
  return SECircuitPath::HasNextInductance();
}
SEScalarHeatInductance& SEThermalCircuitPath::GetNextInductance()
{
  return SECircuitPath::GetNextInductance();
}
double SEThermalCircuitPath::GetNextInductance(const HeatInductanceUnit& unit) const
{
  if (m_NextInductance == nullptr)
    return SEScalar::dNaN();
  return m_NextInductance->GetValue(unit);
}
bool SEThermalCircuitPath::HasInductanceBaseline() const
{
  return SECircuitPath::HasInductanceBaseline();
}
SEScalarHeatInductance& SEThermalCircuitPath::GetInductanceBaseline()
{
  return SECircuitPath::GetInductanceBaseline();
}
double SEThermalCircuitPath::GetInductanceBaseline(const HeatInductanceUnit& unit) const
{
  if (m_InductanceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_InductanceBaseline->GetValue(unit);
}
void SEThermalCircuitPath::RemoveInductance()
{
  SECircuitPath::RemoveInductance();
}


///////////////////////////
// Thermal Flux Types //
///////////////////////////
bool SEThermalCircuitPath::HasHeatTransferRate() const
{
  return HasFlux();
}
SEScalarPower& SEThermalCircuitPath::GetHeatTransferRate()
{
  return GetFlux();
}
double SEThermalCircuitPath::GetHeatTransferRate(const PowerUnit& unit) const
{
  if (m_Flux == nullptr)
    return SEScalar::dNaN();
  return m_Flux->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextHeatTransferRate() const
{
  return HasNextFlux();
}
SEScalarPower& SEThermalCircuitPath::GetNextHeatTransferRate()
{
  return GetNextFlux();
}
double SEThermalCircuitPath::GetNextHeatTransferRate(const PowerUnit& unit) const
{
  if (m_NextFlux == nullptr)
    return SEScalar::dNaN();
  return m_NextFlux->GetValue(unit);
}
bool SEThermalCircuitPath::HasHeatSource() const
{
  return HasFluxSource();
}
SEScalarPower& SEThermalCircuitPath::GetHeatSource()
{
  return GetFluxSource();
}
double SEThermalCircuitPath::GetHeatSource(const PowerUnit& unit) const
{
  if (m_FluxSource == nullptr)
    return SEScalar::dNaN();
  return m_FluxSource->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextHeatSource() const
{
  return HasNextFluxSource();
}
SEScalarPower& SEThermalCircuitPath::GetNextHeatSource()
{
  return GetNextFluxSource();
}
double SEThermalCircuitPath::GetNextHeatSource(const PowerUnit& unit) const
{
  if (m_NextFluxSource == nullptr)
    return SEScalar::dNaN();
  return m_NextFluxSource->GetValue(unit);
}
bool SEThermalCircuitPath::HasHeatSourceBaseline() const
{
  return HasFluxSourceBaseline();
}
SEScalarPower& SEThermalCircuitPath::GetHeatSourceBaseline()
{
  return GetFluxSourceBaseline();
}
double SEThermalCircuitPath::GetHeatSourceBaseline(const PowerUnit& unit) const
{
  if (m_FluxSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_FluxSourceBaseline->GetValue(unit);
}
void SEThermalCircuitPath::RemoveHeatSource()
{
  SECircuitPath::RemoveFluxSource();
}

////////////////////////////////
// Thermal Potential Types //
////////////////////////////////
bool SEThermalCircuitPath::HasTemperatureSource() const
{
  return HasPotentialSource();
}
SEScalarTemperature& SEThermalCircuitPath::GetTemperatureSource()
{
  return GetPotentialSource();
}
double SEThermalCircuitPath::GetTemperatureSource(const TemperatureUnit& unit) const
{
  if (m_PotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSource->GetValue(unit);
}
bool SEThermalCircuitPath::HasNextTemperatureSource() const
{
  return HasNextPotentialSource();
}
SEScalarTemperature& SEThermalCircuitPath::GetNextTemperatureSource()
{
  return GetNextPotentialSource();
}
double SEThermalCircuitPath::GetNextTemperatureSource(const TemperatureUnit& unit) const
{
  if (m_NextPotentialSource == nullptr)
    return SEScalar::dNaN();
  return m_NextPotentialSource->GetValue(unit);
}
bool SEThermalCircuitPath::HasTemperatureSourceBaseline() const
{
  return HasPotentialSourceBaseline();
}
SEScalarTemperature& SEThermalCircuitPath::GetTemperatureSourceBaseline()
{
  return GetPotentialSourceBaseline();
}
double SEThermalCircuitPath::GetTemperatureSourceBaseline(const TemperatureUnit& unit) const
{
  if (m_PotentialSourceBaseline == nullptr)
    return SEScalar::dNaN();
  return m_PotentialSourceBaseline->GetValue(unit);
}
void SEThermalCircuitPath::RemoveTemperatureSource()
{
  SECircuitPath::RemovePotentialSource();
}

bool SEThermalCircuitPath::HasValveBreakdownTemperature() const
{
  return HasValveBreakdownPotential();
}
SEScalarTemperature& SEThermalCircuitPath::GetValveBreakdownTemperature()
{
  return GetValveBreakdownPotential();
}
double SEThermalCircuitPath::GetValveBreakdownTemperature(const TemperatureUnit& unit) const
{
  if (m_ValveBreakdownPotential == nullptr)
    return SEScalar::dNaN();
  return m_ValveBreakdownPotential->GetValue(unit);
}
