/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include "system/SESystem.h"
#include "bind/cdm/EnvironmentConditions.pb.h"
class SESubstance;
class SESubstanceFraction;
class SESubstanceConcentration;
class SEEnvironment;
class SESubstanceManager;
class SEEnvironmentChange;
class SEInitialEnvironment;

class DLL_DECL SEEnvironmentalConditions : public Loggable
{
protected:
  friend SEEnvironment;
  friend SEEnvironmentChange;
  friend SEInitialEnvironment;
public:

  SEEnvironmentalConditions(SESubstanceManager& substances);
  virtual ~SEEnvironmentalConditions();

  virtual void Clear();

  static void Load(const cdm::EnvironmentConditionData& src, SEEnvironmentalConditions& dst);
  static cdm::EnvironmentConditionData* Unload(const SEEnvironmentalConditions& src);
protected:
  static void Serialize(const cdm::EnvironmentConditionData& src, SEEnvironmentalConditions& dst);
  static void Serialize(const SEEnvironmentalConditions& src, cdm::EnvironmentConditionData& dst);

   virtual void Merge(const SEEnvironmentalConditions& from);
public:

  virtual bool LoadFile(const std::string& environmentFile);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual cdm::EnvironmentData_eSurroundingType GetSurroundingType() const;
  virtual void SetSurroundingType(cdm::EnvironmentData_eSurroundingType name);
  virtual bool HasSurroundingType() const;
  virtual void InvalidateSurroundingType();

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

  cdm::EnvironmentData_eSurroundingType   m_SurroundingType;
  
  SEScalarMassPerVolume*            m_AirDensity;
  SEScalarLengthPerTime*            m_AirVelocity;
  SEScalarTemperature*              m_AmbientTemperature;
  SEScalarPressure*                  m_AtmosphericPressure;
  SEScalarHeatResistanceArea*        m_ClothingResistance;
  SEScalar0To1*                  m_Emissivity;
  SEScalarTemperature*              m_MeanRadiantTemperature;
  SEScalar0To1*                 m_RelativeHumidity;
  SEScalarTemperature*              m_RespirationAmbientTemperature;

  std::vector<SESubstanceFraction*>       m_AmbientGases;
  std::vector<const SESubstanceFraction*> m_cAmbientGases;

  std::vector<SESubstanceConcentration*>       m_AmbientAerosols;
  std::vector<const SESubstanceConcentration*> m_cAmbientAerosols;

  SESubstanceManager&               m_Substances;
};                  
