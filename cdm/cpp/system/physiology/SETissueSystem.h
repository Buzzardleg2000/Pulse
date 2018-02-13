/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
namespace cdm { class TissueSystemData; }

class CDM_DECL SETissueSystem : public SESystem
{
public:

  SETissueSystem(Logger* logger);
  virtual ~SETissueSystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);
  
  static void Load(const cdm::TissueSystemData& src, SETissueSystem& dst);
  static cdm::TissueSystemData* Unload(const SETissueSystem& src);
protected:
  static void Serialize(const cdm::TissueSystemData& src, SETissueSystem& dst);
  static void Serialize(const SETissueSystem& src, cdm::TissueSystemData& dst);

public:

  virtual bool HasCarbonDioxideProductionRate() const;
  virtual SEScalarVolumePerTime& GetCarbonDioxideProductionRate();
  virtual double GetCarbonDioxideProductionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasExtracellularFluidVolume() const;
  virtual SEScalarVolume& GetExtracellularFluidVolume();
  virtual double GetExtracellularFluidVolume(const VolumeUnit& unit) const;
 
  virtual bool HasExtravascularFluidVolume() const;
  virtual SEScalarVolume& GetExtravascularFluidVolume();
  virtual double GetExtravascularFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasIntracellularFluidVolume() const;
  virtual SEScalarVolume& GetIntracellularFluidVolume();
  virtual double GetIntracellularFluidVolume(const VolumeUnit& unit) const;

  virtual bool HasIntracellularFluidPH() const;
  virtual SEScalar& GetIntracellularFluidPH();
  virtual double GetIntracellularFluidPH() const;

  virtual bool HasOxygenConsumptionRate() const;
  virtual SEScalarVolumePerTime& GetOxygenConsumptionRate();
  virtual double GetOxygenConsumptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasRespiratoryExchangeRatio() const;
  virtual SEScalar& GetRespiratoryExchangeRatio();
  virtual double GetRespiratoryExchangeRatio() const;


protected:
  SEScalarVolumePerTime*  m_CarbonDioxideProductionRate;
  SEScalarVolume*         m_ExtracellularFluidVolume;//planned
  SEScalarVolume*         m_ExtravascularFluidVolume;//planned
  SEScalarVolume*         m_IntracellularFluidVolume;//planned
  SEScalar*               m_IntracellularFluidPH;
  SEScalarVolumePerTime*  m_OxygenConsumptionRate;
  SEScalar*               m_RespiratoryExchangeRatio;


};
