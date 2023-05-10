/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"
#include "cdm/system/physiology/SERespiratorySystem.h"

class CDM_DECL SEAcuteRespiratoryDistressSyndrome : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEAcuteRespiratoryDistressSyndrome(Logger* logger=nullptr);
  virtual ~SEAcuteRespiratoryDistressSyndrome();

  static constexpr char const* Name = "Acute Respiratory Distress Syndrome";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEAcuteRespiratoryDistressSyndrome& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual LungImpairmentMap& GetSeverities();
  virtual const LungImpairmentMap& GetSeverities() const;
  virtual bool HasSeverity(eLungCompartment cmpt) const;
  virtual SEScalar0To1& GetSeverity(eLungCompartment cmpt);
  virtual const SEScalar0To1* GetSeverity(eLungCompartment cmpt) const;

protected:
  LungImpairmentMap m_Severities;
};