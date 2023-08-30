/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/CommonDefs.h"
#include "PatientIteration.h"
#include "TCCCIteration.h"
#include "ValidationIteration.h"

#include "cdm/utils/FileUtils.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  bool clear                   = true;
  bool generateOnly            = true;

  std::vector<PatientIteration*> iPatients;
  std::vector<ScenarioIteration*> iScenarios;
  

  // Process arguments
  std::string mode="test";
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
  std::string rootDir = "./test_results/patient_variability/"+mode+"/";
  if (clear)// Then we can delete the specific solo run results
    DeleteDirectory(rootDir);
  CreatePath(rootDir);

  Logger logger;
  logger.LogToConsole(true);
  logger.SetLogFile(rootDir + "PatientVariability.log");

  if (mode == "validation" || mode == "hemorrhage" || mode == "itm")
  {
    PatientIteration* male = new PatientIteration(logger);
    male->SetGenStyle(eGenStyle::Combo);
    male->SetSex(ePatient_Sex::Male);
    male->GetAge_yr().SetValues({minAge_yr, maxAge_yr, stdAge_yr }, 2);
    male->GetHR_bpm().SetValues({minHR_bpm, maxHR_bpm, stdHR_bpm }, 2);
    male->GetMAP_mmHg().SetValues({ minMAP_mmHg, maxMAP_mmHg, stdMAP_mmHg }, 2);
    male->GetPP_mmHg().SetValues({ minPulsePressure_mmHg, maxPulsePressure_mmHg, stdPulsePressure_mmHg }, 2);
    male->GetRR_bpm().SetValues({ minRR_bpm, maxRR_bpm, stdRR_bpm }, 2);
    male->GetHeight_cm().SetValues({ minMaleHeight_cm, maxMaleHeight_cm, stdMaleHeight_cm }, 2);
    male->GetBMI().SetValues({ minBMI, maxBMI, stdMaleBMI }, 2);
    male->GetBFF().SetValues({ minMaleBFF, maxMaleBFF, stdMaleBFF }, 2);
    iPatients.push_back(male);

    PatientIteration* female = new PatientIteration(logger);
    female->SetGenStyle(eGenStyle::Combo);
    female->SetSex(ePatient_Sex::Female);
    female->GetAge_yr().SetValues({ minAge_yr, maxAge_yr, stdAge_yr }, 2);
    female->GetHR_bpm().SetValues({ minHR_bpm, maxHR_bpm, stdHR_bpm }, 2);
    female->GetMAP_mmHg().SetValues({ minMAP_mmHg, maxMAP_mmHg, stdMAP_mmHg }, 2);
    female->GetPP_mmHg().SetValues({ minPulsePressure_mmHg, maxPulsePressure_mmHg, stdPulsePressure_mmHg }, 2);
    female->GetRR_bpm().SetValues({ minRR_bpm, maxRR_bpm, stdRR_bpm }, 2);
    female->GetHeight_cm().SetValues({ minFemaleHeight_cm, maxFemaleHeight_cm, stdFemaleHeight_cm }, 2);
    female->GetBMI().SetValues({ minBMI, maxBMI, stdFemaleBMI }, 2);
    female->GetBFF().SetValues({ minFemaleBFF, maxFemaleBFF, stdFemaleBFF }, 2);
    iPatients.push_back(female);

    if (mode == "validation")
    {
      ValidationIteration* vi = new ValidationIteration(logger);
      vi->SetMaxSimTime_min(2);
      iScenarios.push_back(vi);
    }
    else if (mode == "hemorrhage")
    {
      TCCCIteration* tccc = new TCCCIteration(logger);
      tccc->SetBaselineDuration_s(15);
      tccc->SetMaxSimTime_min(60);
      tccc->PerformInterventions(false);
      tccc->GetHemorrhageSeverity().SetMinMaxStep(0, 0.9, 0.1);
      iScenarios.push_back(tccc);
    }
    else if (mode == "itm")
    {
      TCCCIteration* tccc = new TCCCIteration(logger);
      tccc->SetBaselineDuration_s(15);
      tccc->SetMaxSimTime_min(60);
      tccc->PerformInterventions(true);
      tccc->GetInsultDuration_s().SetMinMaxStep(5*60, 40*60, 5*60);
      tccc->GetAirwayObstructionSeverity().SetMinMaxStep(0, 0.9, 0.3);
      tccc->GetHemorrhageSeverity().SetMinMaxStep(0, 0.9, 0.3);
      tccc->GetTensionPneumothoraxSeverity().SetMinMaxStep(0, 0.9, 0.3);
      // What is our equipment variability?
      // Let's assume we have everything in our bag
      tccc->GetSalineAvailable().SetValues({ 1 });
      tccc->GetNeedleAvailable().SetValues({ 1 });
      tccc->GetChestWrapAvailable().SetValues({ 1 });
      iScenarios.push_back(tccc);
    }
  }
  else
  {
    PatientIteration* male = new PatientIteration(logger);
    iPatients.push_back(male);
    TCCCIteration* tccc = new TCCCIteration(logger);
    tccc->CreateStates(true);
    tccc->SetBaselineDuration_s(15);
    tccc->SetMaxSimTime_min(60);
    tccc->PerformInterventions(false);
    tccc->GetHemorrhageSeverity().SetValues({ 0.2, 0.5 });
    tccc->GetInsultDuration_s().SetValues({ 5 });
    iScenarios.push_back(tccc);
  }

  std::string destDir = rootDir;
  for (PatientIteration* pi : iPatients)
  {
    pi->GenerateScenarios(destDir);
    for (ScenarioIteration* ssi : iScenarios)
    {
      ssi->GenerateScenarios(*pi, destDir);
    }
  }

  if (generateOnly)
    return 0;
/*
  PulseScenarioExec opts(&logger);
  opts.SetModelType(eModelType::HumanAdultWholeBody);
  opts.LogToConsole(eSwitch::Off);
  // Run the patient scenarios
  opts.SetScenarioDirectory(sg.GetPatientScenarioDirectory());
  opts.SetOutputRootDirectory(sg.GetPatientScenarioDirectory() + "/results/");
  if (!opts.Execute())
  {
    logger.Error("Unable to run patient scenarios");
    return 1;
  }
  // Run the action scenarios
  for (std::string actionDir : sg.GetActionSetDirectories())
  {
    opts.SetScenarioDirectory(actionDir);
    opts.SetOutputRootDirectory(actionDir + "/results/");
    if (!opts.Execute())
    {
      logger.Error("Unable to run action set scenarios");
      return 1;
    }
  }
*/
  return 0;
}
