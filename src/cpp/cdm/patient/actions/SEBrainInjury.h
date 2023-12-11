/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eBrainInjury_Type { Diffuse = 0, LeftFocal, RightFocal };
extern CDM_DECL const std::string& eBrainInjury_Type_Name(eBrainInjury_Type m);

class CDM_DECL SEBrainInjury : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEBrainInjury(Logger* logger=nullptr);
  virtual ~SEBrainInjury();

  static constexpr char const* Name = "Brain Injury";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEBrainInjury& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual eBrainInjury_Type GetType() const;
  virtual void SetType(eBrainInjury_Type t);

protected:
  SEScalar0To1*          m_Severity;
  eBrainInjury_Type      m_Type;
};
