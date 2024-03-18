/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"
#include "engine/human_adult/whole_body/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/system/physiology/ImmuneModel.h"

#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"

namespace pulse { namespace human_adult_whole_body
{
  void EngineTest::MildInfectionMildProgressionSepsisTest(const std::string &sTestDirectory)
  {
    SepsisTest(0.3, 0.3, 300, sTestDirectory, "MildInfectionMildProgressionSepsis");
  }
  void EngineTest::MildInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.3, 0.6, 300, sTestDirectory, "MildInfectionModerateProgressionSepsis");
  }
  void EngineTest::MildInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.3, 0.9, 300, sTestDirectory, "MildInfectionSevereProgressionSepsis");
  }

  void EngineTest::ModerateInfectionMildProgressionSepsisTest(const std::string &sTestDirectory)
  {
    SepsisTest(0.6, 0.3, 300, sTestDirectory, "ModerateInfectionMildProgressionSepsis");
  }
  void EngineTest::ModerateInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.6, 0.6, 300, sTestDirectory, "ModerateInfectionModerateProgressionSepsis");
  }
  void EngineTest::ModerateInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.6, 0.9, 300, sTestDirectory, "ModerateInfectionSevereProgressionSepsis");
  }

  void EngineTest::SevereInfectionMildProgressionSepsisTest(const std::string &sTestDirectory)
  {
    SepsisTest(0.9, 0.3, 300, sTestDirectory, "SevereInfectionMildProgressionSepsis");
  }
  void EngineTest::SevereInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.9, 0.6, 300, sTestDirectory, "SevereInfectionModerateProgressionSepsis");
  }
  void EngineTest::SevereInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.9, 0.9, 300, sTestDirectory, "SevereInfectionSevereProgressionSepsis");
  }

  void EngineTest::AsepticTest(const std::string &sTestDirectory)
  {
    SepsisTest(1.0, 0.4875, 300, sTestDirectory, "Aseptic");
  }

  void EngineTest::AsepticToMaximumTest(const std::string& sTestDirectory)
  {
    SepsisTest(1.0, 0.4875, 300, sTestDirectory, "AsepticToMaximum", 1.0);
  }

  void EngineTest::MaximumSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(1.0, 1.0, 300, sTestDirectory, "MaximumSepsis");
  }

  void EngineTest::SepsisTest(double infectionSeverity, double progressionSeverity, double duration_hr,
    const std::string& sTestDirectory, const std::string& sTestName, double halfwayProgressionSeverity)
  {
    std::string tName = sTestName;

    DataTrack outTrk;
    std::ofstream file;

    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    pc.GetLogger()->SetLogFile(sTestDirectory + "/" + tName + "Test.log");
    pc.GetLogger()->Info("Running " + tName);

    ImmuneModel& im = (ImmuneModel&)pc.GetImmune();
    im.Initialize();

    double time_s = 0.0;
    double testTime_s = 60 * 60 * duration_hr;
    double timeStep_s = 60;

    for (double infSev = 0; infSev <= 1.0; infSev+=0.1)
      pc.GetLogger()->Info("Infection Severity: " + pulse::cdm::to_string(infSev)
        + " = " + pulse::cdm::to_string(im.m_SepsisModel->InfectionSeverityToPathogenCount(infSev)));
    for (double progSev = 0; progSev <= 1.0; progSev+=0.1)
      pc.GetLogger()->Info("Progression Severity: " + pulse::cdm::to_string(progSev)
        + " = " + pulse::cdm::to_string(im.m_SepsisModel->ProgressionSeverityToPathogenGrowthRate(progSev)));

    im.m_SepsisModel->PathogenCount = im.m_SepsisModel->InfectionSeverityToPathogenCount(infectionSeverity);
    im.m_SepsisModel->ActivatedPhagocytes = 0;
    im.m_SepsisModel->TissueDamage = 0;
    im.m_SepsisModel->AntiInflammatoryMediators = 0.125;
    im.m_SepsisModel->PathogenGrowthRate = im.m_SepsisModel->ProgressionSeverityToPathogenGrowthRate(progressionSeverity);
    unsigned int halfway = (testTime_s / timeStep_s) * 0.5;
    for (unsigned int i = 0; i < (testTime_s / timeStep_s); i++)
    {
      outTrk.Track("PathogenCount", time_s, im.m_SepsisModel->PathogenCount);
      outTrk.Track("ActivatedPhagocytes", time_s, im.m_SepsisModel->ActivatedPhagocytes);
      outTrk.Track("TissueDamage", time_s, im.m_SepsisModel->TissueDamage);
      outTrk.Track("AntiInflammatoryMediators", time_s, im.m_SepsisModel->AntiInflammatoryMediators);

      if (i == halfway && halfwayProgressionSeverity >= 0)
      {
        double newPathogenGrowthRate = im.m_SepsisModel->ProgressionSeverityToPathogenGrowthRate(halfwayProgressionSeverity);
        pc.GetLogger()->Info("Changing the progression severity from " + pulse::cdm::to_string(progressionSeverity) + " to " + pulse::cdm::to_string(halfwayProgressionSeverity));
        pc.GetLogger()->Info("Changing the PathogenGrowthRate from " + pulse::cdm::to_string(im.m_SepsisModel->PathogenGrowthRate) + " to " + pulse::cdm::to_string(newPathogenGrowthRate));
        im.m_SepsisModel->PathogenGrowthRate = newPathogenGrowthRate;
      }

      // TODO Change/Test time step to 0.02s
      im.m_SepsisModel->AdvanceModelTime(timeStep_s);
      
      if (i == 0)
      {
        outTrk.CreateFile(std::string(sTestDirectory + "/" + tName + "Test.csv").c_str(), file);
      }
      time_s += timeStep_s;
    }
    outTrk.StreamTrackToFile(file);
  }
END_NAMESPACE_EX
