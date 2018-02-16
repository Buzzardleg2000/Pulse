/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
CDM_BIND_DECL(RespiratorySystemData)


class CDM_DECL SERespiratorySystem : public SESystem
{
public:

  SERespiratorySystem(Logger* logger);
  virtual ~SERespiratorySystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);

  static void Load(const cdm::RespiratorySystemData& src, SERespiratorySystem& dst);
  static cdm::RespiratorySystemData* Unload(const SERespiratorySystem& src);
protected:
  static void Serialize(const cdm::RespiratorySystemData& src, SERespiratorySystem& dst);
  static void Serialize(const SERespiratorySystem& src, cdm::RespiratorySystemData& dst);

public:

  virtual bool HasAlveolarArterialGradient() const;
  virtual SEScalarPressure& GetAlveolarArterialGradient();
  virtual double GetAlveolarArterialGradient(const PressureUnit& unit) const;

  virtual bool HasCarricoIndex() const;
  virtual SEScalarPressure& GetCarricoIndex();
  virtual double GetCarricoIndex(const PressureUnit& unit) const;

  virtual bool HasEndTidalCarbonDioxideFraction() const;
  virtual SEScalar0To1& GetEndTidalCarbonDioxideFraction();
  virtual double GetEndTidalCarbonDioxideFraction() const;

  virtual bool HasEndTidalCarbonDioxidePressure() const;
  virtual SEScalarPressure& GetEndTidalCarbonDioxidePressure();
  virtual double GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const;

  virtual bool HasExpiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetExpiratoryFlow();
  virtual double GetExpiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetInspiratoryFlow();
  virtual double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPulmonaryCompliance() const;
  virtual SEScalarFlowCompliance& GetPulmonaryCompliance();
  virtual double GetPulmonaryCompliance(const FlowComplianceUnit& unit) const;

  virtual bool HasPulmonaryResistance() const;
  virtual SEScalarFlowResistance& GetPulmonaryResistance();
  virtual double GetPulmonaryResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRespirationDriverPressure() const;
  virtual SEScalarPressure& GetRespirationDriverPressure();
  virtual double GetRespirationDriverPressure(const PressureUnit& unit) const;

  virtual bool HasRespirationMusclePressure() const;
  virtual SEScalarPressure& GetRespirationMusclePressure();
  virtual double GetRespirationMusclePressure(const PressureUnit& unit) const;
  
  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasSpecificVentilation() const;
  virtual SEScalar& GetSpecificVentilation();
  virtual double GetSpecificVentilation() const;

  virtual bool HasTidalVolume() const;
  virtual SEScalarVolume& GetTidalVolume();
  virtual double GetTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalAlveolarVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalAlveolarVentilation();
  virtual double GetTotalAlveolarVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalDeadSpaceVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalDeadSpaceVentilation();
  virtual double GetTotalDeadSpaceVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalPulmonaryVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalPulmonaryVentilation();
  virtual double GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalLungVolume() const;
  virtual SEScalarVolume& GetTotalLungVolume();
  virtual double GetTotalLungVolume(const VolumeUnit& unit) const;

  virtual bool HasTranspulmonaryPressure() const;
  virtual SEScalarPressure& GetTranspulmonaryPressure();
  virtual double GetTranspulmonaryPressure(const PressureUnit& unit) const;


protected:

  SEScalarPressure*          m_AlveolarArterialGradient;
  SEScalarPressure*          m_CarricoIndex;
  SEScalar0To1*              m_EndTidalCarbonDioxideFraction;
  SEScalarPressure*          m_EndTidalCarbonDioxidePressure;
  SEScalarVolumePerTime*     m_ExpiratoryFlow;
  SEScalar*                  m_InspiratoryExpiratoryRatio;
  SEScalarVolumePerTime*     m_InspiratoryFlow;
  SEScalarFlowCompliance*    m_PulmonaryCompliance;
  SEScalarFlowResistance*    m_PulmonaryResistance;
  SEScalarPressure*          m_RespirationDriverPressure;
  SEScalarPressure*          m_RespirationMusclePressure;
  SEScalarFrequency*         m_RespirationRate;
  SEScalar*                  m_SpecificVentilation;
  SEScalarVolume*            m_TidalVolume;
  SEScalarVolumePerTime*     m_TotalAlveolarVentilation;
  SEScalarVolumePerTime*     m_TotalDeadSpaceVentilation;
  SEScalarVolume*            m_TotalLungVolume;
  SEScalarVolumePerTime*     m_TotalPulmonaryVentilation;
  SEScalarPressure*          m_TranspulmonaryPressure;
};