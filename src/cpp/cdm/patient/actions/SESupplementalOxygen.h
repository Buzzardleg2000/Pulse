/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
class SESubstance;

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eSupplementalOxygen_Device { None = 0, NasalCannula, SimpleMask, NonRebreatherMask };
extern CDM_DECL const std::string& eSupplementalOxygen_Device_Name(eSupplementalOxygen_Device e);

class CDM_DECL SESupplementalOxygen : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESupplementalOxygen(Logger* logger=nullptr);
  virtual ~SESupplementalOxygen();

  static constexpr char const* Name = "Supplemental Oxygen";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SESupplementalOxygen& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual eSupplementalOxygen_Device GetDevice() const;
  virtual void SetDevice(eSupplementalOxygen_Device name);

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

protected:
  eSupplementalOxygen_Device              m_Device;
  SEScalarVolumePerTime*                  m_Flow;
  SEScalarVolume*                         m_Volume;
};
