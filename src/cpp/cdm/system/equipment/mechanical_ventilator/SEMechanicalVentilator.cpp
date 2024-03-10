/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"

#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"

SEMechanicalVentilator::SEMechanicalVentilator(Logger* logger) : SEEquipment(logger)
{
  m_AirwayPressure = nullptr;
  m_BreathState = eBreathState::NoBreath;
  m_DynamicPulmonaryCompliance = nullptr;
  m_EndTidalCarbonDioxideFraction = nullptr;
  m_EndTidalCarbonDioxidePressure = nullptr;
  m_EndTidalOxygenFraction = nullptr;
  m_EndTidalOxygenPressure = nullptr;
  m_ExpiratoryFlow = nullptr;
  m_ExpiratoryResistance = nullptr;
  m_ExpiratoryTidalVolume = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_InspiratoryFlow = nullptr;
  m_InspiratoryResistance = nullptr;
  m_InspiratoryTidalVolume = nullptr;
  m_IntrinsicPositiveEndExpiredPressure = nullptr;
  m_LeakFraction = nullptr;
  m_MeanAirwayPressure = nullptr;
  m_PeakExpiratoryFlow = nullptr;
  m_PeakInspiratoryFlow = nullptr;
  m_PeakInspiratoryPressure = nullptr;
  m_PlateauPressure = nullptr;
  m_PositiveEndExpiratoryPressure = nullptr;
  m_RespirationRate = nullptr;
  m_StaticPulmonaryCompliance = nullptr;
  m_TidalVolume = nullptr;
  m_TotalLungVolume = nullptr;
  m_TotalPulmonaryVentilation = nullptr;

  m_Settings = nullptr;
}

SEMechanicalVentilator::~SEMechanicalVentilator()
{
  SAFE_DELETE(m_AirwayPressure);
  m_BreathState = eBreathState::NoBreath;
  SAFE_DELETE(m_DynamicPulmonaryCompliance);
  SAFE_DELETE(m_EndTidalCarbonDioxideFraction);
  SAFE_DELETE(m_EndTidalCarbonDioxidePressure);
  SAFE_DELETE(m_EndTidalOxygenFraction);
  SAFE_DELETE(m_EndTidalOxygenPressure);
  SAFE_DELETE(m_ExpiratoryFlow);
  SAFE_DELETE(m_ExpiratoryResistance);
  SAFE_DELETE(m_ExpiratoryTidalVolume);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_InspiratoryFlow);
  SAFE_DELETE(m_InspiratoryResistance);
  SAFE_DELETE(m_InspiratoryTidalVolume);
  SAFE_DELETE(m_IntrinsicPositiveEndExpiredPressure);
  SAFE_DELETE(m_LeakFraction);
  SAFE_DELETE(m_MeanAirwayPressure);
  SAFE_DELETE(m_PeakExpiratoryFlow);
  SAFE_DELETE(m_PeakInspiratoryFlow);
  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_PlateauPressure);
  SAFE_DELETE(m_PositiveEndExpiratoryPressure);
  SAFE_DELETE(m_RespirationRate);
  SAFE_DELETE(m_StaticPulmonaryCompliance);
  SAFE_DELETE(m_TidalVolume);
  SAFE_DELETE(m_TotalLungVolume);
  SAFE_DELETE(m_TotalPulmonaryVentilation);

  SAFE_DELETE(m_Settings);
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();

  INVALIDATE_PROPERTY(m_AirwayPressure);
  m_BreathState = eBreathState::NoBreath;
  INVALIDATE_PROPERTY(m_DynamicPulmonaryCompliance);
  INVALIDATE_PROPERTY(m_EndTidalCarbonDioxideFraction);
  INVALIDATE_PROPERTY(m_EndTidalCarbonDioxidePressure);
  INVALIDATE_PROPERTY(m_EndTidalOxygenFraction);
  INVALIDATE_PROPERTY(m_EndTidalOxygenPressure);
  INVALIDATE_PROPERTY(m_ExpiratoryFlow);
  INVALIDATE_PROPERTY(m_ExpiratoryResistance);
  INVALIDATE_PROPERTY(m_ExpiratoryTidalVolume);
  INVALIDATE_PROPERTY(m_InspiratoryExpiratoryRatio);
  INVALIDATE_PROPERTY(m_InspiratoryFlow);
  INVALIDATE_PROPERTY(m_InspiratoryResistance);
  INVALIDATE_PROPERTY(m_InspiratoryTidalVolume);
  INVALIDATE_PROPERTY(m_IntrinsicPositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_LeakFraction);
  INVALIDATE_PROPERTY(m_MeanAirwayPressure);
  INVALIDATE_PROPERTY(m_PeakExpiratoryFlow);
  INVALIDATE_PROPERTY(m_PeakInspiratoryFlow);
  INVALIDATE_PROPERTY(m_PeakInspiratoryPressure);
  INVALIDATE_PROPERTY(m_PlateauPressure);
  INVALIDATE_PROPERTY(m_PositiveEndExpiratoryPressure);
  INVALIDATE_PROPERTY(m_RespirationRate);
  INVALIDATE_PROPERTY(m_StaticPulmonaryCompliance);
  INVALIDATE_PROPERTY(m_TidalVolume);
  INVALIDATE_PROPERTY(m_TotalLungVolume);
  INVALIDATE_PROPERTY(m_TotalPulmonaryVentilation);

  if (m_Settings)
    m_Settings->Clear();
}

void SEMechanicalVentilator::TurnOff()
{
  // Set anything that has a value, to 0
  ZERO_UNIT_SCALAR(m_AirwayPressure);
  m_BreathState = eBreathState::NoBreath;
  ZERO_UNIT_SCALAR(m_DynamicPulmonaryCompliance);
  ZERO_SCALAR(m_EndTidalCarbonDioxideFraction);
  ZERO_UNIT_SCALAR(m_EndTidalCarbonDioxidePressure);
  ZERO_SCALAR(m_EndTidalOxygenFraction);
  ZERO_UNIT_SCALAR(m_EndTidalOxygenPressure);
  ZERO_UNIT_SCALAR(m_ExpiratoryFlow);
  ZERO_UNIT_SCALAR(m_ExpiratoryResistance);
  ZERO_UNIT_SCALAR(m_ExpiratoryTidalVolume);
  ZERO_SCALAR(m_InspiratoryExpiratoryRatio);
  ZERO_UNIT_SCALAR(m_InspiratoryFlow);
  ZERO_UNIT_SCALAR(m_InspiratoryResistance);
  ZERO_UNIT_SCALAR(m_InspiratoryTidalVolume);
  ZERO_UNIT_SCALAR(m_IntrinsicPositiveEndExpiredPressure);
  ZERO_SCALAR(m_LeakFraction);
  ZERO_UNIT_SCALAR(m_MeanAirwayPressure);
  ZERO_UNIT_SCALAR(m_PeakExpiratoryFlow);
  ZERO_UNIT_SCALAR(m_PeakInspiratoryFlow);
  ZERO_UNIT_SCALAR(m_PeakInspiratoryPressure);
  ZERO_UNIT_SCALAR(m_PlateauPressure);
  ZERO_UNIT_SCALAR(m_PositiveEndExpiratoryPressure);
  ZERO_UNIT_SCALAR(m_RespirationRate);
  ZERO_UNIT_SCALAR(m_StaticPulmonaryCompliance);
  ZERO_UNIT_SCALAR(m_TidalVolume);
  ZERO_UNIT_SCALAR(m_TotalLungVolume);
  ZERO_UNIT_SCALAR(m_TotalPulmonaryVentilation);

  if (m_Settings)
    m_Settings->Clear();

  GetSettings().SetConnection(eSwitch::Off);
  GetSettings().SetExpirationCycleRespiratoryModel(eSwitch::Off);
  GetSettings().SetInspirationPatientTriggerRespiratoryModel(eSwitch::Off);
}

const SEScalar* SEMechanicalVentilator::GetScalar(const std::string& name)
{
  if (name.compare("AirwayPressure") == 0)
    return &GetAirwayPressure();
  if (name.compare("BreathState") == 0)
    return &m_BreathState;
  if (name.compare("DynamicPulmonaryCompliance") == 0)
    return &GetDynamicPulmonaryCompliance();
  if (name.compare("EndTidalCarbonDioxideFraction") == 0)
    return &GetEndTidalCarbonDioxideFraction();
  if (name.compare("EndTidalCarbonDioxidePressure") == 0)
    return &GetEndTidalCarbonDioxidePressure();
  if (name.compare("EndTidalOxygenFraction") == 0)
    return &GetEndTidalOxygenFraction();
  if (name.compare("EndTidalOxygenPressure") == 0)
    return &GetEndTidalOxygenPressure();
  if (name.compare("ExpiratoryFlow") == 0)
    return &GetExpiratoryFlow();
  if (name.compare("ExpiratoryResistance") == 0)
    return &GetExpiratoryResistance();
  if (name.compare("ExpiratoryTidalVolume") == 0)
    return &GetExpiratoryTidalVolume();
  if (name.compare("InspiratoryExpiratoryRatio") == 0)
    return &GetInspiratoryExpiratoryRatio();
  if (name.compare("InspiratoryFlow") == 0)
    return &GetInspiratoryFlow();
  if (name.compare("InspiratoryResistance") == 0)
    return &GetInspiratoryResistance();
  if (name.compare("InspiratoryTidalVolume") == 0)
    return &GetInspiratoryTidalVolume();
  if (name.compare("IntrinsicPositiveEndExpiredPressure") == 0)
    return &GetIntrinsicPositiveEndExpiredPressure();
  if (name.compare("LeakFraction") == 0)
    return &GetLeakFraction();
  if (name.compare("MeanAirwayPressure") == 0)
    return &GetMeanAirwayPressure();
  if (name.compare("PeakExpiratoryFlow") == 0)
    return &GetPeakExpiratoryFlow();
  if (name.compare("PeakInspiratoryFlow") == 0)
    return &GetPeakInspiratoryFlow();
  if (name.compare("PeakInspiratoryPressure") == 0)
    return &GetPeakInspiratoryPressure();
  if (name.compare("PlateauPressure") == 0)
    return &GetPlateauPressure();
  if (name.compare("PositiveEndExpiratoryPressure") == 0)
    return &GetPositiveEndExpiratoryPressure();
  if (name.compare("RespirationRate") == 0)
    return &GetRespirationRate();
  if (name.compare("StaticPulmonaryCompliance") == 0)
    return &GetStaticPulmonaryCompliance();
  if (name.compare("TidalVolume") == 0)
    return &GetTidalVolume();
  if (name.compare("TotalLungVolume") == 0)
    return &GetTotalLungVolume();
  if (name.compare("TotalPulmonaryVentilation") == 0)
    return &GetTotalPulmonaryVentilation();

  return GetSettings().GetScalar(name);
}

bool SEMechanicalVentilator::HasAirwayPressure() const
{
  return m_AirwayPressure == nullptr ? false : m_AirwayPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetAirwayPressure()
{
  if (m_AirwayPressure == nullptr)
    m_AirwayPressure = new SEScalarPressure();
  return *m_AirwayPressure;
}
double SEMechanicalVentilator::GetAirwayPressure(const PressureUnit& unit) const
{
  if (m_AirwayPressure == nullptr)
    return SEScalar::dNaN();
  return m_AirwayPressure->GetValue(unit);
}

void SEMechanicalVentilator::SetBreathState(eBreathState b)
{
  m_BreathState = b;
}
eBreathState SEMechanicalVentilator::GetBreathState() const
{
  return m_BreathState.GetEnum();
}

bool SEMechanicalVentilator::HasDynamicPulmonaryCompliance() const
{
  return m_DynamicPulmonaryCompliance == nullptr ? false : m_DynamicPulmonaryCompliance->IsValid();
}
SEScalarVolumePerPressure& SEMechanicalVentilator::GetDynamicPulmonaryCompliance()
{
  if (m_DynamicPulmonaryCompliance == nullptr)
    m_DynamicPulmonaryCompliance = new SEScalarVolumePerPressure();
  return *m_DynamicPulmonaryCompliance;
}
double SEMechanicalVentilator::GetDynamicPulmonaryCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_DynamicPulmonaryCompliance == nullptr)
    return SEScalar::dNaN();
  return m_DynamicPulmonaryCompliance->GetValue(unit);
}

bool SEMechanicalVentilator::HasEndTidalCarbonDioxideFraction() const
{
  return m_EndTidalCarbonDioxideFraction == nullptr ? false : m_EndTidalCarbonDioxideFraction->IsValid();
}
SEScalar0To1& SEMechanicalVentilator::GetEndTidalCarbonDioxideFraction()
{
  if (m_EndTidalCarbonDioxideFraction == nullptr)
    m_EndTidalCarbonDioxideFraction = new SEScalar0To1();
  return *m_EndTidalCarbonDioxideFraction;
}
double SEMechanicalVentilator::GetEndTidalCarbonDioxideFraction() const
{
  if (m_EndTidalCarbonDioxideFraction == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalCarbonDioxideFraction->GetValue();
}

bool SEMechanicalVentilator::HasEndTidalCarbonDioxidePressure() const
{
  return m_EndTidalCarbonDioxidePressure == nullptr ? false : m_EndTidalCarbonDioxidePressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetEndTidalCarbonDioxidePressure()
{
  if (m_EndTidalCarbonDioxidePressure == nullptr)
    m_EndTidalCarbonDioxidePressure = new SEScalarPressure();
  return *m_EndTidalCarbonDioxidePressure;
}
double SEMechanicalVentilator::GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_EndTidalCarbonDioxidePressure == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalCarbonDioxidePressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasEndTidalOxygenFraction() const
{
  return m_EndTidalOxygenFraction == nullptr ? false : m_EndTidalOxygenFraction->IsValid();
}
SEScalar0To1& SEMechanicalVentilator::GetEndTidalOxygenFraction()
{
  if (m_EndTidalOxygenFraction == nullptr)
    m_EndTidalOxygenFraction = new SEScalar0To1();
  return *m_EndTidalOxygenFraction;
}
double SEMechanicalVentilator::GetEndTidalOxygenFraction() const
{
  if (m_EndTidalOxygenFraction == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalOxygenFraction->GetValue();
}

bool SEMechanicalVentilator::HasEndTidalOxygenPressure() const
{
  return m_EndTidalOxygenPressure == nullptr ? false : m_EndTidalOxygenPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetEndTidalOxygenPressure()
{
  if (m_EndTidalOxygenPressure == nullptr)
    m_EndTidalOxygenPressure = new SEScalarPressure();
  return *m_EndTidalOxygenPressure;
}
double SEMechanicalVentilator::GetEndTidalOxygenPressure(const PressureUnit& unit) const
{
  if (m_EndTidalOxygenPressure == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalOxygenPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpiratoryFlow() const
{
  return m_ExpiratoryFlow == nullptr ? false : m_ExpiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetExpiratoryFlow()
{
  if (m_ExpiratoryFlow == nullptr)
    m_ExpiratoryFlow = new SEScalarVolumePerTime();
  return *m_ExpiratoryFlow;
}
double SEMechanicalVentilator::GetExpiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (m_ExpiratoryFlow == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpiratoryResistance() const
{
  return m_ExpiratoryResistance == nullptr ? false : m_ExpiratoryResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetExpiratoryResistance()
{
  if (m_ExpiratoryResistance == nullptr)
    m_ExpiratoryResistance = new SEScalarPressureTimePerVolume();
  return *m_ExpiratoryResistance;
}
double SEMechanicalVentilator::GetExpiratoryResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ExpiratoryResistance == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryResistance->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpiratoryTidalVolume() const
{
  return m_ExpiratoryTidalVolume == nullptr ? false : m_ExpiratoryTidalVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetExpiratoryTidalVolume()
{
  if (m_ExpiratoryTidalVolume == nullptr)
    m_ExpiratoryTidalVolume = new SEScalarVolume();
  return *m_ExpiratoryTidalVolume;
}
double SEMechanicalVentilator::GetExpiratoryTidalVolume(const VolumeUnit& unit) const
{
  if (m_ExpiratoryTidalVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryTidalVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
SEScalar& SEMechanicalVentilator::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
double SEMechanicalVentilator::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}

bool SEMechanicalVentilator::HasInspiratoryFlow() const
{
  return m_InspiratoryFlow == nullptr ? false : m_InspiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetInspiratoryFlow()
{
  if (m_InspiratoryFlow == nullptr)
    m_InspiratoryFlow = new SEScalarVolumePerTime();
  return *m_InspiratoryFlow;
}
double SEMechanicalVentilator::GetInspiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspiratoryFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspiratoryResistance() const
{
  return m_InspiratoryResistance == nullptr ? false : m_InspiratoryResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetInspiratoryResistance()
{
  if (m_InspiratoryResistance == nullptr)
    m_InspiratoryResistance = new SEScalarPressureTimePerVolume();
  return *m_InspiratoryResistance;
}
double SEMechanicalVentilator::GetInspiratoryResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_InspiratoryResistance == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryResistance->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspiratoryTidalVolume() const
{
  return m_InspiratoryTidalVolume == nullptr ? false : m_InspiratoryTidalVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetInspiratoryTidalVolume()
{
  if (m_InspiratoryTidalVolume == nullptr)
    m_InspiratoryTidalVolume = new SEScalarVolume();
  return *m_InspiratoryTidalVolume;
}
double SEMechanicalVentilator::GetInspiratoryTidalVolume(const VolumeUnit& unit) const
{
  if (m_InspiratoryTidalVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryTidalVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasIntrinsicPositiveEndExpiredPressure() const
{
  return m_IntrinsicPositiveEndExpiredPressure == nullptr ? false : m_IntrinsicPositiveEndExpiredPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetIntrinsicPositiveEndExpiredPressure()
{
  if (m_IntrinsicPositiveEndExpiredPressure == nullptr)
    m_IntrinsicPositiveEndExpiredPressure = new SEScalarPressure();
  return *m_IntrinsicPositiveEndExpiredPressure;
}
double SEMechanicalVentilator::GetIntrinsicPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_IntrinsicPositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_IntrinsicPositiveEndExpiredPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasLeakFraction() const
{
  return m_LeakFraction == nullptr ? false : m_LeakFraction->IsValid();
}
SEScalar0To1& SEMechanicalVentilator::GetLeakFraction()
{
  if (m_LeakFraction == nullptr)
    m_LeakFraction = new SEScalar0To1();
  return *m_LeakFraction;
}
double SEMechanicalVentilator::GetLeakFraction() const
{
  if (m_LeakFraction == nullptr)
    return SEScalar::dNaN();
  return m_LeakFraction->GetValue();
}

bool SEMechanicalVentilator::HasMeanAirwayPressure() const
{
  return m_MeanAirwayPressure == nullptr ? false : m_MeanAirwayPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetMeanAirwayPressure()
{
  if (m_MeanAirwayPressure == nullptr)
    m_MeanAirwayPressure = new SEScalarPressure();
  return *m_MeanAirwayPressure;
}
double SEMechanicalVentilator::GetMeanAirwayPressure(const PressureUnit& unit) const
{
  if (m_MeanAirwayPressure == nullptr)
    return SEScalar::dNaN();
  return m_MeanAirwayPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasPeakExpiratoryFlow() const
{
  return m_PeakExpiratoryFlow == nullptr ? false : m_PeakExpiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetPeakExpiratoryFlow()
{
  if (m_PeakExpiratoryFlow == nullptr)
    m_PeakExpiratoryFlow = new SEScalarVolumePerTime();
  return *m_PeakExpiratoryFlow;
}
double SEMechanicalVentilator::GetPeakExpiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (m_PeakExpiratoryFlow == nullptr)
    return SEScalar::dNaN();
  return m_PeakExpiratoryFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasPeakInspiratoryFlow() const
{
  return m_PeakInspiratoryFlow == nullptr ? false : m_PeakInspiratoryFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetPeakInspiratoryFlow()
{
  if (m_PeakInspiratoryFlow == nullptr)
    m_PeakInspiratoryFlow = new SEScalarVolumePerTime();
  return *m_PeakInspiratoryFlow;
}
double SEMechanicalVentilator::GetPeakInspiratoryFlow(const VolumePerTimeUnit& unit) const
{
  if (m_PeakInspiratoryFlow == nullptr)
    return SEScalar::dNaN();
  return m_PeakInspiratoryFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasPeakInspiratoryPressure() const
{
  return m_PeakInspiratoryPressure == nullptr ? false : m_PeakInspiratoryPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetPeakInspiratoryPressure()
{
  if (m_PeakInspiratoryPressure == nullptr)
    m_PeakInspiratoryPressure = new SEScalarPressure();
  return *m_PeakInspiratoryPressure;
}
double SEMechanicalVentilator::GetPeakInspiratoryPressure(const PressureUnit& unit) const
{
  if (m_PeakInspiratoryPressure == nullptr)
    return SEScalar::dNaN();
  return m_PeakInspiratoryPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasPlateauPressure() const
{
  return m_PlateauPressure == nullptr ? false : m_PlateauPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetPlateauPressure()
{
  if (m_PlateauPressure == nullptr)
    m_PlateauPressure = new SEScalarPressure();
  return *m_PlateauPressure;
}
double SEMechanicalVentilator::GetPlateauPressure(const PressureUnit& unit) const
{
  if (m_PlateauPressure == nullptr)
    return SEScalar::dNaN();
  return m_PlateauPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasPositiveEndExpiratoryPressure() const
{
  return m_PositiveEndExpiratoryPressure == nullptr ? false : m_PositiveEndExpiratoryPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetPositiveEndExpiratoryPressure()
{
  if (m_PositiveEndExpiratoryPressure == nullptr)
    m_PositiveEndExpiratoryPressure = new SEScalarPressure();
  return *m_PositiveEndExpiratoryPressure;
}
double SEMechanicalVentilator::GetPositiveEndExpiratoryPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiratoryPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiratoryPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasRespirationRate() const
{
  return m_RespirationRate == nullptr ? false : m_RespirationRate->IsValid();
}
SEScalarFrequency& SEMechanicalVentilator::GetRespirationRate()
{
  if (m_RespirationRate == nullptr)
    m_RespirationRate = new SEScalarFrequency();
  return *m_RespirationRate;
}
double SEMechanicalVentilator::GetRespirationRate(const FrequencyUnit& unit) const
{
  if (m_RespirationRate == nullptr)
    return SEScalar::dNaN();
  return m_RespirationRate->GetValue(unit);
}

bool SEMechanicalVentilator::HasStaticPulmonaryCompliance() const
{
  return m_StaticPulmonaryCompliance == nullptr ? false : m_StaticPulmonaryCompliance->IsValid();
}
SEScalarVolumePerPressure& SEMechanicalVentilator::GetStaticPulmonaryCompliance()
{
  if (m_StaticPulmonaryCompliance == nullptr)
    m_StaticPulmonaryCompliance = new SEScalarVolumePerPressure();
  return *m_StaticPulmonaryCompliance;
}
double SEMechanicalVentilator::GetStaticPulmonaryCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_StaticPulmonaryCompliance == nullptr)
    return SEScalar::dNaN();
  return m_StaticPulmonaryCompliance->GetValue(unit);
}

bool SEMechanicalVentilator::HasTidalVolume() const
{
  return m_TidalVolume == nullptr ? false : m_TidalVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetTidalVolume()
{
  if (m_TidalVolume == nullptr)
    m_TidalVolume = new SEScalarVolume();
  return *m_TidalVolume;
}
double SEMechanicalVentilator::GetTidalVolume(const VolumeUnit& unit) const
{
  if (m_TidalVolume == nullptr)
    return SEScalar::dNaN();
  return m_TidalVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasTotalLungVolume() const
{
  return m_TotalLungVolume == nullptr ? false : m_TotalLungVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetTotalLungVolume()
{
  if (m_TotalLungVolume == nullptr)
    m_TotalLungVolume = new SEScalarVolume();
  return *m_TotalLungVolume;
}
double SEMechanicalVentilator::GetTotalLungVolume(const VolumeUnit& unit) const
{
  if (m_TotalLungVolume == nullptr)
    return SEScalar::dNaN();
  return m_TotalLungVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasTotalPulmonaryVentilation() const
{
  return m_TotalPulmonaryVentilation == nullptr ? false : m_TotalPulmonaryVentilation->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetTotalPulmonaryVentilation()
{
  if (m_TotalPulmonaryVentilation == nullptr)
    m_TotalPulmonaryVentilation = new SEScalarVolumePerTime();
  return *m_TotalPulmonaryVentilation;
}
double SEMechanicalVentilator::GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const
{
  if (m_TotalPulmonaryVentilation == nullptr)
    return SEScalar::dNaN();
  return m_TotalPulmonaryVentilation->GetValue(unit);
}

bool SEMechanicalVentilator::HasSettings() const
{
  return m_Settings != nullptr;
}
SEMechanicalVentilatorSettings& SEMechanicalVentilator::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEMechanicalVentilatorSettings(GetLogger());
  return *m_Settings;
}
const SEMechanicalVentilatorSettings* SEMechanicalVentilator::GetSettings() const
{
  return m_Settings;
}
void SEMechanicalVentilator::RemoveSettings()
{
  SAFE_DELETE(m_Settings);
}
