/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "PulseEngine.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/PulseConfiguration.h"
#include "engine/PulseScenario.h"

#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/io/protobuf/PBPatient.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/patient/actions/SEAirwayObstruction.h"
#include "cdm/patient/actions/SETensionPneumothorax.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/utils/ConfigParser.h"
#include "cdm/utils/CSV.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/TimingProfile.h"

using namespace pulse;
using eFailure = pulse::study::bind::patient_variability::PatientStateData_eFailure;

namespace pulse::study::patient_variability
{
  PVRunner::PVRunner(const std::string& rootDir, eStandardValidationType t, Logger* logger) : Loggable(logger)
  {
    m_RootDir = rootDir;
    m_PatientList = nullptr;
    m_PatientResultsList = nullptr;
    m_StandardValidationType = t;
  }
  PVRunner::~PVRunner()
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
  }

  bool PVRunner::Run(const std::string& filename)
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
    m_PatientList = new pulse::study::bind::patient_variability::PatientStateListData();
    m_PatientResultsList = new pulse::study::bind::patient_variability::PatientStateListData();
    if (!SerializeFromFile(filename, *m_PatientList))
      return false;

    return Run();
  }
  bool PVRunner::Run(pulse::study::bind::patient_variability::PatientStateListData& simList)
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
    m_PatientList = &simList;
    m_PatientResultsList = new pulse::study::bind::patient_variability::PatientStateListData();

    bool b = Run();
    m_PatientList = nullptr;
    SAFE_DELETE(m_PatientResultsList);
    return b;
  }


  bool PVRunner::Run()
  {
    if (m_PatientList->patientstate_size() == 0)
      return true; // Nothing to do

    TimingProfile profiler;
    profiler.Start("Total");

    if (m_PatientList->patientstate()[0].has_validation() && m_StandardValidationType != eStandardValidationType::None)
    {
      // Generate json files for standard male and female validation data from baselines
      // If json files already exist don't redo that work
      if (!FileExists(m_RootDir + "/standard_results.json"))
      {
        // Run standard patients if we aren't using baseline
        std::string dir = "./verification/scenarios/validation/systems";
        if (m_StandardValidationType == eStandardValidationType::Current)
        {
          Info("Executing System Validation...");
          std::string runCommand = "cmake -DTYPE:STRING=SystemValidation -P run.cmake";
          dir = "./test_results/scenarios/validation/systems";
          std::system(runCommand.c_str());
          Info("Finished System Validation!");

          std::string content;
          ReadFile("./test_results/SystemVerificationReport.json", content);
          WriteFile(content, m_RootDir + "/SystemVerificationReport-StandardPatients.json");
          ReadFile("./test_results/SystemVerification.html", content);
          WriteFile(content, m_RootDir + "/SystemVerification-StandardPatients.html");
          ReadFile("./test_results/SystemValidation.html", content);
          WriteFile(content, m_RootDir + "/SystemValidation-StandardPatients.html");
        }

        Info("Validating Results in: " + dir);
        std::string command = "cmake -DTYPE:STRING=ValidateFolder -DARG1:STRING=" + dir + " -DARG2:STRING=false -P run.cmake";
        std::system(command.c_str());
        Info("Finished Validating");

        // Standard male
        pulse::study::bind::patient_variability::PatientStateListData standardResults;
        std::vector<std::string> standardPatients{ "StandardMale", "StandardFemale" };
        for (auto& standard : standardPatients)
        {
          // Load patient info to save for later analysis
          SEPatient p(GetLogger());
          p.SerializeFromFile("./patients/" + standard + ".json");

          auto patientStateData = standardResults.add_patientstate();
          pulse::cdm::bind::PatientData* patientData = patientStateData->mutable_patient();
          PBPatient::Serialize(p, *patientData);

          patientStateData->set_outputbasefilename(standard);

          // Save validation results
          std::vector<std::string> validation_files;
          ListFiles(dir, validation_files, true, "-" + standard + "-ValidationResults.json");
          if (!AggregateResults(*patientStateData, validation_files, GetLogger()))
          {
            GetLogger()->Warning("Unable to aggregate results for " + standard);
          }
        }
        SerializeToFile(standardResults, m_RootDir + "/standard_results.json");
      }
    }

    if (SerializationFormat == eSerializationFormat::JSON)
      m_PatientResultsListFile = m_RootDir + "/patient_results.json";
    else
      m_PatientResultsListFile = m_RootDir + "/patient_results.pbb";
    if (FileExists(m_PatientResultsListFile))
    {
      if (PostProcessOnly)
      {
        if (!DeleteFile(m_PatientResultsListFile, 5))
          Error("Unable to remove file " + m_PatientResultsListFile);
      }
      else if (!SerializeFromFile(m_PatientResultsListFile, *m_PatientResultsList))
      {
        GetLogger()->Warning("Unable to read found results file");
      }
    }
    else
    {
      // If there is no file, let's go through the patients and cull out any that cannot setup
      size_t failedPatients = 0;
      for (int i = 0; i < m_PatientList->patientstate_size(); i++)
      {
        SEPatient p(nullptr);
        auto& patientStateData = m_PatientList->patientstate()[i];
        PBPatient::Serialize(patientStateData.patient(), p);
        if (!pulse::human_adult_whole_body::SetupPatient(p))
        {
          failedPatients++;
          auto patientData = m_PatientResultsList->add_patientstate();
          patientData->CopyFrom(patientStateData);
          patientData->set_failure(eFailure::PatientStateData_eFailure_FailedSetup);
          Info("[!Setup]  " + p.GetName() + "_" + PVGenerator::ToString(p));
        }
      }
      // Write this file to disk
      if (failedPatients > 0)
      {
        Info("There are " + std::to_string(failedPatients) + " invalid patients in this dataset");
        if (!SerializeToFile(*m_PatientResultsList, m_PatientResultsListFile))
          Error("Unable to write initial results file, you may loose all your runs, check permissions.");
      }
    }

    Info("Looking for any previously run patients...");
    // Get the ID's of Patients we need to run
    m_PatientsToRun.clear();
    for (int i = 0; i < m_PatientList->patientstate_size(); i++)
      m_PatientsToRun.insert(m_PatientList->patientstate()[i].id());
    // Remove any id's we have in the results
    if (m_PatientResultsList->patientstate_size() > 0)
    {
      Info("Found " + std::to_string(m_PatientResultsList->patientstate_size()) + " previously run Patients");
      for (int i = 0; i < m_PatientResultsList->patientstate_size(); i++)
        m_PatientsToRun.erase(m_PatientResultsList->patientstate()[i].id());
    }
    else
      Info("None found, starting from scratch");

    size_t numSimsToRun = m_PatientList->patientstate_size() - m_PatientResultsList->patientstate_size();
    if (numSimsToRun == 0)
    {
      Info("All Patients are run in the results file");
      return true;
    }
    size_t processor_count = std::thread::hardware_concurrency();
    if (processor_count == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }
    // Let's not kill the computer this is running on...
    if (processor_count > 1)
      processor_count -= 1;
    // Let's not kick off more threads than we need
    if (processor_count > numSimsToRun)
      processor_count = numSimsToRun;
    Info("Starting " + std::to_string(processor_count) + " Threads to run " + std::to_string(m_PatientsToRun.size()) + " Patients");
    // Crank up our threads
    for (size_t p = 0; p < processor_count; p++)
      m_Threads.push_back(std::thread(&PVRunner::ControllerLoop, this));

    for (size_t p = 0; p < processor_count; p++)
      m_Threads[p].join();

    // Let's count up our stats
    size_t SetupErrors = 0;
    size_t StabilizationErrors = 0;
    size_t RuntimeErrors = 0;
    size_t FatalRuntimeErrors = 0;
    size_t ValidRuns = 0;
    Info("Total Patients: " + std::to_string(m_PatientResultsList->patientstate_size()));
    for (int i = 0; i < m_PatientResultsList->patientstate_size(); i++)
    {
      auto& patientStateData = m_PatientResultsList->patientstate()[i];
      if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FailedSetup)
        SetupErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FailedStabilization)
        StabilizationErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_RuntimeError)
        RuntimeErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FatalRuntimeError)
        FatalRuntimeErrors++;
      else
        ValidRuns++;
    }
    Info("Total Valid Patients: " + std::to_string(ValidRuns));
    Info("Total SetupErrors: " + std::to_string(SetupErrors));
    Info("Total StabilizationErrors: " + std::to_string(StabilizationErrors));
    Info("Total RuntimeErrors: " + std::to_string(RuntimeErrors));
    Info("Total FatalRuntimeErrors: " + std::to_string(FatalRuntimeErrors));

    Info("It took " + std::to_string(profiler.GetElapsedTime_s("Total")) + "s to run this Patient list");
    profiler.Clear();
    return true;
  }

  void PVRunner::ControllerLoop()
  {
    CreatePath(m_RootDir);

    pulse::study::bind::patient_variability::PatientStateData* patient = nullptr;
    while (true)
    {
      try
      {
        patient = GetNextPatient();
        if (patient == nullptr)
          break;

        if (patient->failure() != eFailure::PatientStateData_eFailure_None)
        {
          Info("Not running invalid patient " + std::to_string(patient->id()) + " " + patient->outputbasefilename());
          CompletePatient(*patient);
        }
        else
        {
          Info("Running " + patient->outputbasefilename());
          if (!RunPatient(*patient) && patient->failure() == eFailure::PatientStateData_eFailure_None)
            GetLogger()->Error("Unable to run patient " + patient->outputbasefilename());
        }
      }
      catch (CommonDataModelException& cdm_ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(cdm_ex.what());
        std::cerr << cdm_ex.what() << std::endl;
      }
      catch (std::exception& ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(ex.what());
        std::cerr << ex.what() << std::endl;
      }
      catch (...)
      {
        std::cerr << "Unable to run Patient " << patient->outputbasefilename() << std::endl;
      }
    }
  }

  enum class eHemorrhageState
  {
    None = 0,
    Start,
    TrackStart,
    TrackTriage,
    Finish
  };

  bool CheckPatientSetup(const SEPatient& p)
  {
    SEPatient c(p.GetLogger());
    c.Copy(p);
    return pulse::human_adult_whole_body::SetupPatient(c);
  }

  bool PVRunner::RunPatient(pulse::study::bind::patient_variability::PatientStateData& patientState)
  {
    TimingProfile profiler;
    profiler.Start("Total");
    profiler.Start("Status");


    std::string csvFilename;
    // For validation runs only
    std::map<std::string, std::vector<std::string>> allScenarioRequests;
    // For hemorrhage runs only
    eHemorrhageState hState = eHemorrhageState::None;
    pulse::study::bind::patient_variability::PatientStateData_HemorrhageData* hemorrhageData = nullptr;
    if (patientState.has_hemorrhage())
    {
      hState = eHemorrhageState::Start;
      hemorrhageData = patientState.mutable_hemorrhage();
    }
    std::string patientName = patientState.patient().name();

    if (!PostProcessOnly)
    {
      PulseConfiguration cfgChanges;
      auto pulse = CreatePulseEngine();
      // No logging to console (when threaded)
      pulse->GetLogger()->LogToConsole(false);
      // But, do write a log file
      pulse->GetLogger()->SetLogFile(m_RootDir + patientState.outputbasefilename() + "/patient.log");

      SEPatientConfiguration pc(pulse->GetLogger());
      PBPatient::Serialize(patientState.patient(), pc.GetPatient());
      if (!CheckPatientSetup(pc.GetPatient()))
      {
        Error("[" + patientName + "] Could not setup patient");
        patientState.set_failure(eFailure::PatientStateData_eFailure_FailedSetup);
        CompletePatient(patientState);
        return false;
      }

      if (patientState.has_validation())
      {
        // Write out the computed patient baseline values so we can see how well we met them
        cfgChanges.EnableWritePatientBaselineFile(eSwitch::On);
        cfgChanges.SetInitialPatientBaselineFilepath(m_RootDir + patientState.outputbasefilename() + "/patient.init.json");
        pulse->SetConfigurationOverride(&cfgChanges);

        // Read in all System and Patient validation scenarios
        // Add in all the data requests from all of those files
        csvFilename = m_RootDir + patientState.outputbasefilename() + "/AllValidationResults.csv";

        std::string scenario_dir;
        ConfigSet* config = ConfigParser::FileToConfigSet("run.config");
        if (config->HasKey("scenario_dir"))
        {
          scenario_dir = config->GetValue("scenario_dir");

          //Retrieve all patients validation scenarios
          std::vector<std::string> scenario_files;
          ListFiles(scenario_dir + "/validation/patients", scenario_files, true, "Validation.json");

          //Retrieve all systems validation scenarios
          ListFiles(scenario_dir + "/validation/systems", scenario_files, true, "Validation.json");

          //Copy all data requests from each scenario
          for (auto scenario_filepath : scenario_files)
          {
            PulseScenario s(pulse->GetLogger());
            s.SerializeFromFile(scenario_filepath);

            std::string scenario_filename;
            SplitFilename(scenario_filepath, scenario_filename);
            std::string output_csv_file = Replace(scenario_filename, ".json", "Results.csv");
            allScenarioRequests.insert(std::pair<std::string, std::vector<std::string>>(output_csv_file, std::vector<std::string>()));

            for (auto dr : s.GetDataRequestManager().GetDataRequests())
            {
              SEDataRequest& new_dr = pulse->GetEngineTracker()->GetDataRequestManager().CopyDataRequest(*dr);
              allScenarioRequests[output_csv_file].push_back(new_dr.GetHeaderName());
            }
          }
        }
        else
        {
          Error("[" + patientName + "] Cannot find scenario root... cannot add any data requests...");
          patientState.set_failure(eFailure::PatientStateData_eFailure_RuntimeError);
          return false;
        }
      }
      else if (hemorrhageData)
      {
        csvFilename = m_RootDir + patientState.outputbasefilename() + "/HemorrhageResults.csv";
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent", MassUnit::g);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhageRate", VolumePerTimeUnit::mL_Per_s);
        pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhagedVolume", VolumeUnit::mL);
      }
      else
      {
        Error("[" + patientName + "] Unknown run mode");
        patientState.set_failure(eFailure::PatientStateData_eFailure_RuntimeError);
        return false;
      }
      pulse->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(csvFilename);

      if (!pulse->InitializeEngine(pc))
      {
        Error("[" + patientName + "] Unable to stabilize engine");
        patientState.set_failure(eFailure::PatientStateData_eFailure_FailedStabilization);
        CompletePatient(patientState);
        return false;
      }
      // Get the Initial Patient Baseline File
      // This is the patient after SetupPatient has been called, and before stabilization
      SEPatient baselinePatient(GetLogger());
      baselinePatient.SerializeFromFile(cfgChanges.GetInitialPatientBaselineFilepath());
      PBPatient::Serialize(baselinePatient, *patientState.mutable_setuppatient());

      patientState.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
      pulse->GetLogger()->Info("[" + patientName + "] It took " + cdm::to_string(patientState.stabilizationtime_s()) + "s to stabilize this Patient");

      pulse->GetEngineTracker()->TrackData(0);
      double dT_s = pulse->GetTimeStep(TimeUnit::s);
      int count = patientState.maxsimulationtime_s() / dT_s;

      for (int i = 0; i < count; i++)
      {
        if (!pulse->AdvanceModelTime()) // Compute 1 time step
        {
          Error("[" + patientName + "] Unable to advance time");
          patientState.set_failure(eFailure::PatientStateData_eFailure_FatalRuntimeError);
          return false;
        }

        double time_s = pulse->GetSimulationTime(TimeUnit::s);
        pulse->GetEngineTracker()->TrackData(time_s);

        // Hemorrhage Mode //
        if (hemorrhageData)
        {
          switch (hState)
          {
          case eHemorrhageState::Start:
          {
            if (time_s > patientState.hemorrhage().starttime_s())
            {
              SEHemorrhage hemorrhage;
              hemorrhage.SetCompartment((eHemorrhage_Compartment)patientState.hemorrhage().action().compartment());
              hemorrhage.GetSeverity().SetValue(patientState.hemorrhage().action().severity().scalar0to1().value());
              pulse->ProcessAction(hemorrhage);
              hState = eHemorrhageState::TrackStart;
            }
            break;
          }
          case eHemorrhageState::TrackStart:
          {
            if (pulse->GetCardiovascularSystem()->HasTotalHemorrhageRate())
            {
              double flowRate_mL_Per_s = pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s);
              if (flowRate_mL_Per_s > 0)
              {
                hemorrhageData->set_initialbloodvolume_ml(pulse->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL));
                hemorrhageData->set_initialheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
                hemorrhageData->set_initialcardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
                hemorrhageData->set_initialmeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
                hemorrhageData->set_initialhemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
                hState = eHemorrhageState::TrackTriage;
              }
            }
            break;
          }
          case eHemorrhageState::TrackTriage:
          {
            if (time_s > patientState.hemorrhage().triagetime_s())
            {
              hemorrhageData->set_triageheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
              hemorrhageData->set_triagecardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
              hemorrhageData->set_triagemeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
              hemorrhageData->set_triagebloodloss_ml(pulse->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL));
              hemorrhageData->set_triagehemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
              hState = eHemorrhageState::Finish;
            }
            break;
          }
          }

          if (pulse->GetEventManager().IsEventActive(eEvent::CardiovascularCollapse))
          {
            Info("[" + patientName + "] " + std::to_string(patientState.id()) + " Has Cardiovascular Collapse " + patientState.outputbasefilename());
            break;
          }
        }

      }
      patientState.set_finalsimulationtime_s(pulse->GetSimulationTime(TimeUnit::s));
      // Hemorrhage Mode //
      if (hemorrhageData)
      {
        hemorrhageData->set_finalheartrate_bpm(pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min));
        hemorrhageData->set_finalcardiacoutput_l_per_min(pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::L_Per_min));
        hemorrhageData->set_finalmeanarterialpressure_mmhg(pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg));
        hemorrhageData->set_finalbloodloss_ml(pulse->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL));
        hemorrhageData->set_finalhemorrhageflow_ml_per_s(pulse->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s));
      }

      pulse->GetEngineTracker()->ResetFile();// So we can close it

      if (patientState.has_validation())
      {
        CSV::SplitCSV(csvFilename, allScenarioRequests);
        if (!DeleteFile(csvFilename, 5))
          Warning("[" + patientName + "] Unable to remove file " + csvFilename);
      }
    }// end if(!m_PostProcessOnly)

    if (patientState.has_validation())
    {
      // Run patient and system validation
      std::string command = "cmake -DTYPE:STRING=ValidateFolder -DARG1:STRING=" + m_RootDir + patientState.outputbasefilename() + " -DARG2:STRING=false -P run.cmake";
      BlockSystemCall(command);// Results analysis need to be single threaded or it will thrash memory

      //Retrieve all validation json files for patient and serialize data from those files
      std::vector<std::string> validation_files;
      ListFiles(m_RootDir + patientState.outputbasefilename(), validation_files, true, "ValidationResults.json");
      if (!AggregateResults(patientState, validation_files, GetLogger()))
      {
        Error("[" + patientName + "] Unable to aggregate results");
        return false;
      }
    }

    CompletePatient(patientState);
    profiler.Clear();
    return true;
  }

  pulse::study::bind::patient_variability::PatientStateData* PVRunner::GetNextPatient()
  {
    m_VectorMutex.lock();
    pulse::study::bind::patient_variability::PatientStateData* patient = nullptr;
    if (!m_PatientsToRun.empty())
    {
      auto id = *m_PatientsToRun.begin();
      for (int i = 0; i < m_PatientList->patientstate_size(); i++)
      {
        patient = &(*m_PatientList->mutable_patientstate())[i];
        if (patient->id() == id)
          break;
      }
      m_PatientsToRun.erase(id);
    }
    m_VectorMutex.unlock();
    return patient;
  }

  void PVRunner::CompletePatient(pulse::study::bind::patient_variability::PatientStateData& patientState)
  {
    // Add our results to our results file
    m_VectorMutex.lock();
    auto pResult = m_PatientResultsList->add_patientstate();
    pResult->CopyFrom(patientState);
    SerializeToFile(*m_PatientResultsList, m_PatientResultsListFile);
    Info("[" + patientState.patient().name() + "] Completed Simulation " + std::to_string(m_PatientResultsList->patientstate_size()) + " of " + std::to_string(m_PatientList->patientstate_size()));
    m_VectorMutex.unlock();
  }

  bool PVRunner::SerializeToString(pulse::study::bind::patient_variability::PatientStateListData& src, std::string& output) const
  {
    google::protobuf::util::JsonPrintOptions printOpts;
    printOpts.add_whitespace = true;
    printOpts.preserve_proto_field_names = true;
    printOpts.always_print_primitive_fields = true;
    if (!google::protobuf::util::MessageToJsonString(src, &output, printOpts).ok())
    {
      Error("Unable to serialize Patient list");
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeToFile(pulse::study::bind::patient_variability::PatientStateListData& src, const std::string& filename) const
  {
    return PBUtils::SerializeToFile(src, filename, GetLogger());
  }
  bool PVRunner::SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::PatientStateListData& dst)
  {
    google::protobuf::util::JsonParseOptions parseOpts;
    auto status = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
    if (!status.ok())
    {
      PBUtils::LogError("PVRunner::SerializeFromString", status.ToString(), GetLogger());
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::PatientStateListData& dst)
  {
    return PBUtils::SerializeFromFile(filename, dst, GetLogger());
  }

  bool PVRunner::AggregateResults(pulse::study::bind::patient_variability::PatientStateData& patient, const std::vector<std::string>& validation_files, Logger* logger)
  {
    const auto& vMap = patient.mutable_validation()->mutable_validationmap();
    for (auto validation_filepath : validation_files)
    {
      std::string validation_filename;
      SplitFilename(validation_filepath, validation_filename);

      //Serialize the file contents
      pulse::cdm::bind::PropertyValidationListData vList;
      if (!PBUtils::SerializeFromFile(validation_filepath, vList, logger))
        return false;

      (*vMap)[validation_filename] = vList;
    }
    return true;
  }

  void PVRunner::BlockSystemCall(const std::string& cmd)
  {
    std::scoped_lock<std::mutex>  m(m_SystemMutex);
    Info("Processing: " + cmd);
    std::system(cmd.c_str());
  }

  bool PVRunner::RunTriage(pulse::study::bind::patient_variability::PatientStateListData& simList)
  {
    SAFE_DELETE(m_PatientList);
    SAFE_DELETE(m_PatientResultsList);
    m_PatientList = &simList;
    m_PatientResultsList = new pulse::study::bind::patient_variability::PatientStateListData();

    bool b = RunTriage();
    m_PatientList = nullptr;
    SAFE_DELETE(m_PatientResultsList);
    return b;
  }

  bool PVRunner::RunTriage()
  {
    if (m_PatientList->patientstate_size() == 0)
      return true; // Nothing to do

    TimingProfile profiler;
    profiler.Start("Total");

    // Get the ID's of Patients we need to run
    m_PatientsToRun.clear();
    for (int i = 0; i < m_PatientList->patientstate_size(); i++)
      m_PatientsToRun.insert(m_PatientList->patientstate()[i].id());

    size_t numSimsToRun = m_PatientList->patientstate_size() - m_PatientResultsList->patientstate_size();
    if (numSimsToRun == 0)
    {
      Info("All Patients are run in the results file");
      return true;
    }
    size_t processor_count = std::thread::hardware_concurrency();
    if (processor_count == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }
    // Let's not kill the computer this is running on...
    if (processor_count > 1)
      processor_count -= 1;
    // Let's not kick off more threads than we need
    if (processor_count > numSimsToRun)
      processor_count = numSimsToRun;
    Info("Starting " + std::to_string(processor_count) + " Threads to run " + std::to_string(m_PatientsToRun.size()) + " Patients");
    // Crank up our threads
    for (size_t p = 0; p < processor_count; p++)
      m_Threads.push_back(std::thread(&PVRunner::TriageControllerLoop, this));

    for (size_t p = 0; p < processor_count; p++)
      m_Threads[p].join();

    // Let's count up our stats
    size_t SetupErrors = 0;
    size_t StabilizationErrors = 0;
    size_t RuntimeErrors = 0;
    size_t FatalRuntimeErrors = 0;
    size_t ValidRuns = 0;
    Info("Total Patients: " + std::to_string(m_PatientResultsList->patientstate_size()));
    for (int i = 0; i < m_PatientResultsList->patientstate_size(); i++)
    {
      auto& patientStateData = m_PatientResultsList->patientstate()[i];
      if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FailedSetup)
        SetupErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FailedStabilization)
        StabilizationErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_RuntimeError)
        RuntimeErrors++;
      else if (patientStateData.failure() == eFailure::PatientStateData_eFailure_FatalRuntimeError)
        FatalRuntimeErrors++;
      else
        ValidRuns++;
    }
    Info("Total Valid Patients: " + std::to_string(ValidRuns));
    Info("Total SetupErrors: " + std::to_string(SetupErrors));
    Info("Total StabilizationErrors: " + std::to_string(StabilizationErrors));
    Info("Total RuntimeErrors: " + std::to_string(RuntimeErrors));
    Info("Total FatalRuntimeErrors: " + std::to_string(FatalRuntimeErrors));

    Info("It took " + std::to_string(profiler.GetElapsedTime_s("Total")) + "s to run this Patient list");
    profiler.Clear();
    return true;
  }

  void PVRunner::TriageControllerLoop()
  {
    CreatePath(m_RootDir);

    pulse::study::bind::patient_variability::PatientStateData* patient = nullptr;
    while (true)
    {
      try
      {
        patient = GetNextPatient();
        if (patient == nullptr)
          break;

        if (patient->failure() != eFailure::PatientStateData_eFailure_None)
        {
          Info("Not running invalid patient " + std::to_string(patient->id()) + " " + patient->outputbasefilename());
          CompletePatient(*patient);
        }
        else
        {
          Info("Running " + patient->outputbasefilename());
          if (!RunTriagePatient(*patient) && patient->failure() == eFailure::PatientStateData_eFailure_None)
            GetLogger()->Error("Unable to run patient " + patient->outputbasefilename());
        }
      }
      catch (CommonDataModelException& cdm_ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(cdm_ex.what());
        std::cerr << cdm_ex.what() << std::endl;
      }
      catch (std::exception& ex)
      {
        GetLogger()->Fatal("Exception caught runnning Patient " + patient->outputbasefilename());
        GetLogger()->Fatal(ex.what());
        std::cerr << ex.what() << std::endl;
      }
      catch (...)
      {
        std::cerr << "Unable to run Patient " << patient->outputbasefilename() << std::endl;
      }
    }
  }

  bool PVRunner::RunTriagePatient(pulse::study::bind::patient_variability::PatientStateData& patientState)
  {
    TimingProfile profiler;
    profiler.Start("Total");
    profiler.Start("Status");

    Logger* logger = GetLogger();
    SEScenario sce(logger);
    sce.SetDescription(patientState.description());
    std::string patientFileName = patientState.outputbasefilename();
    std::string patientName = patientState.patient().name();
    sce.SetName(patientName);

    logger->SetLogFile(patientFileName + ".log");
    std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine(eModelType::HumanAdultWholeBody, logger);

    if (!patientState.has_triage())
    {
      SEPatient& scePatient = sce.GetPatientConfiguration().GetPatient();
      PBPatient::Serialize(patientState.patient(), scePatient);
      sce.SerializeToFile(patientFileName + ".json");

      //Run stabilization and serialize initial patients
      if (!pe->InitializeEngine(sce.GetPatientConfiguration()))
      {
        pe->GetLogger()->Error("Could not load patient configuration, check the error");
      }

      std::string serializedPatientFilename = patientFileName + "InitialState.json";
      pe->SerializeToFile(serializedPatientFilename);
    }
    else
    {
      std:: string patientSerializedFileName = patientState.statefilename();
      sce.GetPatientConfiguration().SetPatientFile(patientSerializedFileName);

      SEHemorrhage hemorrhage;
      hemorrhage.SetCompartment((eHemorrhage_Compartment)patientState.triage().hemorrhageaction().compartment());
      hemorrhage.GetSeverity().SetValue(patientState.triage().hemorrhageaction().severity().scalar0to1().value());

      SEAirwayObstruction airwayObstruction;
      airwayObstruction.GetSeverity().SetValue(patientState.triage().airwayobstructionaction().severity().scalar0to1().value());

      SETensionPneumothorax pneumothorax;
      pneumothorax.SetType((eGate)patientState.triage().tensionpneumothoraxaction().type());
      pneumothorax.SetSide((eSide)patientState.triage().tensionpneumothoraxaction().side());
      pneumothorax.GetSeverity().SetValue(patientState.triage().tensionpneumothoraxaction().severity().scalar0to1().value());

      sce.AddAction(hemorrhage);
      sce.AddAction(airwayObstruction);
      sce.AddAction(pneumothorax);

      SEAdvanceTime adv;
      adv.GetTime().SetValue(1.0, TimeUnit::min);
      sce.AddAction(adv);

      if (!pe->SerializeFromFile(patientSerializedFileName))
      {
        pe->GetLogger()->Error("Could not load state, check the error");
        return 0;
      }

      //Data requests
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalOxygenPressure", PressureUnit::mmHg);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
      pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit::C);

      sce.GetDataRequestManager().Copy(pe->GetEngineTracker()->GetDataRequestManager());

      pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(patientFileName + "Results.csv");

      sce.SerializeToFile(patientFileName + ".json");

      const SEAdvanceTime* advRun;
      // Now run the scenario actions
      for (const SEAction* a : sce.GetActions())
      {
        // We want the tracker to process an advance time action so it will write each time step of data to our track file
        advRun = dynamic_cast<const SEAdvanceTime*>(a);
        if (advRun != nullptr)
          AdvanceAndTrackTime_s(advRun->GetTime(TimeUnit::s), *pe);// you could just do pe->AdvanceModelTime without tracking timesteps
        else
          pe->ProcessAction(*a);
      }

      std::string serializedInsultsFilename = patientFileName + "EndState.json";
      pe->SerializeToFile(serializedInsultsFilename);
    }

    sce.Clear();
    profiler.Clear();
    return true;
  }

  bool PVRunner::AdvanceAndTrackTime_s(double time_s, PhysiologyEngine& engine)
  {
    double dT_s = engine.GetTimeStep(TimeUnit::s);
    int count = static_cast<int>(time_s / dT_s);
    for (int i = 0; i < count; i++)
    {
      if (!engine.AdvanceModelTime())  // Compute 1 time step
        return false;

      // Pull Track will pull data from the engine and append it to the file
      engine.GetEngineTracker()->TrackData(engine.GetSimulationTime(TimeUnit::s));
    }
    return true;
  }
}
