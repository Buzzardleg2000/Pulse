/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEAcuteRespiratoryDistressSyndromeExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEAcuteRespiratoryDistressSyndromeExacerbation();
  virtual ~SEAcuteRespiratoryDistressSyndromeExacerbation();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEAcuteRespiratoryDistressSyndromeExacerbation& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasLeftLungAffected() const;
  virtual SEScalar0To1& GetLeftLungAffected();
  virtual double GetLeftLungAffected() const;

  virtual bool HasRightLungAffected() const;
  virtual SEScalar0To1& GetRightLungAffected();
  virtual double GetRightLungAffected() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1* m_Severity;
  SEScalar0To1* m_LeftLungAffected;
  SEScalar0To1* m_RightLungAffected;
};