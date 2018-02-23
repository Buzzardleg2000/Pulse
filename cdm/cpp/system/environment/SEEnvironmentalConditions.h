/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SESubstance;
class SESubstanceManager;
class SESubstanceFraction;
class SESubstanceConcentration;
CDM_BIND_DECL(EnvironmentData_ConditionsData)
PROTO_PUSH
#include "bind/cdm/EnvironmentEnums.pb.h"
PROTO_POP

class CDM_DECL SEEnvironmentalConditions : public Loggable
{
protected:
  friend class SEEnvironment;
  friend class SEChangeEnvironmentConditions;
  friend class SEInitialEnvironmentConditions;
public:

  SEEnvironmentalConditions(SESubstanceManager& substances);
  virtual ~SEEnvironmentalConditions();

  virtual void Clear();

  static void Load(const cdm::EnvironmentData_ConditionsData& src, SEEnvironmentalConditions& dst);
  static cdm::EnvironmentData_ConditionsData* Unload(const SEEnvironmentalConditions& src);
protected:
  static void Serialize(const cdm::EnvironmentData_ConditionsData& src, SEEnvironmentalConditions& dst);
  static void Serialize(const SEEnvironmentalConditions& src, cdm::EnvironmentData_ConditionsData& dst);

   virtual void Merge(const SEEnvironmentalConditions& from);
public:

  virtual bool LoadFile(const std::string& environmentFile);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual cdm::eEnvironment_SurroundingType GetSurroundingType() const;
  virtual void SetSurroundingType(cdm::eEnvironment_SurroundingType name);

  virtual bool HasAirDensity() const;
  virtual SEScalarMassPerVolume& GetAirDensity();
  virtual double GetAirDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasAirVelocity() const;
  virtual SEScalarLengthPerTime& GetAirVelocity();
  virtual double GetAirVelocity(const LengthPerTimeUnit& unit) const;

  virtual bool HasAmbientTemperature() const;
  virtual SEScalarTemperature& GetAmbientTemperature();
  virtual double GetAmbientTemperature(const TemperatureUnit& unit) const;

  virtual bool HasAtmosphericPressure() const;
  virtual SEScalarPressure& GetAtmosphericPressure();
  virtual double GetAtmosphericPressure(const PressureUnit& unit) const;

  virtual bool HasClothingResistance() const;
  virtual SEScalarHeatResistanceArea& GetClothingResistance();
  virtual double GetClothingResistance(const HeatResistanceAreaUnit& unit) const;

  virtual bool HasEmissivity() const;
  virtual SEScalar0To1& GetEmissivity();
  virtual double GetEmissivity() const;

  virtual bool HasMeanRadiantTemperature() const;
  virtual SEScalarTemperature& GetMeanRadiantTemperature();
  virtual double GetMeanRadiantTemperature(const TemperatureUnit& unit) const;

  virtual bool HasRelativeHumidity() const;
  virtual SEScalar0To1& GetRelativeHumidity();
  virtual double GetRelativeHumidity() const;

  virtual bool HasRespirationAmbientTemperature() const;
  virtual SEScalarTemperature& GetRespirationAmbientTemperature();
  virtual double GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

  bool HasAmbientGas() const;
  bool HasAmbientGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetAmbientGases();
  const std::vector<const SESubstanceFraction*>& GetAmbientGases() const;
  SESubstanceFraction& GetAmbientGas(SESubstance& substance);
  const SESubstanceFraction* GetAmbientGas(const SESubstance& substance) const;
  void RemoveAmbientGas(const SESubstance& substance);
  void RemoveAmbientGases();

  bool HasAmbientAerosol() const;
  bool HasAmbientAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetAmbientAerosols();
  const std::vector<const SESubstanceConcentration*>& GetAmbientAerosols() const;
  SESubstanceConcentration& GetAmbientAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetAmbientAerosol(const SESubstance& substance) const;
  void RemoveAmbientAerosol(const SESubstance& substance);
  void RemoveAmbientAerosols();

protected:

  cdm::eEnvironment_SurroundingType            m_SurroundingType;
  
  SEScalarMassPerVolume*                       m_AirDensity;
  SEScalarLengthPerTime*                       m_AirVelocity;
  SEScalarTemperature*                         m_AmbientTemperature;
  SEScalarPressure*                            m_AtmosphericPressure;
  SEScalarHeatResistanceArea*                  m_ClothingResistance;
  SEScalar0To1*                                m_Emissivity;
  SEScalarTemperature*                         m_MeanRadiantTemperature;
  SEScalar0To1*                                m_RelativeHumidity;
  SEScalarTemperature*                         m_RespirationAmbientTemperature;

  std::vector<SESubstanceFraction*>            m_AmbientGases;
  std::vector<const SESubstanceFraction*>      m_cAmbientGases;

  std::vector<SESubstanceConcentration*>       m_AmbientAerosols;
  std::vector<const SESubstanceConcentration*> m_cAmbientAerosols;

  SESubstanceManager&               m_Substances;
};                  
