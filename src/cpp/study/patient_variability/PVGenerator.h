/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <algorithm>
#include <utility>

#include "PulseEngine.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/properties/SEScalar.h"

PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Patient.pb.h"
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "io/protobuf/PBUtils.h"

namespace pulse::study::patient_variability
{
  enum class eMode
  {
    Hemorrhage = 0,
    Validation
  };

  enum class eSetType
  {
    Slice = 0,
    Combo,
    Both
  };

  class Parameter
  {
  public:
    Parameter()
    {

    }
    void Set(double min, double max, double standard)
    {
      m_Values.clear();
      m_Values.push_back(min);
      m_Values.push_back(max);
      m_Values.push_back(standard);
    }

    double Min() const { return m_Values[0]; }
    double Max() const { return m_Values[1]; }
    double Standard() const { return m_Values[2]; }
    const std::vector<double>& Values() const { return m_Values; }

  protected:
    std::vector<double> m_Values;
  };

  class StepParameter
  {
  public:
    StepParameter(double lowerLimit, double upperLimit)
    {
      m_LowerLimit = lowerLimit;
      m_UpperLimit = upperLimit;
      Set(lowerLimit, upperLimit, upperLimit - lowerLimit);
    }
    StepParameter(double lowerLimit, double upperLimit, double stepSize)
    {
      m_LowerLimit = lowerLimit;
      m_UpperLimit = upperLimit;
      Set(lowerLimit, upperLimit, stepSize);
    }

    void AdjustBounds(double lowerLimit, double upperLimit)
    {
      AdjustBounds(lowerLimit, upperLimit, m_StepSize);
    }
    void AdjustBounds(double lowerLimit, double upperLimit, double stepSize)
    {
      m_LowerLimit = lowerLimit;
      m_UpperLimit = upperLimit;
      Set(lowerLimit, upperLimit, stepSize);
    }

    void Reset()
    {
      Set(m_Min, m_Max, m_StepSize);
    }
    void Set(double min, double max)
    {
      Set(min, max, max - min);
    }
    void Set(double min, double max, double stepSize, bool includeUpper = true)
    {
      if (min < m_LowerLimit)
      {
        min = m_LowerLimit;
        std::cerr << "Autocorrected invalid minimum to be the lowerLimit\n";
      }
      if (max > m_UpperLimit || max < 0)
      {
        max = m_UpperLimit;
        std::cerr << "Autocorrected invalid maximum to be the upperLimit\n";
      }
      if (stepSize > (m_UpperLimit - m_LowerLimit) || stepSize < 0)
      {
        stepSize = m_UpperLimit - m_LowerLimit;
        std::cerr << "Autocorrected invalid stepSize to be the upperLimit-lowerLimit\n";
      }

      m_Min = min;
      m_Max = max;
      m_StepSize = stepSize;
      m_Values.clear();
      int idx;
      if (m_Max == m_Min)
        m_Values.push_back(m_Max);
      else
      {
        int n = (int)((m_Max - m_Min) / m_StepSize);
        for (idx = 0; idx <= n; ++idx)
        {
          m_Values.push_back(m_Min + m_StepSize * idx);
        }
        if (includeUpper && m_Values[n] != m_UpperLimit)
          m_Values.push_back(m_UpperLimit);
      }
    }
    void AdjustStepSize(double stepSize)
    {
      Set(m_Min, m_Max, stepSize);
    }
    void Insert(double v)
    {
      // Make sure v is not in our list and add it
      if (std::find_if(m_Values.begin(), m_Values.end(), [v](const double& val) { return Equals(v, val); }) != m_Values.end())
        return;
      m_Values.push_back(v);

      // Sort the list
      std::sort(m_Values.begin(), m_Values.end());
    }

    double Min() const { return m_Min; }
    double Max() const { return m_Max; }
    double StepSize() const { return m_StepSize; }
    double LowerLimit() const { return m_LowerLimit; }
    double UpperLimit() const { return m_UpperLimit; }
    const std::vector<double>& Values() const { return m_Values; }
    static bool Equals(const double& v1, const double& v2) { return std::abs(GeneralMath::PercentDifference(v1, v2)) <= ZERO_APPROX; }

  protected:
    double m_Min, m_Max, m_StepSize;
    double m_LowerLimit, m_UpperLimit;
    std::vector<double> m_Values;
  };


  class PVGenerator : public Loggable
  {
    using PatientStateListData = pulse::study::bind::patient_variability::PatientStateListData;
  public:
    PVGenerator(Logger* logger=nullptr);
    virtual ~PVGenerator();


    eMode           GenerateMode = eMode::Validation;
    
    Parameter       Age_yr;
    Parameter       Height_cm;
    Parameter       BMI;
    Parameter       BFF;
    Parameter       HR_bpm;
    Parameter       MAP_mmHg;
    Parameter       PP_mmHg;
    Parameter       RR_bpm;

    StepParameter   HemorrhageSeverity;
    StepParameter   HemorrhageTriageTime_min;

    static std::string ToString(SEPatient& patient);
    void GenerateData(eSetType t, PatientStateListData& pList);

    void GenerateTriagePatientData(std::string rootDir, PatientStateListData& pList);
    void GenerateTriageInsultData(std::string rootDir, PatientStateListData& pList, PatientStateListData& pInsultsList);
    static std::string PatientToDescriptionString(SEPatient& patient);
    static std::string InsultsToDescriptionString(double hemorrhageSeverity, double airwayObstructionSeverity, double pneumothoraxSeverity);
    void GenerateTriagePatients(PatientStateListData& pList, const std::string& directory);
    void GenerateTriageInsults(PatientStateListData& pList, PatientStateListData& pInsultsList, const std::string& directory);

  protected:
    eSetType     m_SetType;
    unsigned int m_TotalPatients=0; // The actual number of patients for this data set, (NOT the number of runs, i.e. hemorrhage options can create more runs for each patient)
    unsigned int m_NumPatientsFailedToSetup = 0;
    unsigned int m_TotalRuns=0; // This is the actual number of runs to perform
    unsigned int m_Duplicates = 0;
    std::set<std::string> m_PatientSet;


    void GenerateSlicedPatientList(PatientStateListData& pList, const SEPatient& basePatient);
    void GenerateMultiVariableCombinationPatientList(PatientStateListData& pList);

    void ResetParameters(ePatient_Sex sex);

    void CreatePatient(PatientStateListData& pList, SEPatient& patient, const std::string& full_dir_path);

    void GenerateHemorrhageOptions(PatientStateListData& pList, SEPatient& patient, const std::string& full_dir_path);

    pulse::study::bind::patient_variability::PatientStateData* AddPatientToList(PatientStateListData& pList,
      SEPatient& patient, double runDuration_s, const std::string& full_dir_path);

    void CreateTriagePatient(PatientStateListData& pList, SEPatient& patient, const std::string& directory);
    pulse::study::bind::patient_variability::PatientStateData* AddTriagePatientToList(PatientStateListData& pList, SEPatient& patient, const std::string& full_dir_path);
  };
}
