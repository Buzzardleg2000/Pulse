/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SESepsisExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESepsisExacerbation(Logger* logger=nullptr);
  virtual ~SESepsisExacerbation();

  static constexpr char const* Name = "Sepsis Exacerbation";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SESepsisExacerbation& src, bool/*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasInfectionSeverity() const;
  virtual SEScalar0To1& GetInfectionSeverity();
  virtual double GetInfectionSeverity() const;

  virtual bool HasProgressionSeverity() const;
  virtual SEScalar0To1& GetProgressionSeverity();
  virtual double GetProgressionSeverity() const;

protected:
  SEScalar0To1*           m_InfectionSeverity;
  SEScalar0To1*           m_ProgressionSeverity;
};
