/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include "properties/SEProperty.h"
#include "utils/unitconversion/UCCommon.h"
#include "bind/cdm/Properties.pb.h"

class DLL_DECL SEHistogram : public SEProperty
{
public:

  SEHistogram();
  virtual ~SEHistogram();

  virtual void Clear(); //clear memory

  static void Load(const cdm::HistogramData& src, SEHistogram& dst);
  static cdm::HistogramData* Unload(const SEHistogram& src);
protected:
  static void Serialize(const cdm::HistogramData& src, SEHistogram& dst);
  static void Serialize(const SEHistogram& src, cdm::HistogramData& dst);
public:
  virtual bool                          IsValid() const;
  virtual void                          Invalidate();

  virtual size_t                        NumberOfBins() const;
  virtual size_t                        NumberOfBoundaries() const;
  
  double                                GetDependentValue(unsigned int index) const;
  std::vector<double>&                  GetDependent();      
  const std::vector<double>&            GetDependent() const;

  double                                GetIndependentValue(unsigned int index) const;
  std::vector<double>&                  GetIndependent();
  const std::vector<double>&            GetIndependent() const;
  
protected:

  std::vector<double> m_Dependent;
  std::vector<double> m_Independent;
};