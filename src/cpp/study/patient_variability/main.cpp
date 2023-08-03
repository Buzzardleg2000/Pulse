/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "cdm/properties/SEScalarLength.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/io/protobuf/PBPatient.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  bool clear                   = false;
  bool binary                  = true;
  bool generateOnly            = false;
  bool postProcessOnly         = false;
  bool validationMode          = false;
  bool hemorrhageMode          = false;
  bool triageMode              = true;

  if (triageMode)
  {
    bool usePatientStates = false;

    std::string rootDir = "./test_results/synthetic_data/";

    Logger logger;
    PVGenerator pvg(&logger);

    pulse::study::bind::patient_variability::PatientStateListData patients;
    pvg.GenerateTriagePatientData(rootDir, patients);

    eStandardValidationType vType = eStandardValidationType::None;
    PVRunner pvr(rootDir, vType, &logger);
    if (!usePatientStates)
    {
      if (!pvr.RunTriage(patients))
      {
        ///\TODO: Throw error
      }
    }

    pulse::study::bind::patient_variability::PatientStateListData insultPatients;
    pvg.GenerateTriageInsultData(rootDir, patients, insultPatients);

    if (!pvr.RunTriage(insultPatients))
    {
      ///\TODO: Throw error
    }

    return 0;
  }

  eStandardValidationType vType = eStandardValidationType::None;
  std::string data = "full";
  eMode mode = eMode::Validation;
  std::string rootDir = "./test_results/patient_variability/";

  // Process arguments
  for(int i = 1; i < argc; ++i)
  {
    // data
    if(!strcmp(argv[i], "full") || !strcmp(argv[i], "solo") || !strcmp(argv[i], "test"))
    {
      data = argv[i];
    }

    // Binary output
    if(!strcmp(argv[i], "-b") || !strcmp(argv[i], "--binary"))
    {
      binary = true;
    }

    // Clear
    if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--clear"))
    {
      clear = true;
    }

    // Only generate
    if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--generate"))
    {
      generateOnly = true;
    }

    // Only post-processing
    if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--post-process"))
    {
      postProcessOnly = true;
      clear = false;
    }

    // Validation RunMode
    if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--validation"))
    {
      validationMode = true;
      mode = eMode::Validation;
    }
    // Hemorrhage RunMode
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--hemorrhage"))
    {
      hemorrhageMode = true;
      mode = eMode::Hemorrhage;
    }

    // Generate standard validation from baseline results for comparison
    if(!strcmp(argv[i], "-b") || !strcmp(argv[i], "--baseline"))
    {
      vType = eStandardValidationType::Baseline;
    }
    // Generate standard validation from current code base
    if (!strcmp(argv[i], "-rv") || !strcmp(argv[i], "--run_validation"))
    {
      vType = eStandardValidationType::Current;
    }

  }
  if (validationMode && hemorrhageMode)
  {
    std::cerr << "Can only select one run mode\n";
    return 1;
  }

  std::string modeDir = "";
  switch (mode)
  {
  case eMode::Validation:
    modeDir = "validation/";
    break;
  case eMode::Hemorrhage:
    modeDir = "hemorrhage/";
    break;
  }

  Logger log;
  log.LogToConsole(true);
  std::string logName = "PatientVariability.log";

  pulse::study::bind::patient_variability::PatientStateListData patients;
  PVGenerator pvg(&log);
  pvg.GenerateMode = mode;

  if (data == "solo")
  {
    rootDir += modeDir+"solo/";
    // I always want to run, so remove our "compiled" results of 1
    DeleteFile(rootDir+"patient_results.json");// If we are solo, always rerun
    if(clear)// Then we can delete the specific solo run results
      DeleteDirectory(rootDir);
    log.SetLogFile(rootDir + logName);

    /// male/age_yr18/height_cm163.000000/bmi16/hr_bpm100.000000/map_mmHg70.000000/pp_mmHg40.500000/bp_mmHg114.000000-73.500000
    uint32_t age_yr = 18;
    double   height_cm = 163;
    double   bmi    = 16;
    double   hr_bpm = 100;
    double   map_mmHg = 70;
    double   pp_mmHg = 40.5;
    double systolic_mmHg = 114;// Set to -1 if you want it computed
    double diastolic_mmHg = 73.5;// Set to -1 if you want it computed

    SEPatient patient(&log);
    patient.SetSex(ePatient_Sex::Male);
    patient.GetAge().SetValue(age_yr, TimeUnit::yr);

    //patientData->set_bmi(bmi);
    patient.GetHeight().SetValue(height_cm, LengthUnit::cm);
    patient.GetHeartRateBaseline().SetValue(hr_bpm, FrequencyUnit::Per_min);
    //patientData->set_meanarterialpressure_mmhg(map_mmHg);
    //patientData->set_pulsepressure_mmhg(pp_mmHg);
    patient.GetSystolicArterialPressureBaseline().SetValue(diastolic_mmHg, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(systolic_mmHg, PressureUnit::mmHg);

    auto p = patients.add_patientstate();
    p->set_id(0);


    p->set_outputbasefilename("solo/");
    p->set_maxsimulationtime_s(120); // Generate 2 mins of data
    p->mutable_validation();// Create a validation object to fill
  }
  else if(data == "full")
  {
    rootDir += modeDir+"full/";
    if(clear)
      DeleteDirectory(rootDir);
    log.SetLogFile(rootDir + logName);

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    // Defaults are
    // HemorrhageSeverity(0.25,1.00,0.25)
    // HemorrhageTriageTime(1.0,5.0,1.0)

    pvg.GenerateData(eSetType::Both, patients);
  }

  if (generateOnly)
  {
    std::string patientFile = rootDir + "/patients.json";
    if (binary)
      patientFile = rootDir + "/patients.pbb";
    PBUtils::SerializeToFile(patients, patientFile, &log);
    return 0;
  }

  PVRunner pvr(rootDir, vType, &log);
  pvr.PostProcessOnly = postProcessOnly;
  pvr.SerializationFormat = binary ? eSerializationFormat::BINARY : eSerializationFormat::JSON;
  return !pvr.Run(patients);
}
