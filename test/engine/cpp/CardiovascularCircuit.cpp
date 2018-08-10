/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"
#include "Controller/Controller.h"
#include "Controller/Substances.h"
#include "Controller/Circuits.h"
#include "Controller/Compartments.h"
#include "Systems/Cardiovascular.h"
#include "Systems/Saturation.h"
#include "PulseConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceFraction.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarFlowResistance.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "utils/DataTrack.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "bind/cdm/Patient.pb.h"

enum Driver { Sinusoid = 0, Heart };

// We use 1,1,1,0 to run our test without any scaling of the circuit and using the HeartRate Baseline in the standard patient file
void PulseEngineTest::CardiovascularCircuitAndTransportTest(const std::string& sTestDirectory)
{
  CardiovascularCircuitAndTransportTest(Heart, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, false, false, false, sTestDirectory, "Cardiovascular", false);
}

void PulseEngineTest::CardiovascularAndRenalCircuitAndTransportTest(const std::string& sTestDirectory)
{
  CardiovascularCircuitAndTransportTest(Heart, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, false, true, false, sTestDirectory, "CardiovascularAndRenal", false);
}

void PulseEngineTest::CardiovascularAndTissueCircuitAndTransportTest(const std::string& sTestDirectory)
{
  CardiovascularCircuitAndTransportTest(Heart, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, true, false, false, sTestDirectory, "CardiovascularAndTissue", false);
}

void PulseEngineTest::CardiovascularTissueAndRenalCircuitAndTransportTest(const std::string& sTestDirectory)
{
  CardiovascularCircuitAndTransportTest(Heart, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, true, true, false, sTestDirectory, "CardiovascularTissueAndRenal", false);
}

void PulseEngineTest::CardiovascularBloodGasesTest(const std::string& sTestDirectory)
{
  CardiovascularCircuitAndTransportTest(Heart, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, true, true, true, sTestDirectory, "CardiovascularBloodGasesTest", false);
}

void PulseEngineTest::TuneCardiovascularCircuitTest(const std::string& sTestDirectory)
{
  m_Logger->ResetLogFile(sTestDirectory + "/TuneCardiovascularCircuit.log");

  SETestReport testReport = SETestReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName("TuneCircuit");

  SEPatient patient(nullptr);
  patient.SetName("TuneTest");
  patient.SetSex(cdm::ePatient_Sex_Male);

  double HRLower = 60;
  double HRUpper = 100;
  double DiaLower = 60;
  double DiaUpper = 80;
  double SysLower = 90;
  double SysUpper = 120;

  //Only test the worst cases

  for (unsigned int HRIter = 0; HRIter < 2; HRIter++)
  {
    double HR = HRLower;
    if (HRIter == 1) HR = HRUpper;
    for (unsigned int DiaIter = 0; DiaIter < 2; DiaIter++)
    {
      double Dia = DiaLower;
      if (DiaIter == 1) Dia = DiaUpper;
      for (unsigned int SysIter = 0; SysIter < 2; SysIter++)
      {
        double Sys = SysLower;
        if (SysIter == 1) Sys = SysUpper;

        //Make sure the pulse pressure isn't abnormally narrow
        if (Dia > 0.75 * Sys)
        {
          if (Sys == SysUpper)
          {
            //Don't reduce Systolic because we're testing this upper limit
            //Increase Diastolic as little as we can
            Dia = 0.75 * Sys;
          }
          else
          {
            //Don't increase Diastolic because we're testing this lower limit
            //Reduce Systolic as little as we can
            Sys = 1 / 0.75 * Dia;
          }
        }        

        patient.GetHeartRateBaseline().SetValue(HR, FrequencyUnit::Per_min);
        patient.GetSystolicArterialPressureBaseline().SetValue(Sys, PressureUnit::mmHg);
        patient.GetDiastolicArterialPressureBaseline().SetValue(Dia, PressureUnit::mmHg);
        std::stringstream ss;
        ss << "HR=" << HR << ", Sys=" << Sys << ", Dia=" << Dia;
        TuneCardiovascularCircuitTest(testSuite, sTestDirectory, ss.str(), patient);
      }
    }
  }
  testReport.WriteFile(sTestDirectory + "/TuneCardiovascularCircuitReport.pba");
}
void PulseEngineTest::TuneCardiovascularCircuitTest(SETestSuite& testSuite, const std::string& sTestDirectory, const std::string& sTestName, SEPatient& patient)
{
  TimingProfile timer;
  timer.Start("TestCase");
  PulseController pc(testSuite.GetLogger());
  testSuite.GetLogger()->Info("Running " + sTestName);
  auto* p = SEPatient::Unload(patient);
  SEPatient::Load(*p, pc.GetPatient());
  delete p;  
  pc.m_Config->EnableRenal(cdm::eSwitch::On);
  pc.m_Config->EnableTissue(cdm::eSwitch::On);
  pc.SetupPatient();
  pc.CreateCircuitsAndCompartments();

  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName(sTestName);

  Cardiovascular& cv = (Cardiovascular&)pc.GetCardiovascular();
  try {
    // cv.m_TuningFile = sTestDirectory + "/Tune" + sTestName + "CircuitOutput.csv";//For Debugging
    cv.Initialize();
  }
  catch (PhysiologyEngineException ex)
  {
    testCase.AddFailure("Failed test case " + sTestName);
  }
  testCase.GetDuration().SetValue(timer.GetElapsedTime_s("TestCase"), TimeUnit::s);
}

void PulseEngineTest::CardiovascularCircuitAndTransportTest(CardiovascularDriver driverType,
  double complianceScale, double resistanceScale, double volumeScale, double heartRate_bpm,
  double systemicResistanceScale, double systemicComplianceScale, double aortaResistanceScale,
  double aortaComplianceScale, double rightHeartResistanceScale, double venaCavaComplianceScale,
  bool connectTissue, bool connectRenal, bool balanceBloodGases, const std::string& sTestDirectory, 
  const std::string& sTestName, bool breakOutResults)
{
  //breakOutResults True = seperate files for different types (i.e. volumes, flows, etc.); False = one file with everything
  double testTime_s = 120;
  std::stringstream ss;
  std::stringstream tName;
  tName << sTestName;
  bool scale = false;
  if (complianceScale != 1.0 || resistanceScale != 1.0 || volumeScale != 1.0 || heartRate_bpm > 0 ||
    systemicResistanceScale != 1.0 || systemicComplianceScale != 1.0 || aortaResistanceScale != 1.0 ||
    aortaComplianceScale != 1.0 || rightHeartResistanceScale != 1.0 || venaCavaComplianceScale != 1.0)
  {
    scale = true;
    tName << "_" << heartRate_bpm << "bpm";
    if (complianceScale != 1.0 || resistanceScale != 1.0 || volumeScale != 1.0)
    {
      tName << "_" << complianceScale << "c";
      tName << "_" << resistanceScale << "r";
      tName << "_" << volumeScale << "v";
    }
    else
    {
      tName << "_" << systemicResistanceScale << "sr";
      tName << "_" << systemicComplianceScale << "sc";
      tName << "_" << aortaResistanceScale << "ar";
      tName << "_" << aortaComplianceScale << "ac";
      tName << "_" << rightHeartResistanceScale << "rr";
      tName << "_" << venaCavaComplianceScale << "vc";
    }
  }

  TimingProfile tmr;
  tmr.Start("Test");
  double circuit_s = 0;
  double transport_s = 0;
  double binding_s = 0;
  PulseController pc(sTestDirectory + "/" + tName.str() + "CircuitAndTransportTest.log");
  pc.GetLogger()->Info("Running " + tName.str());
  pc.GetPatient().LoadFile("./patients/StandardMale.pba");
  pc.SetupPatient();
  if (heartRate_bpm <= 0)
    heartRate_bpm = pc.GetPatient().GetHeartRateBaseline().GetValue(FrequencyUnit::Per_min);
  else
  {
    pc.GetPatient().GetHeartRateBaseline().SetValue(heartRate_bpm, FrequencyUnit::Per_min);
  }

  pc.m_Config->EnableRenal(connectRenal ? cdm::eSwitch::On : cdm::eSwitch::Off);
  pc.m_Config->EnableTissue(connectTissue? cdm::eSwitch::On :cdm::eSwitch::Off);
  pc.CreateCircuitsAndCompartments();

  std::vector<SESubstance*> subs2Track;
  if (balanceBloodGases)
  {
    SEEnvironmentalConditions env(pc.GetSubstances());
    env.LoadFile("./environments/Standard.pba");
    SEGasCompartment* cEnv = pc.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
    for (SESubstanceFraction* subFrac : env.GetAmbientGases())
    {
      pc.GetSubstances().AddActiveSubstance(subFrac->GetSubstance());
      cEnv->GetSubstanceQuantity(subFrac->GetSubstance())->GetVolumeFraction().Set(subFrac->GetFractionAmount());
    }
    pc.GetSubstances().InitializeSubstances();
    subs2Track.push_back(&pc.GetSubstances().GetO2());
    subs2Track.push_back(&pc.GetSubstances().GetCO2());
    subs2Track.push_back(&pc.GetSubstances().GetHb());
    subs2Track.push_back(&pc.GetSubstances().GetHbO2());
    subs2Track.push_back(&pc.GetSubstances().GetHbCO2());
    subs2Track.push_back(&pc.GetSubstances().GetHbO2CO2());
    subs2Track.push_back(&pc.GetSubstances().GetHCO3());
  }
  else
  {
    subs2Track.push_back(&pc.GetSubstances().GetN2());
    pc.GetSubstances().AddActiveSubstance(pc.GetSubstances().GetN2());
    SEScalarMassPerVolume N2_ug_per_mL;
    N2_ug_per_mL.SetValue(0.5, MassPerVolumeUnit::ug_Per_mL);
    pc.GetSubstances().SetSubstanceConcentration(pc.GetSubstances().GetN2(), pc.GetCompartments().GetVascularLeafCompartments(), N2_ug_per_mL);
    if (connectRenal)
    {
      pc.GetSubstances().SetSubstanceConcentration(pc.GetSubstances().GetN2(), pc.GetCompartments().GetUrineLeafCompartments(), N2_ug_per_mL);
    }
  }

  Cardiovascular& cv = (Cardiovascular&)pc.GetCardiovascular();
  cv.m_TuneCircuit = true;// Run the circuit as constructed
  //cv.m_TuningFile = "./test_results/unit_tests/Pulse/"+ sTestName+"Tuning.csv";

  SEFluidCircuit& cvCircuit = pc.GetCircuits().GetActiveCardiovascularCircuit();
  
  // Make a file with all the resistances and compliances
  DataTrack     cvCompBaseTrk;
  std::ofstream cvCompBaseFile;
  DataTrack     cvResBaseTrk;
  std::ofstream cvResBaseFile;
  for (SEFluidCircuitPath* p : cvCircuit.GetPaths())
  {
    if (p->HasComplianceBaseline())
    {
      auto unit = p->GetComplianceBaseline().GetUnit();
      cvCompBaseTrk.Track(p->GetName() + "_" + unit->GetString(), 0, p->GetComplianceBaseline().GetValue(*unit));
    }

    if (p->HasResistanceBaseline())
    {
      auto unit = p->GetResistanceBaseline().GetUnit();
      cvResBaseTrk.Track(p->GetName() + "_" + unit->GetString(), 0, p->GetResistanceBaseline().GetValue(*unit));
    }
  }

  cv.Initialize();
  if (connectRenal)
    ((Renal&)pc.GetRenal());

  DataTrack     cvGraphTrk;
  std::ofstream cvGraphFile;
  DataTrack     cvGraphPresTrk;
  std::ofstream cvGraphPresFile;
  DataTrack     cvGraphVolTrk;
  std::ofstream cvGraphVolFile;
  DataTrack     cvGraphFlowTrk;
  std::ofstream cvGraphFlowFile;
  DataTrack     circiutTrk;
  std::ofstream circuitFile;
  DataTrack     cvPressureTrk;
  std::ofstream cvPressureFile;
  DataTrack     cvVolumeTrk;
  std::ofstream cvVolumeFile;
  DataTrack     cvNormVolumeTrk;
  std::ofstream cvNormVolumeFile;
  DataTrack     cvFlowTrk;
  std::ofstream cvFlowFile;
  DataTrack     cvCompFlowTrk;
  std::ofstream cvCompFlowFile;
  DataTrack     cvResFlowTrk;
  std::ofstream cvResFlowFile;
  DataTrack     cvHDTrk;
  std::ofstream cvHDFile;
  DataTrack     cvCompTrk;
  std::ofstream cvCompFile;
  DataTrack     cvResTrk;
  std::ofstream cvResFile;

  double time_s = 0.0;
  //double testTime_s = 1200;
  double timeStep_s = 1.0 / 90;
  double heartFreq_Per_s = heartRate_bpm / 60.0; // 72 beats per min * 1 min per 60 s = beats per s
  double aortaPressure;
  double venousPressure;

  SELiquidCompartmentGraph& cvGraph = pc.GetCompartments().GetActiveCardiovascularGraph();

  if (scale)
  {
    for (SEFluidCircuitPath* path : cvCircuit.GetPaths())
    {
      if (path->HasCapacitanceBaseline())
      {
        auto unit = path->GetCapacitanceBaseline().GetUnit();
        double c = path->GetCapacitanceBaseline().GetValue(*unit);
        path->GetCapacitanceBaseline().SetValue(c*complianceScale, *unit);
      }
      if (path->HasResistanceBaseline())
      {
        auto unit = path->GetResistanceBaseline().GetUnit();
        double c = path->GetResistanceBaseline().GetValue(*unit);
        path->GetResistanceBaseline().SetValue(c*resistanceScale, *unit);
      }
      
    }
    for (SEFluidCircuitNode* node : cvCircuit.GetNodes())
    {
      if (node->HasVolumeBaseline())
      {
        auto unit = node->GetVolumeBaseline().GetUnit();
        double c = node->GetVolumeBaseline().GetValue(*unit);
        node->GetVolumeBaseline().SetValue(c*volumeScale, *unit);
      }
    }
    // if (path->IsSystemic) // this would be in addition to the 'overall' scales, which in practice would probably be 1.0 if we had the systemic or targeted path scales
        // if (path->HasCapacitanceBaseline
           //auto unit = path->GetCapacitanceBaseline().GetUnit();
           //double c = path->GetCapacitanceBaseline().GetValue(*unit);
           //path->GetCapacitanceBaseline().SetValue(c*systemicComplianceScale, *unit);
    // See where I'm going with this?
    // For now, here's this method
    cv.TunePaths(systemicResistanceScale, systemicComplianceScale, aortaResistanceScale, aortaComplianceScale, rightHeartResistanceScale, venaCavaComplianceScale);
  }

  SEFluidCircuitNode* Aorta = cvCircuit.GetNode("Aorta1");
  SEFluidCircuitNode* VenaCava = cvCircuit.GetNode("VenaCava");
  SEFluidCircuitPath *RightCompliance = cvCircuit.GetPath(pulse::CardiovascularPath::RightHeart1ToRightHeart3);
  SEFluidCircuitPath *LeftCompliance = cvCircuit.GetPath(pulse::CardiovascularPath::LeftHeart1ToLeftHeart3);
  SEFluidCircuitPath *HeartLeft = cvCircuit.GetPath(pulse::CardiovascularPath::LeftHeart1ToAorta2);

  SELiquidSubstanceQuantity* venaCavaN2 = cvGraph.GetCompartment(pulse::VascularCompartment::VenaCava)->GetSubstanceQuantity(pc.GetSubstances().GetN2());
  SELiquidSubstanceQuantity* leftPulmonaryCapillariesN2 = cvGraph.GetCompartment(pulse::VascularCompartment::LeftPulmonaryCapillaries)->GetSubstanceQuantity(pc.GetSubstances().GetN2());
  SELiquidSubstanceQuantity* rightPulmonaryCapillariesN2 = cvGraph.GetCompartment(pulse::VascularCompartment::LeftPulmonaryCapillaries)->GetSubstanceQuantity(pc.GetSubstances().GetN2());

  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, pc.GetLogger());
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, pc.GetLogger());

  for (unsigned int i = 0; i < (testTime_s / timeStep_s); i++)
  {
    aortaPressure = Aorta->GetPressure(PressureUnit::mmHg);
    venousPressure = VenaCava->GetPressure(PressureUnit::mmHg);
    switch (driverType)
    {
    case Sinusoid:
      double rHeartElastance;
      double lHeartElastance;
      SinusoidHeartDriver(time_s, heartFreq_Per_s, lHeartElastance, rHeartElastance);
      RightCompliance->GetNextCompliance().SetValue(1.0 / rHeartElastance, FlowComplianceUnit::mL_Per_mmHg);
      LeftCompliance->GetNextCompliance().SetValue(1.0 / lHeartElastance, FlowComplianceUnit::mL_Per_mmHg);
      break;
    case Heart:
      cv.HeartDriver();
      break;
    }
    
    tmr.Start("Circuit");
    calc.Process(cvCircuit, timeStep_s);//Process - Execute the circuit  
    circuit_s += tmr.GetElapsedTime_s("Circuit");
    if (!scale)
    {
      tmr.Start("Transport");
      txpt.Transport(cvGraph, timeStep_s);
      transport_s += tmr.GetElapsedTime_s("Transport");
      if (balanceBloodGases)
      {
        tmr.Start("Binding");
        for (SELiquidCompartment* cmpt : pc.GetCompartments().GetVascularLeafCompartments())
        {
          if (!cmpt->HasVolume()) continue;
          pc.GetSaturationCalculator().CalculateBloodGasDistribution(*cmpt);
        }
        binding_s += tmr.GetElapsedTime_s("Binding");

        for (SELiquidCompartment* cmpt : pc.GetCompartments().GetVascularLeafCompartments())
        {
          SELiquidSubstanceQuantity* O2 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetO2());
          SELiquidSubstanceQuantity* HbO2 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetHbO2());
          SELiquidSubstanceQuantity* HbO2CO2 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetHbO2CO2());

          SELiquidSubstanceQuantity* CO2 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetCO2());
          SELiquidSubstanceQuantity* HbCO2 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetHbCO2());
          SELiquidSubstanceQuantity* HCO3 = cmpt->GetSubstanceQuantity(pc.GetSubstances().GetHCO3());
          cvGraphTrk.Track(cmpt->GetName() + "_TotalOxygenMolarConcentration_mmol_per_L", time_s, O2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
          cvGraphTrk.Track(cmpt->GetName() + "_TotalCarbonDioxideMolarConcentration_mmol_per_L", time_s, CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
        }
      }
      else
      {
        /*double N2_ug = venaCavaN2->GetMass(MassUnit::ug);
        venaCavaN2->GetMass().SetValue(N2_ug - 0.1, MassUnit::ug);
        venaCavaN2->Balance(BalanceLiquidBy::Mass);

        N2_ug = leftPulmonaryCapillariesN2->GetMass(MassUnit::ug);
        leftPulmonaryCapillariesN2->GetMass().SetValue(N2_ug + 0.05, MassUnit::ug);
        leftPulmonaryCapillariesN2->Balance(BalanceLiquidBy::Mass);
        N2_ug = rightPulmonaryCapillariesN2->GetMass(MassUnit::ug);
        rightPulmonaryCapillariesN2->GetMass().SetValue(N2_ug + 0.05, MassUnit::ug);
        rightPulmonaryCapillariesN2->Balance(BalanceLiquidBy::Mass);*/
      }
      cvGraphTrk.Track(time_s, cvGraph, &subs2Track);
    }
    circiutTrk.Track("LeftHeartCompliance_mL_Per_mmHg", time_s, LeftCompliance->GetCompliance().GetValue(FlowComplianceUnit::mL_Per_mmHg));
    circiutTrk.Track("LeftHeartElastance_mmHg_Per_mL", time_s, 1 / LeftCompliance->GetCompliance().GetValue(FlowComplianceUnit::mL_Per_mmHg));

    cv.CalculateVitalSigns();
    calc.PostProcess(cvCircuit);//convert 'Next' values to current 

    double totalVolume_mL = 0.0;
    for (SEFluidCircuitNode* n : cvCircuit.GetNodes())
    {
      if (n->HasNextVolume())
        totalVolume_mL += n->GetNextVolume(VolumeUnit::mL);
    }
    circiutTrk.Track("TotalVolume_mL", time_s, totalVolume_mL);
    circiutTrk.Track(time_s, cvCircuit);

    // See if we are stable
    double map_mmHg = cv.GetMeanArterialPressure(PressureUnit::mmHg);
    double sys_mmHg = cv.GetSystolicArterialPressure(PressureUnit::mmHg);
    double dia_mmHg = cv.GetDiastolicArterialPressure(PressureUnit::mmHg);
    double co_mL_per_s = cv.GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    double blood_mL = cv.GetBloodVolume(VolumeUnit::mL);

    circiutTrk.Track("MAP_mmHg", time_s, map_mmHg);
    circiutTrk.Track("Systolic_mmHg", time_s, sys_mmHg);
    circiutTrk.Track("Diastolilc_mmHg", time_s, dia_mmHg);
    circiutTrk.Track("CardiacOutput_mL_per_s", time_s, co_mL_per_s);
    circiutTrk.Track("BloodVolume_mL", time_s, blood_mL);

    if (!scale)
    {
      if (i==0)
      {
        circiutTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "CircuitOutput.csv").c_str(), circuitFile);
        cvGraphTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "TransportOutput.csv").c_str(), cvGraphFile);
      }
      circiutTrk.StreamTrackToFile(circuitFile);
      cvGraphTrk.StreamTrackToFile(cvGraphFile);
    }
    
    if(breakOutResults)
    {
      cvHDTrk.Track("MAP_mmHg", time_s, map_mmHg);
      cvHDTrk.Track("Systolic_mmHg", time_s, sys_mmHg);
      cvHDTrk.Track("Diastolilc_mmHg", time_s, dia_mmHg);
      cvHDTrk.Track("CardiacOutput_mL_per_s", time_s, co_mL_per_s);
      cvHDTrk.Track("BloodVolume_mL", time_s, blood_mL);
      cvHDTrk.Track("TotalVolume_mL", time_s, totalVolume_mL);

      for (SELiquidCompartment* cmpt : cvGraph.GetCompartments())
      {
        if (cmpt->HasPressure())
        {
          auto unit = cmpt->GetPressure().GetUnit();
          cvGraphPresTrk.Track(cmpt->GetName() + "_Pressure_" + unit->GetString(), time_s, cmpt->GetPressure().GetValue(*unit));
        }
        if (cmpt->HasVolume())
        {
          auto unit = cmpt->GetVolume().GetUnit();
          cvGraphVolTrk.Track(cmpt->GetName() + "_Volume_" + unit->GetString(), time_s, cmpt->GetVolume().GetValue(*unit));
        }

      }

      for (SELiquidCompartmentLink* link : cvGraph.GetLinks())
      {
        if (link->HasFlow())
        {
          auto unit = link->GetFlow().GetUnit();
          cvGraphFlowTrk.Track(link->GetName() + "_Flow_" + unit->GetString(), time_s, link->GetFlow(*unit));
        }
      }


      for (SEFluidCircuitNode* n : cvCircuit.GetNodes())
      {
        if (n->HasNextPressure())
        {
          auto unit = n->GetNextPressure().GetUnit();
          cvPressureTrk.Track(n->GetName() + "_" + unit->GetString(), time_s, n->GetNextPressure().GetValue(*unit));
        }
        if (n->HasNextVolume())
        {
          auto unit = n->GetNextVolume().GetUnit();
          double volume = n->GetNextVolume().GetValue(*unit);
          if (volume < 0)
            pc.GetLogger()->Error("Negative volume for : " + n->GetName());
          cvVolumeTrk.Track(n->GetName() + "_" + unit->GetString(), time_s, volume);
          if (n->HasVolumeBaseline())
          {
            double baseVolume = n->GetVolumeBaseline().GetValue(*unit);
            cvNormVolumeTrk.Track(n->GetName() + "_" + unit->GetString(), time_s, volume / baseVolume);
          }
        }
      }
      for (SEFluidCircuitPath* p : cvCircuit.GetPaths())
      {
        if (p->HasNextFlow())
        {
          if (p->HasComplianceBaseline())
          {
            auto unit = p->GetNextFlow().GetUnit();
            cvCompFlowTrk.Track(p->GetName() + "_" + unit->GetString(), time_s, p->GetNextFlow().GetValue(*unit));
          }
          else if (p->HasResistanceBaseline())
          {
            auto unit = p->GetNextFlow().GetUnit();
            cvResFlowTrk.Track(p->GetName() + "_" + unit->GetString(), time_s, p->GetNextFlow().GetValue(*unit));
          }
          else
          {
            auto unit = p->GetNextFlow().GetUnit();
            cvFlowTrk.Track(p->GetName() + "_" + unit->GetString(), time_s, p->GetNextFlow().GetValue(*unit));
          }          
        }
      }
      if (i == 0)
      {
        cvPressureTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_PressureOutput.csv").c_str(), cvPressureFile);
        cvVolumeTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_VolumeOutput.csv").c_str(), cvVolumeFile);
        cvNormVolumeTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_NormVolumeOutput.csv").c_str(), cvNormVolumeFile);
        cvCompFlowTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_CompFlowOutput.csv").c_str(), cvCompFlowFile);
        cvResFlowTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_ResFlowOutput.csv").c_str(), cvResFlowFile);
        cvFlowTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_FlowOutput.csv").c_str(), cvFlowFile);
        cvHDTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_HD.csv").c_str(), cvHDFile);
        cvGraphPresTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_CPMTPressureOutput.csv").c_str(), cvGraphPresFile);
        cvGraphFlowTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_CPMTFlowOutput.csv").c_str(), cvGraphFlowFile);
        cvGraphVolTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_CPMTVolumeOutput.csv").c_str(), cvGraphVolFile);        
      }
      cvPressureTrk.StreamTrackToFile(cvPressureFile);
      cvVolumeTrk.StreamTrackToFile(cvVolumeFile);
      cvNormVolumeTrk.StreamTrackToFile(cvNormVolumeFile);
      cvCompFlowTrk.StreamTrackToFile(cvCompFlowFile);
      cvResFlowTrk.StreamTrackToFile(cvResFlowFile);
      cvFlowTrk.StreamTrackToFile(cvFlowFile);
      cvHDTrk.StreamTrackToFile(cvHDFile);
      cvGraphPresTrk.StreamTrackToFile(cvGraphPresFile);
      cvGraphFlowTrk.StreamTrackToFile(cvGraphFlowFile);
      cvGraphVolTrk.StreamTrackToFile(cvGraphVolFile);
    }   

    time_s += timeStep_s;
    if (i % 1000 == 0)
    {
      ss << "Current time(s) " << time_s;
      pc.GetLogger()->Info(ss);
      ss << "Total Circuit time(s) so far :" << circuit_s;
      pc.GetLogger()->Info(ss);
      ss << "Total Transport time(s) so far :" << transport_s;
      pc.GetLogger()->Info(ss);
      ss << "Total Binding time(s) so far :" << binding_s;
      pc.GetLogger()->Info(ss);
    }
  }

  if (breakOutResults)
  {
    // Make a file with the final resistances and compliances
    for (SEFluidCircuitPath* p : cvCircuit.GetPaths())
    {
      if (p->HasComplianceBaseline())
      {
        auto unit = p->GetComplianceBaseline().GetUnit();
        cvCompTrk.Track(p->GetName() + "_" + unit->GetString(), time_s, p->GetComplianceBaseline().GetValue(*unit));
      }
      else if (p->HasResistanceBaseline())
      {
        auto unit = p->GetResistanceBaseline().GetUnit();
        cvResTrk.Track(p->GetName() + "_" + unit->GetString(), time_s, p->GetResistanceBaseline().GetValue(*unit));
      }
    }

    cvCompBaseTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "CompBase.csv").c_str(), cvCompBaseFile);
    cvResBaseTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_ResBase.csv").c_str(), cvResBaseFile);
    cvCompTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_Comp.csv").c_str(), cvCompFile);
    cvResTrk.CreateFile(std::string(sTestDirectory + "/" + tName.str() + "_Res.csv").c_str(), cvResFile);

    cvCompBaseTrk.StreamTrackToFile(cvCompBaseFile);
    cvResBaseTrk.StreamTrackToFile(cvResBaseFile);
    cvCompTrk.StreamTrackToFile(cvCompFile);
    cvResTrk.StreamTrackToFile(cvResFile);

     cvPressureFile.close();
    cvVolumeFile.close();
    cvNormVolumeFile.close();
    cvCompFlowFile.close();
    cvResFlowFile.close();
    cvFlowFile.close();

    cvHDFile.close();
    cvGraphPresFile.close();
    cvGraphFlowFile.close();
    cvGraphVolFile.close();
    cvCompBaseFile.close();
    cvResBaseFile.close();
    cvCompFile.close();
    cvResFile.close();
  }

  circuitFile.close();
  cvGraphFile.close();

  ss << "It took " << tmr.GetElapsedTime_s("Test") << "s to run";
  pc.GetLogger()->Info(ss, "CardiovascularCircuitAndTransportTest");
}

void PulseEngineTest::SinusoidHeartDriver(double time_s, double heartFreq_Per_s, double& lHeartElastance, double& rHeartElastance)
{
  double RmaxElastance = 0.523;
  double RminElastance = 0.0243;
  double LmaxElastance = 2.49;
  double LminElastance = 0.049;
  double omega = heartFreq_Per_s * 2.0 * 3.14; // 3.14 is pi
  double RhalfAmp = (RmaxElastance - RminElastance) * 0.5;
  double LhalfAmp = (LmaxElastance - LminElastance) * 0.5;
  double wave = cos(omega*time_s);
  rHeartElastance = RminElastance + RhalfAmp - RhalfAmp * wave;
  lHeartElastance = LminElastance + LhalfAmp - LhalfAmp * wave;
}
// Using the Cardiovascular Driver, but this is the math if you want
//void PulseEngineTest::HeartDriver(double cycleTime_s, double period_s, double& lHeartElastance, double& rHeartElastance)
//{
//  double alpha1 = 0.303;
//  double alpha2 = 0.508;
//  double n1 = 1.32;
//  double n2 = 21.9;
//  double maxShape = 0.598;
//  double RmaxElastance = 0.523;
//  double RminElastance = 0.0243;
//  double LmaxElastance = 2.49;
//  double LminElastance = 0.049;
//  double normalizedCardiacTime = cycleTime_s / period_s; //Fraction of the way through the current cardiac cycle
//  double elastanceShapeFunction = (pow(normalizedCardiacTime / alpha1, n1) / (1.0 + pow(normalizedCardiacTime / alpha1, n1)))*(1.0 / (1.0 + pow(normalizedCardiacTime / alpha2, n2))) / maxShape;
//
//  lHeartElastance = (LmaxElastance - LminElastance)*elastanceShapeFunction + LminElastance;
//  rHeartElastance = (RmaxElastance - RminElastance)*elastanceShapeFunction + RminElastance;
//}

void PulseEngineTest::CardiovascularCircuitScaleTests(const std::string& sTestDirectory)
{
  DataTrack     cvLastMeanPressureTrk;
  std::ofstream cvLastMeanPressureFile;
  std::ostringstream ss;
  bool firstTest = true;
  unsigned int testNo = 1;
  m_MeanAortaPressure = 0;
  m_MeanCVPressure = 0;
  m_Systolic = 0;
  m_Diastolic = 0;
  m_Pulse = 0;
  m_CardiacOutput = 0;

  // You can scale the circuits: Compliances, Resistances, Volumes, and Driver Frequency/HeartRate
  // Note: You could scale the amplitude/elastance of the driver if want (We didn't at the time I wrote it, but its possible!)
  double heartRate_bpm = 72;// Note, you should always pass in a bpm other than <=0 for a well named file
  ss << heartRate_bpm;
  double comp = 1.0, res = 1.0, vol = 1, sysRes = 1, sysComp = 1, aortaRes = 0.7, aortaComp = 0.7, venaRes = 1, venaComp = 1;

  //for (double rFactor = 0.3; rFactor <= 1.7; rFactor += 0.1)
  //{
  //  CardiovascularCircuitAndTransportTest(Heart, 1, rFactor,1,heartRate_bpm, false, true, false, sTestDirectory, "CardiovascularScale");
  //}

  //CardiovascularCircuitAndTransportTest(Heart, comp, res, vol, heartRate_bpm, sysRes, sysComp, aortaRes, aortaComp, venaRes, venaComp, false, false, false, sTestDirectory, "CardiovascularScale");

  for (double srFactor = 1; srFactor < 1.1; srFactor += 0.25)
  {
    for (double scFactor = 1; scFactor < 1.1; scFactor += 0.25)
    {
      for (double arFactor = 1; arFactor < 1.1; arFactor += 0.25)
      {
        for (double acFactor = 1; acFactor < 1.1; acFactor += 0.25)
        {
          for (double vrFactor = 0.5; vrFactor < 1.51; vrFactor += 0.25)
          {
            for (double vcFactor = 0.5; vcFactor < 1.51; vcFactor += 0.25)
            {
              CardiovascularCircuitAndTransportTest(Heart, comp, res, vol, heartRate_bpm, srFactor, scFactor, arFactor, acFactor, vrFactor, vcFactor, false, false, false, sTestDirectory, "CVScale", true);

              cvLastMeanPressureTrk.Track("SystemicResistanceScale", testNo, srFactor);
              cvLastMeanPressureTrk.Track("SystemicComplianceScale", testNo, scFactor);
              cvLastMeanPressureTrk.Track("AortaResistanceScale", testNo, arFactor);
              cvLastMeanPressureTrk.Track("AortaComplianceScale", testNo, acFactor);
              cvLastMeanPressureTrk.Track("VenaResistanceScale", testNo, vrFactor);
              cvLastMeanPressureTrk.Track("VenaComplianceScale", testNo, vcFactor);
              cvLastMeanPressureTrk.Track("MeanAortaPressure_mmHg", testNo, m_MeanAortaPressure);
              cvLastMeanPressureTrk.Track("MeanCVPressure_mmHg", testNo, m_MeanCVPressure);
              cvLastMeanPressureTrk.Track("Systolic_mmHg", testNo, m_Systolic);
              cvLastMeanPressureTrk.Track("Diastolic_mmHg", testNo, m_Diastolic);
              cvLastMeanPressureTrk.Track("Pulse_mmHg", testNo, m_Pulse);
              cvLastMeanPressureTrk.Track("CardiacOutput_mL_Per_min", testNo, m_CardiacOutput);

              if (firstTest)
              {
                firstTest = false;
                cvLastMeanPressureTrk.CreateFile(std::string(sTestDirectory + "/" + "CVVenaScale" + ss.str() + "_LastMeanPressureOutput.csv").c_str(), cvLastMeanPressureFile);
              }
              cvLastMeanPressureTrk.StreamTrackToFile(cvLastMeanPressureFile);
              testNo++;
            }
          }
        }
      }
    }
  }

  //for (double rFactor = 0.5; rFactor <= 1.51; rFactor += 0.1)
  //{
  //  for (double cFactor = 0.5; cFactor <= 1.51; cFactor += 0.1)
  //  {

  //    CardiovascularCircuitAndTransportTest(Heart, comp, res, vol, heartRate_bpm, sysRes, sysComp, aortaRes, aortaComp, venaRes, venaComp, false, false, false, sTestDirectory, "CardiovascularScaleTT");

  //    cvLastMeanPressureTrk.Track("ComplianceScale", testNo, m_ComplianceScale);
  //    cvLastMeanPressureTrk.Track("ResistanceScale", testNo, m_ResistanceScale);
  //    cvLastMeanPressureTrk.Track("MeanAortaPressure_mmHg", testNo, m_MeanAortaPressure);
  //    cvLastMeanPressureTrk.Track("MeanCVPressure_mmHg", testNo, m_MeanCVPressure);
  //    cvLastMeanPressureTrk.Track("Systolic_mmHg", testNo, m_Systolic);
  //    cvLastMeanPressureTrk.Track("Diastolic_mmHg", testNo, m_Diastolic);
  //    cvLastMeanPressureTrk.Track("Pulse_mmHg", testNo, m_Pulse);
  //    cvLastMeanPressureTrk.Track("CardiacOutput_mL_Per_min", testNo, m_CardiacOutput);

  //    if (firstTest)
  //    {
  //      firstTest = false;
  //      cvLastMeanPressureTrk.CreateFile(std::string(sTestDirectory + "/" + "CardiovascularScale" + ss.str() + "_LastMeanPressureOutput.csv").c_str(), cvLastMeanPressureFile);
  //    }
  //    cvLastMeanPressureTrk.StreamTrackToFile(cvLastMeanPressureFile);
  //    testNo++;
  //  }
  //}

  //for (double rFactor = 0.5; rFactor <= 1.51; rFactor += 0.1)
  //{
  //  for (double cFactor = 0.5; cFactor <= 1.51; cFactor += 0.1)
  //  {

  //    CardiovascularCircuitAndTransportTest(Heart, cFactor, rFactor, 1, heartRate_bpm, false, false, false, sTestDirectory, "CardiovascularScale");

  //    cvLastMeanPressureTrk.Track("ComplianceScale", testNo, m_ComplianceScale);
  //    cvLastMeanPressureTrk.Track("ResistanceScale", testNo, m_ResistanceScale);
  //    cvLastMeanPressureTrk.Track("MeanAortaPressure_mmHg", testNo, m_MeanAortaPressure);
  //    cvLastMeanPressureTrk.Track("MeanCVPressure_mmHg", testNo, m_MeanCVPressure);
  //    cvLastMeanPressureTrk.Track("Systolic_mmHg", testNo, m_Systolic);
  //    cvLastMeanPressureTrk.Track("Diastolic_mmHg", testNo, m_Diastolic);
  //    cvLastMeanPressureTrk.Track("Pulse_mmHg", testNo, m_Pulse);
  //    cvLastMeanPressureTrk.Track("CardiacOutput_mL_Per_min", testNo, m_CardiacOutput);

  //    if (firstTest)
  //    {
  //      firstTest = false;
  //      cvLastMeanPressureTrk.CreateFile(std::string(sTestDirectory + "/" + "CardiovascularScale" + ss.str() + "_LastMeanPressureOutput.csv").c_str(), cvLastMeanPressureFile);
  //    }
  //    cvLastMeanPressureTrk.StreamTrackToFile(cvLastMeanPressureFile);
  //    testNo++;
  //  }
  //}

}
