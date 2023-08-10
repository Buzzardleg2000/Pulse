/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEDyspnea : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEDyspnea(Logger* logger=nullptr);
  virtual ~SEDyspnea();

  static constexpr char const* Name = "Dyspnea";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEDyspnea& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  SEScalar0To1*           m_Severity;
};
