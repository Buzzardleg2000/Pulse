/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SESepsis : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SESepsis(Logger* logger=nullptr);
  virtual ~SESepsis();

  static constexpr char const* Name = "Sepsis";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SESepsis& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasInfectionSeverity() const;
  virtual SEScalar0To1& GetInfectionSeverity();
  virtual double GetInfectionSeverity() const;

  virtual bool HasProgressionSeverity() const;
  virtual SEScalar0To1& GetProgressionSeverity();
  virtual double GetProgressionSeverity() const;

protected:
  SEScalar0To1* m_InfectionSeverity;
  SEScalar0To1* m_ProgressionSeverity;
};
