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
#include "bind/cdm/PatientAssessments.pb.h"

/**
 * @brief
 * Data formed at a level of a clinicians report. 
 * This is high level data, such as a mean or generalized data 
 * value intended to give a genaral overview of the patient (SEPatientAssessment) or other systems
 */
class DLL_DECL SEPatientAssessment : public Loggable
{
public:

  SEPatientAssessment(Logger* logger);
  virtual ~SEPatientAssessment();

  virtual void Reset(); 
  virtual void Clear();

  static void Load(const cdm::PatientAssessmentData& src, SEPatientAssessment& dst);
  static cdm::PatientAssessmentData* Unload(const SEPatientAssessment& src);
protected:
  static void Serialize(const cdm::PatientAssessmentData& src, SEPatientAssessment& dst);
  static void Serialize(const SEPatientAssessment& src, cdm::PatientAssessmentData& dst);

public:
  
};  