/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class ePatientAssessment_Type {
  ArterialBloodGasTest = 0,
  CompleteBloodCount,
  ComprehensiveMetabolicPanel,
  Urinalysis
};
extern CDM_DECL const std::string& ePatientAssessment_Type_Name(ePatientAssessment_Type m);

/**
 * @brief
 * Data formed at a level of a clinicians report. 
 * This is high level data, such as a mean or generalized data 
 * value intended to give a genaral overview of the patient (SEPatientAssessment) or other systems
 */
class CDM_DECL SEPatientAssessment : public Loggable
{
  friend class PBPatientAssesment;//friend the serialization class
public:

  SEPatientAssessment(Logger* logger);
  virtual ~SEPatientAssessment();

  virtual void Clear();

  virtual bool SerializeToString(std::string& output, eSerializationFormat m) const=0;
  virtual bool SerializeToFile(const std::string& filename) const=0;
};  