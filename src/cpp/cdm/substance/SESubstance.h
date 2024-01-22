/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceAerosolization;
class SESubstanceClearance;
class SESubstancePharmacokinetics;
class SESubstancePharmacodynamics;

// Keep enums in sync with appropriate schema/cdm/SubstanceEnums.proto file !!
enum class eSubstance_State { NullState = 0, Solid, Liquid, Gas, Molecular };
extern CDM_DECL const std::string& eSubstance_State_Name(eSubstance_State m);

class CDM_DECL SESubstance : public Loggable
{
  friend class PBSubstance;//friend the serialization class
  friend class SESubstanceManager;
protected:
  SESubstance(const std::string& name, Logger* logger);
public:
  virtual ~SESubstance();

  virtual void Clear();

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  virtual const SEScalar* GetScalar(const std::string& name);
  
  virtual std::string GetName() const;

  virtual eSubstance_State GetState() const;
  virtual void SetState(eSubstance_State state);
  virtual bool HasState() const;
  virtual void InvalidateState();

  virtual bool HasDensity() const;
  virtual SEScalarMassPerVolume& GetDensity();
  virtual double GetDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasMolarMass() const;
  virtual SEScalarMassPerAmount& GetMolarMass();
  virtual double GetMolarMass(const MassPerAmountUnit& unit) const;

  virtual bool HasValence() const;
  virtual SEScalar& GetValence();
  virtual double GetValence() const;

  // Diffusion-ish
  virtual bool HasMaximumDiffusionFlux() const;
  virtual SEScalarMassPerAreaTime& GetMaximumDiffusionFlux();
  virtual double GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const;

  virtual bool HasMichaelisCoefficient() const;
  virtual SEScalar& GetMichaelisCoefficient();
  virtual double GetMichaelisCoefficient() const;

  // Liquid-ish
  virtual bool HasAerosolization() const;
  virtual SESubstanceAerosolization& GetAerosolization();
  virtual const SESubstanceAerosolization* GetAerosolization() const;
  virtual void RemoveAerosolization();

  virtual bool HasBloodConcentration() const;
  virtual SEScalarMassPerVolume& GetBloodConcentration();
  virtual double GetBloodConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMassInBody() const;
  virtual SEScalarMass& GetMassInBody();
  virtual double GetMassInBody(const MassUnit& unit) const;

  virtual bool HasMassInBlood() const;
  virtual SEScalarMass& GetMassInBlood();
  virtual double GetMassInBlood(const MassUnit& unit) const;

  virtual bool HasMassInTissue() const;
  virtual SEScalarMass& GetMassInTissue();
  virtual double GetMassInTissue(const MassUnit& unit) const;

  virtual bool HasPlasmaConcentration() const;
  virtual SEScalarMassPerVolume& GetPlasmaConcentration();
  virtual double GetPlasmaConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasSystemicMassCleared() const;
  virtual SEScalarMass& GetSystemicMassCleared();
  virtual double GetSystemicMassCleared(const MassUnit& unit) const;

  virtual bool HasTissueConcentration() const;
  virtual SEScalarMassPerVolume& GetTissueConcentration();
  virtual double GetTissueConcentration(const MassPerVolumeUnit& unit) const;

  // Gas-ish
  virtual bool HasAlveolarTransfer() const;
  virtual SEScalarVolumePerTime& GetAlveolarTransfer();
  virtual double GetAlveolarTransfer(const VolumePerTimeUnit& unit) const;

  virtual bool HasDiffusingCapacity() const;
  virtual SEScalarVolumePerTimePressure& GetDiffusingCapacity();
  virtual double GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasEndTidalFraction() const;
  virtual SEScalar0To1& GetEndTidalFraction();
  virtual double GetEndTidalFraction() const;

  virtual bool HasEndTidalPressure() const;
  virtual SEScalarPressure& GetEndTidalPressure();
  virtual double GetEndTidalPressure(const PressureUnit& unit) const;

  virtual bool HasRelativeDiffusionCoefficient() const;
  virtual SEScalar& GetRelativeDiffusionCoefficient();
  virtual double GetRelativeDiffusionCoefficient() const;

  virtual bool HasSolubilityCoefficient() const;
  virtual SEScalarInversePressure& GetSolubilityCoefficient();
  virtual double GetSolubilityCoefficient(const InversePressureUnit& unit) const;

  virtual bool HasClearance() const;
  virtual SESubstanceClearance& GetClearance();
  virtual const SESubstanceClearance* GetClearance() const;
  virtual void RemoveClearance();

  virtual bool HasPK() const;
  virtual SESubstancePharmacokinetics& GetPK();
  virtual const SESubstancePharmacokinetics* GetPK() const;
  virtual void RemovePK();

  virtual bool HasPD() const;
  virtual SESubstancePharmacodynamics& GetPD();
  virtual const SESubstancePharmacodynamics* GetPD() const;
  virtual void RemovePD();

  
protected: 

  std::string                       m_Name;
  eSubstance_State                  m_State;
  SEScalarMassPerVolume*            m_Density;
  SEScalarMassPerAmount*            m_MolarMass;
  SEScalar*                         m_Valence;

  SEScalarMassPerAreaTime*          m_MaximumDiffusionFlux;
  SEScalar*                         m_MichaelisCoefficient;

  SESubstanceAerosolization*        m_Aerosolization;
  SEScalarMassPerVolume*            m_BloodConcentration;
  SEScalarMass*                     m_MassInBody;
  SEScalarMass*                     m_MassInBlood;
  SEScalarMass*                     m_MassInTissue;
  SEScalarMassPerVolume*            m_PlasmaConcentration;
  SEScalarMass*                     m_SystemicMassCleared;
  SEScalarMassPerVolume*            m_TissueConcentration;

  SEScalarVolumePerTime*            m_AlveolarTransfer;
  SEScalarVolumePerTimePressure*    m_DiffusingCapacity;
  SEScalar0To1*                     m_EndTidalFraction;
  SEScalarPressure*                 m_EndTidalPressure;
  SEScalar*                         m_RelativeDiffusionCoefficient;
  SEScalarInversePressure*          m_SolubilityCoefficient;

  SESubstanceClearance*             m_Clearance;
  SESubstancePharmacokinetics*      m_PK;
  SESubstancePharmacodynamics*      m_PD;
};