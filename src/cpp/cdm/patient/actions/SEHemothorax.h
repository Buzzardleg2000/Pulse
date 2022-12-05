/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEHemothorax : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEHemothorax(Logger* logger=nullptr);
  virtual ~SEHemothorax();

  static constexpr char const* Name = "Hemothorax";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemothorax& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSide GetSide() const;
  virtual void SetSide(eSide name);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasFlowRate() const;
  virtual SEScalarVolumePerTime& GetFlowRate();
  virtual double GetFlowRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasTargetVolume() const;
  virtual SEScalarVolume& GetTargetVolume();
  virtual double GetTargetVolume(const VolumeUnit& unit) const;

  virtual bool HasBloodVolume() const;
  virtual SEScalarVolume& GetBloodVolume();
  virtual double GetBloodVolume(const VolumeUnit& unit) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eSide                   m_Side;
  SEScalar0To1*           m_Severity;
  SEScalarVolumePerTime*  m_FlowRate;
  SEScalarVolume*         m_TargetVolume;
  SEScalarVolume*         m_BloodVolume;
};
