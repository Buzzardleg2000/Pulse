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

SEMechanicalVentilator::SEMechanicalVentilator(Logger* logger) : SEEquipment(logger)
{
  m_AirwayPressure = nullptr;
  m_EndTidalCarbonDioxideFraction = nullptr;
  m_EndTidalCarbonDioxidePressure = nullptr;
  m_EndTidalOxygenFraction = nullptr;
  m_EndTidalOxygenPressure = nullptr;
  m_ExpiratoryFlow = nullptr;
  m_ExpiratoryTidalVolume = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_InspiratoryFlow = nullptr;
  m_InspiratoryTidalVolume = nullptr;
  m_IntrinsicPositiveEndExpiredPressure = nullptr;
  m_LeakFractionOrSeverity = nullptr;
  m_MeanAirwayPressure = nullptr;
  m_PeakInspiratoryPressure = nullptr;
  m_PlateauPressure = nullptr;
  m_PositiveEndExpiratoryPressure = nullptr;
  m_PulmonaryCompliance = nullptr;
  m_PulmonaryElastance = nullptr;
  m_RelativeTotalLungVolume = nullptr;
  m_RespirationRate = nullptr;
  m_TidalVolume = nullptr;
  m_TotalLungVolume = nullptr;
  m_TotalPulmonaryVentilation = nullptr;

  m_Settings = nullptr;
}

SEMechanicalVentilator::~SEMechanicalVentilator()
{
  SAFE_DELETE(m_AirwayPressure);
  SAFE_DELETE(m_EndTidalCarbonDioxideFraction);
  SAFE_DELETE(m_EndTidalCarbonDioxidePressure);
  SAFE_DELETE(m_EndTidalOxygenFraction);
  SAFE_DELETE(m_EndTidalOxygenPressure);
  SAFE_DELETE(m_ExpiratoryFlow);
  SAFE_DELETE(m_ExpiratoryTidalVolume);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_InspiratoryFlow);
  SAFE_DELETE(m_InspiratoryTidalVolume);
  SAFE_DELETE(m_IntrinsicPositiveEndExpiredPressure);
  SAFE_DELETE(m_LeakFractionOrSeverity);
  SAFE_DELETE(m_MeanAirwayPressure);
  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_PlateauPressure);
  SAFE_DELETE(m_PositiveEndExpiratoryPressure);
  SAFE_DELETE(m_PulmonaryCompliance);
  SAFE_DELETE(m_PulmonaryElastance);
  SAFE_DELETE(m_RelativeTotalLungVolume);
  SAFE_DELETE(m_RespirationRate);
  SAFE_DELETE(m_TidalVolume);
  SAFE_DELETE(m_TotalLungVolume);
  SAFE_DELETE(m_TotalPulmonaryVentilation);

  SAFE_DELETE(m_Settings);
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();

  INVALIDATE_PROPERTY(m_AirwayPressure);
  INVALIDATE_PROPERTY(m_EndTidalCarbonDioxideFraction);
  INVALIDATE_PROPERTY(m_EndTidalCarbonDioxidePressure);
  INVALIDATE_PROPERTY(m_EndTidalOxygenFraction);
  INVALIDATE_PROPERTY(m_EndTidalOxygenPressure);
  INVALIDATE_PROPERTY(m_ExpiratoryFlow);
  INVALIDATE_PROPERTY(m_ExpiratoryTidalVolume);
  INVALIDATE_PROPERTY(m_InspiratoryExpiratoryRatio);
  INVALIDATE_PROPERTY(m_InspiratoryFlow);
  INVALIDATE_PROPERTY(m_InspiratoryTidalVolume);
  INVALIDATE_PROPERTY(m_IntrinsicPositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_LeakFractionOrSeverity);
  INVALIDATE_PROPERTY(m_MeanAirwayPressure);
  INVALIDATE_PROPERTY(m_PeakInspiratoryPressure);
  INVALIDATE_PROPERTY(m_PlateauPressure);
  INVALIDATE_PROPERTY(m_PositiveEndExpiratoryPressure);
  INVALIDATE_PROPERTY(m_PulmonaryCompliance);
  INVALIDATE_PROPERTY(m_PulmonaryElastance);
  INVALIDATE_PROPERTY(m_RelativeTotalLungVolume);
  INVALIDATE_PROPERTY(m_RespirationRate);
  INVALIDATE_PROPERTY(m_TidalVolume);
  INVALIDATE_PROPERTY(m_TotalLungVolume);
  INVALIDATE_PROPERTY(m_TotalPulmonaryVentilation);

  if (m_Settings)
    m_Settings->Clear();
}

const SEScalar* SEMechanicalVentilator::GetScalar(const std::string& name)
{
  if (name.compare("AirwayPressure") == 0)
    return &GetAirwayPressure();
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
  if (name.compare("ExpiratoryTidalVolume") == 0)
    return &GetExpiratoryTidalVolume();
  if (name.compare("InspiratoryExpiratoryRatio") == 0)
    return &GetInspiratoryExpiratoryRatio();
  if (name.compare("InspiratoryFlow") == 0)
    return &GetInspiratoryFlow();
  if (name.compare("InspiratoryTidalVolume") == 0)
    return &GetInspiratoryTidalVolume();
  if (name.compare("IntrinsicPositiveEndExpiredPressure") == 0)
    return &GetIntrinsicPositiveEndExpiredPressure();
  if (name.compare("LeakFractionOrSeverity") == 0)
    return &GetLeakFractionOrSeverity();
  if (name.compare("MeanAirwayPressure") == 0)
    return &GetMeanAirwayPressure();
  if (name.compare("PeakInspiratoryPressure") == 0)
    return &GetPeakInspiratoryPressure();
  if (name.compare("PlateauPressure") == 0)
    return &GetPlateauPressure();
  if (name.compare("PositiveEndExpiratoryPressure") == 0)
    return &GetPositiveEndExpiratoryPressure();
  if (name.compare("PulmonaryCompliance") == 0)
    return &GetPulmonaryCompliance();
  if (name.compare("PulmonaryElastance") == 0)
    return &GetPulmonaryElastance();
  if (name.compare("RelativeTotalLungVolume") == 0)
    return &GetRelativeTotalLungVolume();
  if (name.compare("RespirationRate") == 0)
    return &GetRespirationRate();
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

bool SEMechanicalVentilator::HasLeakFractionOrSeverity() const
{
  return m_LeakFractionOrSeverity == nullptr ? false : m_LeakFractionOrSeverity->IsValid();
}
SEScalar0To1& SEMechanicalVentilator::GetLeakFractionOrSeverity()
{
  if (m_LeakFractionOrSeverity == nullptr)
    m_LeakFractionOrSeverity = new SEScalar0To1();
  return *m_LeakFractionOrSeverity;
}
double SEMechanicalVentilator::GetLeakFractionOrSeverity() const
{
  if (m_LeakFractionOrSeverity == nullptr)
    return SEScalar::dNaN();
  return m_LeakFractionOrSeverity->GetValue();
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

bool SEMechanicalVentilator::HasPulmonaryCompliance() const
{
  return m_PulmonaryCompliance == nullptr ? false : m_PulmonaryCompliance->IsValid();
}
SEScalarVolumePerPressure& SEMechanicalVentilator::GetPulmonaryCompliance()
{
  if (m_PulmonaryCompliance == nullptr)
    m_PulmonaryCompliance = new SEScalarVolumePerPressure();
  return *m_PulmonaryCompliance;
}
double SEMechanicalVentilator::GetPulmonaryCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_PulmonaryCompliance == nullptr)
    return SEScalar::dNaN();
  return m_PulmonaryCompliance->GetValue(unit);
}

bool SEMechanicalVentilator::HasPulmonaryElastance() const
{
  return m_PulmonaryElastance == nullptr ? false : m_PulmonaryElastance->IsValid();
}
SEScalarPressurePerVolume& SEMechanicalVentilator::GetPulmonaryElastance()
{
  if (m_PulmonaryElastance == nullptr)
    m_PulmonaryElastance = new SEScalarPressurePerVolume();
  return *m_PulmonaryElastance;
}
double SEMechanicalVentilator::GetPulmonaryElastance(const PressurePerVolumeUnit& unit) const
{
  if (m_PulmonaryElastance == nullptr)
    return SEScalar::dNaN();
  return m_PulmonaryElastance->GetValue(unit);
}

bool SEMechanicalVentilator::HasRelativeTotalLungVolume() const
{
  return m_RelativeTotalLungVolume == nullptr ? false : m_RelativeTotalLungVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetRelativeTotalLungVolume()
{
  if (m_RelativeTotalLungVolume == nullptr)
    m_RelativeTotalLungVolume = new SEScalarVolume();
  return *m_RelativeTotalLungVolume;
}
double SEMechanicalVentilator::GetRelativeTotalLungVolume(const VolumeUnit& unit) const
{
  if (m_RelativeTotalLungVolume == nullptr)
    return SEScalar::dNaN();
  return m_RelativeTotalLungVolume->GetValue(unit);
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
