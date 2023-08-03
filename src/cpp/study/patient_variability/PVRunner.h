/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <thread>
#include <map>

#include "PulseEngine.h"

PUSH_PROTO_WARNINGS
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "io/protobuf/PBUtils.h"

namespace pulse::study::patient_variability
{
  enum class eStandardValidationType
  {
    None = 0,
    Baseline,
    Current
  };

  class PVRunner : public Loggable
  {
  public:
    PVRunner(const std::string& rootDir, eStandardValidationType t, Logger* logger=nullptr);
    virtual ~PVRunner();

    bool PostProcessOnly = false;
    eSerializationFormat SerializationFormat = eSerializationFormat::BINARY;

    bool Run(const std::string& filename);
    bool Run(pulse::study::bind::patient_variability::PatientStateListData& patients);

    bool RunTriage(pulse::study::bind::patient_variability::PatientStateListData& patients);

  protected:
    bool Run();
    bool SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& dst) const;
    bool SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const;
    bool SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst);
    bool SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::PatientStateListData& dst);

    void BlockSystemCall(const std::string& cmd);

    void ControllerLoop();
    bool RunPatient(pulse::study::bind::patient_variability::PatientStateData& patient);
    pulse::study::bind::patient_variability::PatientStateData* GetNextPatient();
    void CompletePatient(pulse::study::bind::patient_variability::PatientStateData& patientState);

    bool AggregateResults(pulse::study::bind::patient_variability::PatientStateData& patient, const std::vector<std::string>& validation_files, Logger* logger);

    std::mutex  m_VectorMutex;
    std::mutex  m_SystemMutex;

    eStandardValidationType m_StandardValidationType;
    std::string m_RootDir;
    std::string m_PatientResultsListFile;
    std::set<unsigned int> m_PatientsToRun;
    std::vector<std::thread>   m_Threads;
    pulse::study::bind::patient_variability::PatientStateListData* m_PatientList;
    pulse::study::bind::patient_variability::PatientStateListData* m_PatientResultsList;

    bool RunTriage();
    void TriageControllerLoop();
    bool RunTriagePatient(pulse::study::bind::patient_variability::PatientStateData& patient);
    bool AdvanceAndTrackTime_s(double time_s, PhysiologyEngine& engine);
  };
}
