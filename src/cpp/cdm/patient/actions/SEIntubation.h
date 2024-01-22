/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eIntubation_Type { Off = 0, Esophageal, LeftMainstem, RightMainstem, Tracheal,
  Oropharyngeal, Nasopharyngeal };
extern CDM_DECL const std::string& eIntubation_Type_Name(eIntubation_Type m);

class CDM_DECL SEIntubation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEIntubation(Logger* logger=nullptr);
  virtual ~SEIntubation();

  static constexpr char const* Name = "Intubation";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEIntubation& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eIntubation_Type GetType() const;
  virtual void SetType(eIntubation_Type t);

  virtual bool HasAirwayResistance() const;
  virtual SEScalarPressureTimePerVolume& GetAirwayResistance();
  virtual double GetAirwayResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  eIntubation_Type               m_Type;
  SEScalarPressureTimePerVolume* m_AirwayResistance;
  SEScalar0To1*                  m_Severity;
};
