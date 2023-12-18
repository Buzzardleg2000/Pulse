/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

// Keep enums in sync with appropriate schema/cdm/SubstanceEnums.proto file !!
enum class eSubstance_IonicState { NullIonicState = 0, Acid, Base, Neutral, WeakBase };
extern CDM_DECL const std::string& eSubstance_IonicState_Name(eSubstance_IonicState m);

// Keep enums in sync with appropriate schema/cdm/SubstanceEnums.proto file !!
enum class eSubstance_BindingProtein { NullBindingProtein = 0, AAG, Albumin, Lipoprotein };
extern CDM_DECL const std::string& eSubstance_BindingProtein_Name(eSubstance_BindingProtein m);

class CDM_DECL SESubstancePhysicochemicals : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstancePhysicochemicals(Logger* logger);
  virtual ~SESubstancePhysicochemicals();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

public:
  virtual bool HasAcidDissociationConstant() const;
  virtual SEScalar& GetAcidDissociationConstant();
  virtual double GetAcidDissociationConstant() const;

  virtual eSubstance_BindingProtein GetBindingProtein() const;
  virtual void SetBindingProtein(eSubstance_BindingProtein state);
  virtual bool HasBindingProtein() const;
  virtual void InvalidateBindingProtein();

  virtual bool HasBloodPlasmaRatio() const;
  virtual SEScalar& GetBloodPlasmaRatio();
  virtual double GetBloodPlasmaRatio() const;

  virtual bool HasFractionUnboundInPlasma() const;
  virtual SEScalar0To1& GetFractionUnboundInPlasma();
  virtual double GetFractionUnboundInPlasma() const;

  virtual eSubstance_IonicState GetIonicState() const;
  virtual void SetIonicState(eSubstance_IonicState state);
  virtual bool HasIonicState() const;
  virtual void InvalidateIonicState();

  virtual bool HasLogP() const;
  virtual SEScalar& GetLogP();
  virtual double GetLogP() const;

  virtual bool HasOralAbsorptionRateConstant() const;
  virtual SEScalar& GetOralAbsorptionRateConstant();
  virtual double GetOralAbsorptionRateConstant() const;

protected: 
  SEScalar*                       m_AcidDissociationConstant;
  eSubstance_BindingProtein       m_BindingProtein;
  SEScalar*                       m_BloodPlasmaRatio;
  SEScalar0To1*                   m_FractionUnboundInPlasma;
  eSubstance_IonicState           m_IonicState;
  SEScalar*                       m_LogP;
  SEScalar*                       m_OralAbsorptionRateConstant;
};