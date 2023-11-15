/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <cstring>

#include "engine/CommonDefs.h"
#include "engine/PulseScenarioExec.h"

#include "cdm/utils/FileUtils.h"

#include "PatientIteration.h"
#include "TCCCIteration.h"
#include "ValidationIteration.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  bool clear                   = false;// TODO
  bool generateOnly            = false;

  std::vector<PatientIteration*> iPatients;
  std::vector<ActionIteration*>  iActions;

  std::string mode="test";
  // Process arguments
  if (argc > 1)
  {
    mode = argv[1];
    for (int i = 2; i < argc; ++i)
    {
      // Clear
      if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--clear"))
        clear = true;

      // Only generate
      if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--generate"))
        generateOnly = true;
    }
  }
  std::string rootDir = "./test_results/patient_variability/"+mode;
  if (clear)// Then we can delete the specific solo run results
    DeleteDirectory(rootDir);
  CreatePath(rootDir);

  Logger logger;
  logger.LogToConsole(true);
  logger.SetLogFile(rootDir + "/PatientVariability.log");

  if (mode == "validation" || mode == "hemorrhage" || mode == "itm")
  {
    PatientIteration* male = new PatientIteration(logger);
    male->SetIterationName("male");
    male->SetScenarioExecListFilename(rootDir + "/scenarios/" + male->GetIterationName() + ".json");
    male->SetStateDirectory(rootDir + "/states/" + male->GetIterationName());
    male->SetResultsDirectory(rootDir + "/results/" + male->GetIterationName());
    male->SetGenStyle(eGenStyle::Combo);
    male->SetSex(ePatient_Sex::Male);
    //male->GetAge_yr().SetValues({minAge_yr, maxAge_yr, stdAge_yr }, 2);
    //male->GetHR_bpm().SetValues({minHR_bpm, maxHR_bpm, stdHR_bpm }, 2);
    //male->GetMAP_mmHg().SetValues({ minMAP_mmHg, maxMAP_mmHg, stdMAP_mmHg }, 2);
    //male->GetPP_mmHg().SetValues({ minPulsePressure_mmHg, maxPulsePressure_mmHg, stdPulsePressure_mmHg }, 2);
    //male->GetRR_bpm().SetValues({ minRR_bpm, maxRR_bpm, stdRR_bpm }, 2);
    //male->GetHeight_cm().SetValues({ minMaleHeight_cm, maxMaleHeight_cm, stdMaleHeight_cm }, 2);
    //male->GetBMI().SetValues({ minBMI, maxBMI, stdMaleBMI }, 2);
    //male->GetBFF().SetValues({ minMaleBFF, maxMaleBFF, stdMaleBFF }, 2);
    iPatients.push_back(male);

    PatientIteration* female = new PatientIteration(logger);
    female->SetIterationName("female");
    female->SetScenarioExecListFilename(rootDir + "/scenarios/" + female->GetIterationName() + ".json");
    female->SetStateDirectory(rootDir + "/states/" + female->GetIterationName());
    female->SetResultsDirectory(rootDir + "/results/" + female->GetIterationName());
    female->SetGenStyle(eGenStyle::Combo);
    female->SetSex(ePatient_Sex::Female);
    //female->GetAge_yr().SetValues({ minAge_yr, maxAge_yr, stdAge_yr }, 2);
    //female->GetHR_bpm().SetValues({ minHR_bpm, maxHR_bpm, stdHR_bpm }, 2);
    //female->GetMAP_mmHg().SetValues({ minMAP_mmHg, maxMAP_mmHg, stdMAP_mmHg }, 2);
    //female->GetPP_mmHg().SetValues({ minPulsePressure_mmHg, maxPulsePressure_mmHg, stdPulsePressure_mmHg }, 2);
    //female->GetRR_bpm().SetValues({ minRR_bpm, maxRR_bpm, stdRR_bpm }, 2);
    //female->GetHeight_cm().SetValues({ minFemaleHeight_cm, maxFemaleHeight_cm, stdFemaleHeight_cm }, 2);
    //female->GetBMI().SetValues({ minBMI, maxBMI, stdFemaleBMI }, 2);
    //female->GetBFF().SetValues({ minFemaleBFF, maxFemaleBFF, stdFemaleBFF }, 2);
    iPatients.push_back(female);

    if (mode == "validation")
    {
      ValidationIteration* vi = new ValidationIteration(logger);
      vi->SetMaxSimTime_min(2);
      iActions.push_back(vi);
    }
    else if (mode == "hemorrhage")
    {
      TCCCIteration* tccc = new TCCCIteration(logger);
      tccc->SetBaselineDuration_s(15);
      tccc->SetMaxSimTime_min(60);
      tccc->PerformInterventions(false);
      tccc->GetHemorrhageSeverity().SetValues(ParameterIteration<double>::SetMinMaxStep(0.0, 0.9, 0.1));
      std::vector<size_t> hemorrhageWounds;
      for (size_t i = 0; i < (size_t)eHemorrhageWound::_LOC_COUNT; ++i)
        hemorrhageWounds.push_back(i);
      iActions.push_back(tccc);
    }
    else if (mode == "itm")
    {
      TCCCIteration* tccc = new TCCCIteration(logger);
      tccc->SetIterationName("tccc");
      tccc->SetBaselineDuration_s(15);
      tccc->SetMaxSimTime_min(60);
      tccc->PerformInterventions(true);
      tccc->GetAirwayObstructionSeverity().SetValues(ParameterIteration<double>::SetMinMaxStep(0., 0.9, 0.3));
      tccc->GetHemorrhageSeverity().SetValues(ParameterIteration<double>::SetMinMaxStep(0., 0.9, 0.3));
      std::vector<size_t> hemorrhageWounds;
      //for(size_t i = 0; i < (size_t)eHemorrhageWound::_LOC_COUNT; ++i)
      //  hemorrhageWounds.push_back(i);
      hemorrhageWounds.push_back((size_t)eHemorrhageWound::LeftLegLaceration);
      tccc->GetHemorrhageWound().SetValues(hemorrhageWounds);
      tccc->GetTensionPneumothoraxSeverity().SetValues(ParameterIteration<double>::SetMinMaxStep(0., 0.9, 0.3));
      // What is our equipment variability?
      // Let's assume we have everything in our bag
      //tccc->GetInsultDuration_s().SetValues(ParameterIteration<double>::SetMinMaxStep(5. * 60, 40. * 60, 5. * 60));
      //tccc->GetSalineAvailable().SetValues({ 1 });
      //tccc->GetNeedleAvailable().SetValues({ 1 });
      //tccc->GetChestWrapAvailable().SetValues({ 1 });
      iActions.push_back(tccc);
    }
  }
  else
  {
    PatientIteration* male = new PatientIteration(logger);
    male->SetIterationName("default_male");
    male->SetScenarioExecListFilename(rootDir + "/scenarios/" + male->GetIterationName()+".json");
    male->SetStateDirectory(rootDir + "/states/" + male->GetIterationName());
    male->SetResultsDirectory(rootDir + "/results/" + male->GetIterationName());
    iPatients.push_back(male);

    PatientIteration* female = new PatientIteration(logger);
    female->SetIterationName("default_female");
    female->SetGenStyle(eGenStyle::Combo);
    female->SetSex(ePatient_Sex::Female);
    female->SetScenarioExecListFilename(rootDir + "/scenarios/" + female->GetIterationName() + ".json");
    female->SetStateDirectory(rootDir + "/states/" + female->GetIterationName());
    female->SetResultsDirectory(rootDir + "/results/" + female->GetIterationName());
    iPatients.push_back(female);

    if (false)
    {
      ValidationIteration* vi = new ValidationIteration(logger);
      vi->SetIterationName("validation");
      iActions.push_back(vi);
    }
    if (true)
    {
      TCCCIteration* tccc = new TCCCIteration(logger);
      tccc->SetIterationName("tccc");
      tccc->CreateStates(true);
      tccc->SetBaselineDuration_s(15);
      tccc->SetMaxSimTime_min(60);
      tccc->PerformInterventions(false);
      tccc->GetTBISeverity().SetValues({ 0.2, 0.5 });
      tccc->GetHemorrhageSeverity().SetValues({ 0.2, 0.5 });
      std::vector<size_t> hemorrhageWounds;
      //for(size_t i = 0; i < (size_t)eHemorrhageWound::_LOC_COUNT; ++i)
      //  hemorrhageWounds.push_back(i);
      hemorrhageWounds.push_back((size_t)eHemorrhageWound::LeftLegLaceration);
      tccc->GetHemorrhageWound().SetValues(hemorrhageWounds);
      tccc->GetInsultDuration_s().SetValues({ 5 });
      iActions.push_back(tccc);
    }
  }

  for (PatientIteration* pi : iPatients)
  {
    if (!clear && FileExists(pi->GetScenarioExecListFilename()))
      logger.Info("Using previously run scenario exec list file: " + pi->GetScenarioExecListFilename());
    else
      pi->GenerateScenarios();

    // Note, not setting the output directory on the opts
    // Our scenarios aleady have a relative (to the working dir) results csv location set,
    // If you provide an output dir, that relative location will be concatenated to the csv relative location

    if (!generateOnly)
    {
      PulseScenarioExec opts(&logger);
      opts.SetModelType(eModelType::HumanAdultWholeBody);
      opts.LogToConsole(eSwitch::Off);
      opts.SetScenarioExecListFilename(pi->GetScenarioExecListFilename());
      opts.Execute();
    }
  }

  for (ActionIteration* ai : iActions)
  {
    ai->SetScenarioExecListFilename(rootDir + "/scenarios/" + ai->GetIterationName() + ".json");
    ai->SetStateDirectory(rootDir + "/states/" + ai->GetIterationName());
    ai->SetResultsDirectory(rootDir + "/results/" + ai->GetIterationName());

    if (!clear && FileExists(ai->GetScenarioExecListFilename()))
      logger.Info("Using previously run scenario exec list file: " + ai->GetScenarioExecListFilename());
    else
    {
      for (PatientIteration* pi : iPatients)
        ai->GenerateScenarios(*pi);
    }

    if (!generateOnly)
    {
      PulseScenarioExec opts(&logger);
      opts.SetModelType(eModelType::HumanAdultWholeBody);
      opts.LogToConsole(eSwitch::Off);
      opts.SetScenarioExecListFilename(ai->GetScenarioExecListFilename());
      opts.Execute();
    }
  }

  SAFE_DELETE_VECTOR(iPatients);
  SAFE_DELETE_VECTOR(iActions);
  return 0;
}
