/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEFunction.h"
namespace cdm { class FunctionElectricPotentialVsTimeData; }

class CDM_DECL SEFunctionElectricPotentialVsTime : public SEFunction
{
public:

  SEFunctionElectricPotentialVsTime();
  virtual ~SEFunctionElectricPotentialVsTime();

  virtual void Clear();

  static void Load(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst);
  static cdm::FunctionElectricPotentialVsTimeData* Unload(const SEFunctionElectricPotentialVsTime& src);
protected:
  static void Serialize(const cdm::FunctionElectricPotentialVsTimeData& src, SEFunctionElectricPotentialVsTime& dst);
  static void Serialize(const SEFunctionElectricPotentialVsTime& src, cdm::FunctionElectricPotentialVsTimeData& dst);
public:

  double                               GetIndependentValue(size_t index) = delete;
  virtual double                       GetTimeValue(size_t index, const TimeUnit& unit);
  virtual std::vector<double>&         GetTime();
  virtual const TimeUnit*              GetTimeUnit();
  virtual void                         SetTimeUnit(const TimeUnit& unit);

  double                               GetDependentValue(size_t index) = delete;
  virtual double                       GetElectricPotentialValue(size_t index, const ElectricPotentialUnit& unit);
  virtual std::vector<double>&         GetElectricPotential();
  virtual const ElectricPotentialUnit* GetElectricPotentialUnit();
  virtual void                         SetElectricPotentialUnit(const ElectricPotentialUnit& unit);

  virtual SEFunctionElectricPotentialVsTime* InterpolateToTime(std::vector<double>& Independent, const TimeUnit& unit);

protected:

  const TimeUnit*              m_TimeUnit;
  const ElectricPotentialUnit* m_ElectricPotentialUnit;
};