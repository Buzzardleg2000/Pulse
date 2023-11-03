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
    SepsisTest(0.3, 0.3, 24, sTestDirectory, "MildInfectionMildProgressionSepsis");
  }
  void EngineTest::MildInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.3, 0.6, 24, sTestDirectory, "MildInfectionModerateProgressionSepsis");
  }
  void EngineTest::MildInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.3, 0.9, 24, sTestDirectory, "MildInfectionSevereProgressionSepsis");
  }

  void EngineTest::ModerateInfectionMildProgressionSepsisTest(const std::string &sTestDirectory)
  {
    SepsisTest(0.6, 0.3, 24, sTestDirectory, "ModerateInfectionMildProgressionSepsis");
  }
  void EngineTest::ModerateInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.6, 0.6, 24, sTestDirectory, "ModerateInfectionModerateProgressionSepsis");
  }
  void EngineTest::ModerateInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.6, 0.9, 24, sTestDirectory, "ModerateInfectionSevereProgressionSepsis");
  }

  void EngineTest::SevereInfectionMildProgressionSepsisTest(const std::string &sTestDirectory)
  {
    SepsisTest(0.9, 0.3, 24, sTestDirectory, "SevereInfectionMildProgressionSepsis");
  }
  void EngineTest::SevereInfectionModerateProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.9, 0.6, 24, sTestDirectory, "SevereInfectionModerateProgressionSepsis");
  }
  void EngineTest::SevereInfectionSevereProgressionSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(0.9, 0.9, 24, sTestDirectory, "SevereInfectionSevereProgressionSepsis");
  }

  void EngineTest::AsepticTest(const std::string &sTestDirectory)
  {
    SepsisTest(1, 0.3, 24, sTestDirectory, "Aseptic");
  }

  void EngineTest::MaximumSepsisTest(const std::string& sTestDirectory)
  {
    SepsisTest(1.0, 1.0, 24, sTestDirectory, "MaximumSepsis");
  }

  void EngineTest::SepsisTest(double infectionSeverity, double progressionSeverity, double duration_hr, const std::string& sTestDirectory, const std::string& sTestName)
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
    double timeStep_s = 600;

    im.m_PathogenCount = infectionSeverity;
    im.m_ActivatedPhagocytes = 0;
    im.m_TissueDamage = 0;
    im.m_AntiInflammatoryMediators = 0.125;
    im.m_PathogenGrowthRate = progressionSeverity;
    for (unsigned int i = 0; i < (testTime_s / timeStep_s); i++)
    {
      outTrk.Track("PathogenCount", time_s, im.m_PathogenCount);
      outTrk.Track("ActivatedPhagocytes", time_s, im.m_ActivatedPhagocytes);
      outTrk.Track("TissueDamage", time_s, im.m_TissueDamage);
      outTrk.Track("AntiInflammatoryMediators", time_s, im.m_AntiInflammatoryMediators);

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
