/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"
#include "engine/human_adult/whole_body/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/system/physiology/Saturation.h"
#include "engine/human_adult/whole_body/system/physiology/TissueModel.h"

#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/tissue/SETissueCompartment.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerAmount.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarMassPerAreaTime.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/DataTrack.h"

namespace pulse { namespace human_adult_whole_body
{
  void EngineTest::DistributeMass(SETestSuite& testSuite)
  {
    TimingProfile timer;
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    // First test case  
    SETestCase& testCase = testSuite.CreateTestCase();
    testCase.SetName("DistributeMassToHierarchy");
    timer.Start("Test");

    //          L0C0         //
    //         /    \        //
    //     L1C0      L1C1    //
    //     /  \      /  \    //
    //  L2C0  L2C1 L2C2 L2C3 <-- Only these cmpts have data

    pc.GetSubstances().LoadSubstanceDirectory();

    SELiquidCompartment& L0C0 = pc.GetCompartments().CreateLiquidCompartment("L0C0");
    SELiquidCompartment& L1C0 = pc.GetCompartments().CreateLiquidCompartment("L1C0");
    SELiquidCompartment& L1C1 = pc.GetCompartments().CreateLiquidCompartment("L1C1");
    SELiquidCompartment& L2C0 = pc.GetCompartments().CreateLiquidCompartment("L2C0");
    SELiquidCompartment& L2C1 = pc.GetCompartments().CreateLiquidCompartment("L2C1");
    SELiquidCompartment& L2C2 = pc.GetCompartments().CreateLiquidCompartment("L2C2");
    SELiquidCompartment& L2C3 = pc.GetCompartments().CreateLiquidCompartment("L2C3");

    // Build up the hierarchy
    L0C0.AddChild(L1C0);
    L0C0.AddChild(L1C1);
    L1C0.AddChild(L2C0);
    L1C0.AddChild(L2C1);
    L1C1.AddChild(L2C2);
    L1C1.AddChild(L2C3);
    pc.GetCompartments().StateChange();// Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

    SESubstance* sub = pc.GetSubstances().GetSubstance("Oxygen");
    pc.GetSubstances().AddActiveSubstance(*sub);

    double L2C0_mL = 10;
    double L2C0_g = 10;
    double L2C1_mL = 20;
    double L2C1_g = 20;
    double L2C2_mL = 40;
    double L2C2_g = 40;
    double L2C3_mL = 80;
    double L2C3_g = 80;
    double total_mL = L2C0_mL + L2C1_mL + L2C2_mL + L2C3_mL;

    L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
    L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
    L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
    L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
    L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
    L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
    L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
    L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

    double delta_g = -20;

    double expected_L2C0_g = L2C0_g + (delta_g * L2C0_mL / total_mL);
    double expected_L2C1_g = L2C1_g + (delta_g * L2C1_mL / total_mL);
    double expected_L2C2_g = L2C2_g + (delta_g * L2C2_mL / total_mL);
    double expected_L2C3_g = L2C3_g + (delta_g * L2C3_mL / total_mL);

    tsu.DistributeMassbyVolumeWeighted(L0C0, *sub, delta_g, MassUnit::g);

    double L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
      testCase.AddFailure("DistributeMassbyVolumeWeighted L2C0 mass is not expected mass");

    double L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
      testCase.AddFailure("DistributeMassbyVolumeWeighted L2C1 mass is not expected mass");

    double L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
      testCase.AddFailure("DistributeMassbyVolumeWeighted L2C2 mass is not expected mass");

    double L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
      testCase.AddFailure("DistributeMassbyVolumeWeighted L2C3 mass is not expected mass");

    // Log how long our test took
    testCase.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

    // Second test case  
    SETestCase& testCase2 = testSuite.CreateTestCase();
    testCase2.SetName("DistributeMassbyMassToHierarchy");
    timer.Start("Test");

    L2C0_mL = 10;
    L2C0_g = 10;
    L2C1_mL = 20;
    L2C1_g = 20;
    L2C2_mL = 40;
    L2C2_g = 40;
    L2C3_mL = 80;
    L2C3_g = 80;
    double total_g = L2C0_g + L2C1_g + L2C2_g + L2C3_g;

    L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
    L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
    L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
    L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
    L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
    L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
    L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
    L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

    delta_g = -20;

    expected_L2C0_g = L2C0_g + (delta_g * L2C0_g / total_g);
    expected_L2C1_g = L2C1_g + (delta_g * L2C1_g / total_g);
    expected_L2C2_g = L2C2_g + (delta_g * L2C2_g / total_g);
    expected_L2C3_g = L2C3_g + (delta_g * L2C3_g / total_g);

    tsu.DistributeMassbyMassWeighted(L0C0, *sub, delta_g, MassUnit::g);

    L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
      testCase2.AddFailure("DistributeMassbyMassWeighted L2C0 mass is not expected mass");

    L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
      testCase2.AddFailure("DistributeMassbyMassWeighted L2C1 mass is not expected mass");

    L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
      testCase2.AddFailure("DistributeMassbyMassWeighted L2C2 mass is not expected mass");

    L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
      testCase2.AddFailure("DistributeMassbyMassWeighted L2C3 mass is not expected mass");

    // Third test case
    // Second test case  
    SETestCase& testCase3 = testSuite.CreateTestCase();
    testCase3.SetName("DistributeMassbyMassToHierarchy_tooMuch");
    timer.Start("Test");

    L2C0_mL = 10;
    L2C0_g = 10;
    L2C1_mL = 20;
    L2C1_g = 20;
    L2C2_mL = 40;
    L2C2_g = 40;
    L2C3_mL = 80;
    L2C3_g = 80;
    total_g = L2C0_g + L2C1_g + L2C2_g + L2C3_g;

    L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
    L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
    L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
    L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
    L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
    L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
    L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
    L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

    delta_g = -2000;

    expected_L2C0_g = 0.;
    expected_L2C1_g = 0.;
    expected_L2C2_g = 0.;
    expected_L2C3_g = 0.;

    tsu.DistributeMassbyMassWeighted(L0C0, *sub, delta_g, MassUnit::g);

    L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
      testCase3.AddFailure("L2C0 mass is not expected mass - Expecting zero");

    L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
      testCase3.AddFailure("L2C1 mass is not expected mass - Expecting zero");

    L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
      testCase3.AddFailure("L2C2 mass is not expected mass - Expecting zero");

    L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
    if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
      testCase3.AddFailure("L2C3 mass is not expected mass - Expecting zero");


  }

  void EngineTest::PerfusionLimitedDiffusionTest(SETestSuite& testSuite)
  {
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    TimingProfile timer;
    double timestep_s = 1. / 90.;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* sub = pc.GetSubstances().GetSubstance("Ketamine");
    double bFlow_mL_Per_s = 2.0;
    double PartitionCoeff = 1.52201;
    double matrixVolume_mL = 2.0;
    double tissueMass_ug = 3.;
    double bVol_mL = 10.;
    double bConc_ug_Per_mL = 10.;

    pc.GetSubstances().AddActiveSubstance(*sub);

    // First test case  
    SETestCase& testCase1 = testSuite.CreateTestCase();
    testCase1.SetName("PerfusionLimitedDiffusionTest");
    timer.Start("Test");

    SETissueCompartment& tissue = pc.GetCompartments().CreateTissueCompartment(pulse::TissueCompartment::Bone);
    SELiquidCompartment& extracellular = pc.GetCompartments().CreateLiquidCompartment(pulse::ExtravascularCompartment::BoneExtracellular);
    SELiquidCompartment& intracellular = pc.GetCompartments().CreateLiquidCompartment(pulse::ExtravascularCompartment::BoneIntracellular);
    tissue.GetMatrixVolume().SetValue(matrixVolume_mL, VolumeUnit::mL);
    intracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(tissueMass_ug, MassUnit::ug);
    intracellular.GetVolume().SetValue(1.0, VolumeUnit::mL); //Need fluid volume to balance. 
    extracellular.GetVolume().SetValue(1.0, VolumeUnit::mL); //Should be fine, right? We don't validate and tissue concentrations.

    SELiquidCompartment& vascular = pc.GetCompartments().CreateLiquidCompartment(pulse::VascularCompartment::Bone);
    vascular.GetVolume().SetValue(bVol_mL, VolumeUnit::mL);
    vascular.GetSubstanceQuantity(*sub)->GetConcentration().SetValue(bConc_ug_Per_mL, MassPerVolumeUnit::ug_Per_mL);
    vascular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Concentration);
    intracellular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Mass);

    SELiquidCompartmentLink& flow = pc.GetCompartments().CreateLiquidLink(intracellular, vascular, "ExtravascularExchange");
    flow.GetFlow().SetValue(bFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
    extracellular.AddLink(flow);
    vascular.AddLink(flow);

    pc.GetCompartments().StateChange();

    double rtnMassInc_ug = tsu.PerfusionLimitedDiffusion(tissue, vascular, *sub, PartitionCoeff, timestep_s);

    // Check
    double MassIncrement_ug = bFlow_mL_Per_s * timestep_s * ((bConc_ug_Per_mL)-((tissueMass_ug / matrixVolume_mL) / PartitionCoeff));
    if (std::abs(rtnMassInc_ug - MassIncrement_ug) > 1.e-10)
    {
      testCase1.AddFailure("PerfusionLimitedDiffusion returned unexpected mass increment.");
    }

    // Log how long our test took
    testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

    // Subsequent test cases
    //timer.Start("Test");
  }

  void EngineTest::AlveolarOxygenDiffusionTest(const std::string& rptDirectory)
  {
    //This test examines diffusion of O2 from a gas compartment (like lungs) to a liquid compartment
    //The gas compartment is set to resemble atmospheric conditions, and the liquid compartment is oxygen-poor
    //We should expect to see oxygen diffuse into the liquid compartment
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();

    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* O2 = pc.GetSubstances().GetSubstance("Oxygen");
    SESubstance* N2 = pc.GetSubstances().GetSubstance("Nitrogen");
    pc.GetSubstances().AddActiveSubstance(*O2);
    pc.GetSubstances().AddActiveSubstance(*N2);

    SELiquidCompartment& lcmpt = pc.GetCompartments().CreateLiquidCompartment("lcmpt");
    SEGasCompartment& gcmpt = pc.GetCompartments().CreateGasCompartment("gcmpt");

    DataTrack trk1;
    std::string rptFile = rptDirectory + "/AlveolarOxygenDiffusionTest.csv";
    double time = 0.0;

    double liquidVol_mL = 1000.0;
    double lMass_g = .001;  //oxygen-poor

    gcmpt.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
    lcmpt.GetVolume().SetValue(liquidVol_mL, VolumeUnit::mL);

    gcmpt.GetSubstanceQuantity(*O2)->GetVolumeFraction().SetValue(.2);
    gcmpt.GetSubstanceQuantity(*N2)->GetVolumeFraction().SetValue(.8);
    gcmpt.GetPressure().SetValue(760, PressureUnit::mmHg);
    lcmpt.GetSubstanceQuantity(*O2)->GetMass().SetValue(lMass_g, MassUnit::g);
    lcmpt.GetSubstanceQuantity(*N2)->GetMass().SetValue(lMass_g, MassUnit::g);
    gcmpt.Balance(BalanceGasBy::VolumeFraction);
    lcmpt.Balance(BalanceLiquidBy::Mass);

    double diffusingCapacityO2_mL_Per_s_mmHg = .33;

    double l_O2_ug = lcmpt.GetSubstanceQuantity(*O2)->GetMass().GetValue(MassUnit::ug);
    double lconc = lcmpt.GetSubstanceQuantity(*O2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);


    trk1.Track("LiquidCompartmentOxygenMass_ug", time, l_O2_ug);
    trk1.Track("LiquidCompartmentOxygenConc_ug_Per_L", time, lconc);
    trk1.Track("LiquidCompartmentPO2_mmHg", time, lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
    trk1.Track("GasCompartmentPO2_mmHg", time, gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));

    for (int i = 0; i < 180; i++)
    {
      tsu.AlveolarPartialPressureGradientDiffusion(gcmpt, lcmpt, *O2, diffusingCapacityO2_mL_Per_s_mmHg, timestep_s);
      gcmpt.Balance(BalanceGasBy::VolumeFraction);
      time += timestep_s;
      l_O2_ug = lcmpt.GetSubstanceQuantity(*O2)->GetMass().GetValue(MassUnit::ug);
      lconc = lcmpt.GetSubstanceQuantity(*O2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
      trk1.Track("LiquidCompartmentOxygenMass_ug", time, l_O2_ug);
      trk1.Track("LiquidCompartmentOxygenConc_ug_Per_L", time, lconc);
      trk1.Track("LiquidCompartmentPO2_mmHg", time, lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
      trk1.Track("GasCompartmentPO2_mmHg", time, gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
    }

    // Write data to file
    trk1.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::AlveolarCarbonDioxideDiffusionTest(const std::string& rptDirectory)
  {
    //Second test case: High liquid compartment CO2 concentration with a lower gas compartment CO2 concentration
    //We should expect CO2 to diffuse out of the liquid compartment into the gas compartment
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();

    DataTrack trk2;
    std::string rptFile = rptDirectory + "/AlveolarCarbonDioxideDiffusionTest.csv";

    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();

    SESubstance* CO2 = pc.GetSubstances().GetSubstance("CarbonDioxide");
    SESubstance* N2 = pc.GetSubstances().GetSubstance("Nitrogen");
    pc.GetSubstances().AddActiveSubstance(*CO2);
    pc.GetSubstances().AddActiveSubstance(*N2);

    SELiquidCompartment& lcmpt2 = pc.GetCompartments().CreateLiquidCompartment("lcmpt2");
    SEGasCompartment& gcmpt2 = pc.GetCompartments().CreateGasCompartment("gcmpt2");

    double time = 0.0;

    double liquidVol_mL = 1000.0;
    double lMass_g = .04;  //high amount of CO2

    gcmpt2.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
    lcmpt2.GetVolume().SetValue(liquidVol_mL, VolumeUnit::mL);

    gcmpt2.GetSubstanceQuantity(*CO2)->GetVolumeFraction().SetValue(.0004); //atmospheric CO2 is .04%
    gcmpt2.GetSubstanceQuantity(*N2)->GetVolumeFraction().SetValue(1 - .0004);
    gcmpt2.GetPressure().SetValue(760, PressureUnit::mmHg);
    lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().SetValue(lMass_g, MassUnit::g);
    lcmpt2.GetSubstanceQuantity(*N2)->GetMass().SetValue(lMass_g, MassUnit::g);
    gcmpt2.Balance(BalanceGasBy::VolumeFraction);
    lcmpt2.Balance(BalanceLiquidBy::Mass);

    double l_CO2_ug = lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().GetValue(MassUnit::ug);
    double lconc = lcmpt2.GetSubstanceQuantity(*CO2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
    double diffusingCapacityO2_mL_Per_s_mmHg = .33;

    trk2.Track("LiquidCompartmentCO2Mass_ug", time, l_CO2_ug);
    trk2.Track("LiquidCompartmentCO2Conc_ug_Per_L", time, lconc);
    trk2.Track("LiquidCompartmentPCO2_mmHg", time, lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
    trk2.Track("GasCompartmentPCO2_mmHg", time, gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));

    for (int i = 0; i < 180; i++)
    {
      tsu.AlveolarPartialPressureGradientDiffusion(gcmpt2, lcmpt2, *CO2, diffusingCapacityO2_mL_Per_s_mmHg, timestep_s);
      gcmpt2.Balance(BalanceGasBy::VolumeFraction);
      time += timestep_s;
      l_CO2_ug = lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().GetValue(MassUnit::ug);
      lconc = lcmpt2.GetSubstanceQuantity(*CO2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
      trk2.Track("LiquidCompartmentCO2Mass_ug", time, l_CO2_ug);
      trk2.Track("LiquidCompartmentCO2Conc_ug_Per_L", time, lconc);
      trk2.Track("LiquidCompartmentPCO2_mmHg", time, lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
      trk2.Track("GasCompartmentPCO2_mmHg", time, gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
    }

    // Write data to file
    trk2.WriteTrackToFile(rptFile.c_str());

  }

  void EngineTest::InstantPlusSimpleDiffusionTest(const std::string& rptDirectory)
  {
    // Second test - cmpt2 and cmpt4 are connected by instant diffusion, cmpt2 and cmpt1 by simple and cmpt2 and cmpt3 by simple
    //        cmpt1 <-> cmpt2 <-> cmpt3
    //                    |
    //                  cmpt4
    // Expect cmpt2 and cmpt4 to quickly equilibrate, while the others take more time
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* o2 = pc.GetSubstances().GetSubstance("Oxygen");
    pc.GetSubstances().AddActiveSubstance(*o2);
    SELiquidCompartment& cmpt1 = pc.GetCompartments().CreateLiquidCompartment("cmpt1");
    SELiquidCompartment& cmpt2 = pc.GetCompartments().CreateLiquidCompartment("cmpt2");
    SELiquidCompartment& cmpt3 = pc.GetCompartments().CreateLiquidCompartment("cmpt3");
    SELiquidCompartment& cmpt4 = pc.GetCompartments().CreateLiquidCompartment("cmpt4");

    DataTrack trk;
    std::string rptFile = rptDirectory + "/InstantPlusSimpleDiffusionTest.csv";
    double time = 0.0;

    double permeabilityCoefficient21_mL_Per_s = 5.0;
    double permeabilityCoefficient23_mL_Per_s = 500.0;

    cmpt1.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(100.0, MassPerVolumeUnit::g_Per_L);
    cmpt2.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(100.0, MassPerVolumeUnit::g_Per_L);
    cmpt3.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(100.0, MassPerVolumeUnit::g_Per_L);
    cmpt4.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::g_Per_L);
    cmpt1.GetVolume().SetValue(1.0, VolumeUnit::L);
    cmpt2.GetVolume().SetValue(5.0, VolumeUnit::L);
    cmpt3.GetVolume().SetValue(20.0, VolumeUnit::L);
    cmpt4.GetVolume().SetValue(10.0, VolumeUnit::L);
    cmpt1.Balance(BalanceLiquidBy::Concentration);
    cmpt2.Balance(BalanceLiquidBy::Concentration);
    cmpt3.Balance(BalanceLiquidBy::Concentration);
    cmpt4.Balance(BalanceLiquidBy::Concentration);

    trk.Track("cmpt1Mass_g", time, cmpt1.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
    trk.Track("cmpt2Mass_g", time, cmpt2.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
    trk.Track("cmpt3Mass_g", time, cmpt3.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
    trk.Track("cmpt4Mass_g", time, cmpt4.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
    trk.Track("cmpt1Conc_g_Per_L", time, cmpt1.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
    trk.Track("cmpt2Conc_g_Per_L", time, cmpt2.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
    trk.Track("cmpt3Conc_g_Per_L", time, cmpt3.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
    trk.Track("cmpt4Conc_g_Per_L", time, cmpt4.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));

    for (int i = 0; i < 3600; i++)
    {
      tsu.MoveMassByInstantDiffusion(cmpt2, cmpt4, *o2);
      tsu.MoveMassBySimpleDiffusion(cmpt2, cmpt1, *o2, permeabilityCoefficient21_mL_Per_s, timestep_s);
      tsu.MoveMassBySimpleDiffusion(cmpt2, cmpt3, *o2, permeabilityCoefficient23_mL_Per_s, timestep_s);
      cmpt1.Balance(BalanceLiquidBy::Mass);
      cmpt2.Balance(BalanceLiquidBy::Mass);
      cmpt3.Balance(BalanceLiquidBy::Mass);
      cmpt4.Balance(BalanceLiquidBy::Mass);
      time += timestep_s;
      trk.Track("cmpt1Mass_g", time, cmpt1.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
      trk.Track("cmpt2Mass_g", time, cmpt2.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
      trk.Track("cmpt3Mass_g", time, cmpt3.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
      trk.Track("cmpt4Mass_g", time, cmpt4.GetSubstanceQuantity(*o2)->GetMass(MassUnit::g));
      trk.Track("cmpt1Conc_g_Per_L", time, cmpt1.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
      trk.Track("cmpt2Conc_g_Per_L", time, cmpt2.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
      trk.Track("cmpt3Conc_g_Per_L", time, cmpt3.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
      trk.Track("cmpt4Conc_g_Per_L", time, cmpt4.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L));
    }

    trk.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::InstantDiffusionTest(SETestSuite& testSuite)
  {
    TimingProfile timer;
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* o2 = pc.GetSubstances().GetSubstance("Oxygen");
    pc.GetSubstances().AddActiveSubstance(*o2);
    SELiquidCompartment& cmpt1 = pc.GetCompartments().CreateLiquidCompartment("cmpt1");
    SELiquidCompartment& cmpt2 = pc.GetCompartments().CreateLiquidCompartment("cmpt2");

    // First test - simple two compartment instant diffusion test
    timer.Start("Test");
    SETestCase& testCase1 = testSuite.CreateTestCase();
    testCase1.SetName("InstantDiffusionTest");

    cmpt1.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(100.0, MassPerVolumeUnit::g_Per_L);
    cmpt2.GetSubstanceQuantity(*o2)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::g_Per_L);

    cmpt1.GetVolume().SetValue(1.0, VolumeUnit::L);
    cmpt2.GetVolume().SetValue(10.0, VolumeUnit::L);
    cmpt1.Balance(BalanceLiquidBy::Concentration);
    cmpt2.Balance(BalanceLiquidBy::Concentration);

    tsu.MoveMassByInstantDiffusion(cmpt1, cmpt2, *o2);
    cmpt1.Balance(BalanceLiquidBy::Mass);
    cmpt2.Balance(BalanceLiquidBy::Mass);

    if (std::abs(cmpt1.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L) - cmpt2.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L)) > ZERO_APPROX)
      testCase1.AddFailure("Unequal concentrations after instant diffusion.");
    if (std::abs(cmpt1.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L) - 9.0909090909090909) > ZERO_APPROX)
      testCase1.AddFailure("Compartment 1 concentration after instant diffusion not as expected.");
    if (std::abs(cmpt2.GetSubstanceQuantity(*o2)->GetConcentration(MassPerVolumeUnit::g_Per_L) - 9.0909090909090909) > ZERO_APPROX)
      testCase1.AddFailure("Compartment 2 concentration after instant diffusion not as expected.");

    testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

    /// \todo This methodology assumes binary compartments. It works in prototype for multiple compartment instant diffusion, but needs to be tested beyond two compartments.
  }

  void EngineTest::SimpleDiffusionTwoCompartmentTest(const std::string& rptDirectory)
  {
    // Second test case -- 
    // Tests the diffusion between two tissue liquid compartments
    // Calculated permeability coefficient
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* o2 = pc.GetSubstances().GetSubstance("Oxygen");
    pc.GetSubstances().AddActiveSubstance(*o2);
    SETissueCompartment& tissue = pc.GetCompartments().CreateTissueCompartment("Tissue");
    SELiquidCompartment& cmpt1_IC = pc.GetCompartments().CreateLiquidCompartment("cmpt1_IC");
    SELiquidCompartment& cmpt2_EC = pc.GetCompartments().CreateLiquidCompartment("cmpt2_EC");

    DataTrack trk2;
    std::string rptFile = rptDirectory + "/SimpleDiffusionTwoCompartmentTest.csv";
    double time = 0.0;

    double ecVol_mL = 40.0;
    double icVol_mL = 30.0;
    double ecMass_g = 1.0;
    double icMass_g = 20.0;

    cmpt2_EC.GetVolume().SetValue(ecVol_mL, VolumeUnit::mL);
    cmpt1_IC.GetVolume().SetValue(icVol_mL, VolumeUnit::mL);
    cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().SetValue(ecMass_g, MassUnit::g);
    cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().SetValue(icMass_g, MassUnit::g);
    cmpt2_EC.Balance(BalanceLiquidBy::Mass);
    cmpt1_IC.Balance(BalanceLiquidBy::Mass);

    tissue.GetTotalMass().SetValue(500.0, MassUnit::g);

    double molarMass = o2->GetMolarMass().GetValue(MassPerAmountUnit::g_Per_mol);
    double molecularRadius = 0.0348 * pow(molarMass, 0.4175);
    double permeabilityCoefficient_mL_Per_s_hg;
    if (molecularRadius > 1.0)
    {
      permeabilityCoefficient_mL_Per_s_hg = 0.0287 * pow(molecularRadius, -2.920);
    }
    else
    {
      permeabilityCoefficient_mL_Per_s_hg = 0.0184 * pow(molecularRadius, -1.223);
    }

    // The tissue mass baseline is a constant property of the tissue - values can be found in the ICRP and other sources
    // We use the tissue mass as a stand-in for surface area, follow the lead of Renkin and Curry
    // Here are the rules for the different types of compartments
    // Vascular to tissue (in Pulse it is always extracellular, but it doesn't matter)
    // The mass is the tissue compartment mass
    // Tissue to tissue
    // The mass is always the smaller of the two (the smaller area will be the max area for diffusion)
    double permeabilityCoefficient_mL_Per_s = permeabilityCoefficient_mL_Per_s_hg / 100.0 * tissue.GetTotalMass(MassUnit::g);

    trk2.Track("ExtracellularMass_g", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk2.Track("IntracellularMass_g", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk2.Track("ExtracellularConc_g_Per_mL", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk2.Track("IntracellularConc_g_Per_mL", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));

    for (int i = 0; i < 3600; i++)
    {
      tsu.MoveMassBySimpleDiffusion(cmpt1_IC, cmpt2_EC, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
      cmpt2_EC.Balance(BalanceLiquidBy::Mass);
      cmpt1_IC.Balance(BalanceLiquidBy::Mass);
      time += timestep_s;
      ecMass_g = cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g);
      icMass_g = cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g);
      double ecConc_g_Per_mL = cmpt2_EC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
      double icConc_g_Per_mL = cmpt1_IC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
      trk2.Track("ExtracellularMass_g", time, ecMass_g);
      trk2.Track("IntracellularMass_g", time, icMass_g);
      trk2.Track("ExtracellularConc_g_Per_mL", time, ecConc_g_Per_mL);
      trk2.Track("IntracellularConc_g_Per_mL", time, icConc_g_Per_mL);
    }

    trk2.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::SimpleDiffusionFourCompartmentTest(const std::string& rptDirectory)
  {
    // Now we will add two more compartments and test two things
    // First, that the concentration does not change within a time slice
    // Second, that the concentrations equilibrate appropriately
    //              / cmpt1
    //     cmpt4 <->  cmpt2
    //              \ cmpt3
    // Artificial permeability coefficient
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* o2 = pc.GetSubstances().GetSubstance("Oxygen");
    pc.GetSubstances().AddActiveSubstance(*o2);
    pc.GetCompartments().CreateTissueCompartment("Tissue");
    SELiquidCompartment& cmpt1_IC = pc.GetCompartments().CreateLiquidCompartment("cmpt1_IC");
    SELiquidCompartment& cmpt2_EC = pc.GetCompartments().CreateLiquidCompartment("cmpt2_EC");
    SELiquidCompartment& cmpt3_LQ = pc.GetCompartments().CreateLiquidCompartment("cmpt3_LQ");
    SELiquidCompartment& cmpt4_LQ = pc.GetCompartments().CreateLiquidCompartment("cmpt4_LQ");

    DataTrack trk3;
    std::string rptFile = rptDirectory + "/SimpleDiffusionFourCompartmentTest.csv";
    double time = 0.0;

    // Initialize
    double v3Mass_g = 10.0;
    double v4Mass_g = 0.0;
    double v3Vol_mL = 20.0;
    double v4Vol_mL = 50.0;
    double ecVol_mL = 10.0;
    double icVol_mL = 50.0;
    double ecMass_g = 2.5;
    double icMass_g = 2.0;
    cmpt2_EC.GetVolume().SetValue(ecVol_mL, VolumeUnit::mL);
    cmpt1_IC.GetVolume().SetValue(icVol_mL, VolumeUnit::mL);
    cmpt3_LQ.GetVolume().SetValue(v3Vol_mL, VolumeUnit::mL);
    cmpt4_LQ.GetVolume().SetValue(v4Vol_mL, VolumeUnit::mL);
    cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().SetValue(ecMass_g, MassUnit::g);
    cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().SetValue(icMass_g, MassUnit::g);
    cmpt3_LQ.GetSubstanceQuantity(*o2)->GetMass().SetValue(v3Mass_g, MassUnit::g);
    cmpt4_LQ.GetSubstanceQuantity(*o2)->GetMass().SetValue(v4Mass_g, MassUnit::g);
    cmpt4_LQ.Balance(BalanceLiquidBy::Mass);
    cmpt3_LQ.Balance(BalanceLiquidBy::Mass);
    cmpt2_EC.Balance(BalanceLiquidBy::Mass);
    cmpt1_IC.Balance(BalanceLiquidBy::Mass);

    // Make it a little faster for this test
    double permeabilityCoefficient_mL_Per_s = 20.0;

    tsu.MoveMassBySimpleDiffusion(cmpt1_IC, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
    tsu.MoveMassBySimpleDiffusion(cmpt2_EC, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
    tsu.MoveMassBySimpleDiffusion(cmpt3_LQ, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);

    // Ok, now balance
    cmpt4_LQ.Balance(BalanceLiquidBy::Mass);
    cmpt3_LQ.Balance(BalanceLiquidBy::Mass);
    cmpt2_EC.Balance(BalanceLiquidBy::Mass);
    cmpt1_IC.Balance(BalanceLiquidBy::Mass);
    trk3.Track("cmpt1mass_g", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk3.Track("cmpt2mass_g", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk3.Track("cmpt3mass_g", time, cmpt3_LQ.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk3.Track("cmpt4mass_g", time, cmpt4_LQ.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
    trk3.Track("cmpt1conc_g_Per_mL", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk3.Track("cmpt2conc_g_Per_mL", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk3.Track("cmpt3conc_g_Per_mL", time, cmpt3_LQ.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk3.Track("cmpt4conc_g_Per_mL", time, cmpt4_LQ.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));

    for (int i = 0; i < 3600; i++)
    {
      tsu.MoveMassBySimpleDiffusion(cmpt1_IC, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
      tsu.MoveMassBySimpleDiffusion(cmpt2_EC, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
      tsu.MoveMassBySimpleDiffusion(cmpt3_LQ, cmpt4_LQ, *o2, permeabilityCoefficient_mL_Per_s, timestep_s);
      cmpt4_LQ.Balance(BalanceLiquidBy::Mass);
      cmpt3_LQ.Balance(BalanceLiquidBy::Mass);
      cmpt2_EC.Balance(BalanceLiquidBy::Mass);
      cmpt1_IC.Balance(BalanceLiquidBy::Mass);
      time += timestep_s;
      trk3.Track("cmpt1mass_g", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
      trk3.Track("cmpt2mass_g", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
      trk3.Track("cmpt3mass_g", time, cmpt3_LQ.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
      trk3.Track("cmpt4mass_g", time, cmpt4_LQ.GetSubstanceQuantity(*o2)->GetMass().GetValue(MassUnit::g));
      trk3.Track("cmpt1conc_g_Per_mL", time, cmpt1_IC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk3.Track("cmpt2conc_g_Per_mL", time, cmpt2_EC.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk3.Track("cmpt3conc_g_Per_mL", time, cmpt3_LQ.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk3.Track("cmpt4conc_g_Per_mL", time, cmpt4_LQ.GetSubstanceQuantity(*o2)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    }

    // Write data to file
    trk3.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::SimpleDiffusionHierarchyTest(const std::string& rptDirectory)
  {
    // Tests diffusion with distribution for hierarchical compartments                                           //
    //                                       L0C0        <---->        M0C0                                      //
    //                                      /    \                    /  |  \                                    //
    //                                  L1C0      L1C1            M1C0  M1C1 M1C2  <--Only these cmpts have data //
    //                                  /  \      /  \                                                           //
    // Only these cmpts have data--> L2C0  L2C1 L2C2 L2C3                                                        //

    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* sub = pc.GetSubstances().GetSubstance("Desflurane");
    pc.GetSubstances().AddActiveSubstance(*sub);

    DataTrack trk4;
    std::string rptFile = rptDirectory + "/SimpleDiffusionHierarchyTest.csv";
    double time = 0.0;

    SELiquidCompartment& L0C0 = pc.GetCompartments().CreateLiquidCompartment("L0C0");
    SELiquidCompartment& L1C0 = pc.GetCompartments().CreateLiquidCompartment("L1C0");
    SELiquidCompartment& L1C1 = pc.GetCompartments().CreateLiquidCompartment("L1C1");
    SELiquidCompartment& L2C0 = pc.GetCompartments().CreateLiquidCompartment("L2C0");
    SELiquidCompartment& L2C1 = pc.GetCompartments().CreateLiquidCompartment("L2C1");
    SELiquidCompartment& L2C2 = pc.GetCompartments().CreateLiquidCompartment("L2C2");
    SELiquidCompartment& L2C3 = pc.GetCompartments().CreateLiquidCompartment("L2C3");
    SELiquidCompartment& M0C0 = pc.GetCompartments().CreateLiquidCompartment("M0C0");
    SELiquidCompartment& M1C0 = pc.GetCompartments().CreateLiquidCompartment("M1C0");
    SELiquidCompartment& M1C1 = pc.GetCompartments().CreateLiquidCompartment("M1C1");
    SELiquidCompartment& M1C2 = pc.GetCompartments().CreateLiquidCompartment("M1C2");

    // Build up the hierarchy
    L0C0.AddChild(L1C0);
    L0C0.AddChild(L1C1);
    L1C0.AddChild(L2C0);
    L1C0.AddChild(L2C1);
    L1C1.AddChild(L2C2);
    L1C1.AddChild(L2C3);
    M0C0.AddChild(M1C0);
    M0C0.AddChild(M1C1);
    M0C0.AddChild(M1C2);
    pc.GetCompartments().StateChange();// Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

    double Lvol_mL = 10.0;
    double Mvol_mL = 8.0;
    double M1C2mol_fraction = 0.01;
    L2C0.GetVolume().SetValue(Lvol_mL, VolumeUnit::mL);
    L2C1.GetVolume().SetValue(Lvol_mL, VolumeUnit::mL);
    L2C2.GetVolume().SetValue(Lvol_mL, VolumeUnit::mL);
    L2C3.GetVolume().SetValue(Lvol_mL, VolumeUnit::mL);
    M1C0.GetVolume().SetValue(Mvol_mL, VolumeUnit::mL);
    M1C1.GetVolume().SetValue(Mvol_mL, VolumeUnit::mL);
    M1C2.GetVolume().SetValue(Mvol_mL, VolumeUnit::mL);

    double Lmolarity_mmol_Per_mL = 10.0;
    double Mmolarity_mmol_Per_mL = 0.0;
    L2C0.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(Lmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    L2C1.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(0.5 * Lmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    L2C2.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(2.0 * Lmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    L2C3.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(1.5 * Lmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    M1C0.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(Mmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    M1C1.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(Mmolarity_mmol_Per_mL, AmountPerVolumeUnit::mmol_Per_mL);
    M1C2.GetSubstanceQuantity(*sub)->GetMolarity().SetValue(Mmolarity_mmol_Per_mL * M1C2mol_fraction, AmountPerVolumeUnit::mmol_Per_mL); // He has less moles

    L2C0.Balance(BalanceLiquidBy::Molarity);
    L2C1.Balance(BalanceLiquidBy::Molarity);
    L2C2.Balance(BalanceLiquidBy::Molarity);
    L2C3.Balance(BalanceLiquidBy::Molarity);
    M1C0.Balance(BalanceLiquidBy::Molarity);
    M1C1.Balance(BalanceLiquidBy::Molarity);
    M1C2.Balance(BalanceLiquidBy::Molarity);

    double permeabilityCoefficient_mL_Per_s = 2.0;

    trk4.Track("L0C0_mass_g", time, L0C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L1C0_mass_g", time, L1C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L1C1_mass_g", time, L1C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L2C0_mass_g", time, L2C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L2C1_mass_g", time, L2C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L2C2_mass_g", time, L2C2.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L2C3_mass_g", time, L2C3.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("M0C0_mass_g", time, M0C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("M1C0_mass_g", time, M1C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("M1C1_mass_g", time, M1C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("M1C2_mass_g", time, M1C2.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
    trk4.Track("L0C0_conc_g_Per_mL", time, L0C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L1C0_conc_g_Per_mL", time, L1C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L1C1_conc_g_Per_mL", time, L1C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L2C0_conc_g_Per_mL", time, L2C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L2C1_conc_g_Per_mL", time, L2C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L2C2_conc_g_Per_mL", time, L2C2.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("L2C3_conc_g_Per_mL", time, L2C3.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("M0C0_conc_g_Per_mL", time, M0C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("M1C0_conc_g_Per_mL", time, M1C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("M1C1_conc_g_Per_mL", time, M1C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    trk4.Track("M1C2_conc_g_Per_mL", time, M1C2.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));

    for (int i = 0; i < 3600; i++)
    {
      tsu.MoveMassBySimpleDiffusion(L0C0, M0C0, *sub, permeabilityCoefficient_mL_Per_s, timestep_s);
      L2C0.Balance(BalanceLiquidBy::Mass);
      L2C1.Balance(BalanceLiquidBy::Mass);
      L2C2.Balance(BalanceLiquidBy::Mass);
      L2C3.Balance(BalanceLiquidBy::Mass);
      M1C0.Balance(BalanceLiquidBy::Mass);
      M1C1.Balance(BalanceLiquidBy::Mass);
      M1C2.Balance(BalanceLiquidBy::Mass);
      time += timestep_s;
      trk4.Track("L0C0_mass_g", time, L0C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L1C0_mass_g", time, L1C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L1C1_mass_g", time, L1C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L2C0_mass_g", time, L2C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L2C1_mass_g", time, L2C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L2C2_mass_g", time, L2C2.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L2C3_mass_g", time, L2C3.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("M0C0_mass_g", time, M0C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("M1C0_mass_g", time, M1C0.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("M1C1_mass_g", time, M1C1.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("M1C2_mass_g", time, M1C2.GetSubstanceQuantity(*sub)->GetMass().GetValue(MassUnit::g));
      trk4.Track("L0C0_conc_g_Per_mL", time, L0C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L1C0_conc_g_Per_mL", time, L1C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L1C1_conc_g_Per_mL", time, L1C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L2C0_conc_g_Per_mL", time, L2C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L2C1_conc_g_Per_mL", time, L2C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L2C2_conc_g_Per_mL", time, L2C2.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("L2C3_conc_g_Per_mL", time, L2C3.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("M0C0_conc_g_Per_mL", time, M0C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("M1C0_conc_g_Per_mL", time, M1C0.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("M1C1_conc_g_Per_mL", time, M1C1.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
      trk4.Track("M1C2_conc_g_Per_mL", time, M1C2.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL));
    }

    trk4.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::FacilitatedDiffusionTest(const std::string& rptDirectory)
  {
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    TissueModel& tsu = (TissueModel&)pc.GetTissue();
    double timestep_s = 1.0 / 90;
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* sub = pc.GetSubstances().GetSubstance("Glucose");
    pc.GetSubstances().AddActiveSubstance(*sub);
    pc.GetCompartments().CreateTissueCompartment("Tissue");
    SELiquidCompartment& tissueExtracellular = pc.GetCompartments().CreateLiquidCompartment("Extracellular");
    SELiquidCompartment& vascular = pc.GetCompartments().CreateLiquidCompartment("Vascular");

    DataTrack trk1;
    std::string   rptFile = rptDirectory + "/FacilitatedDiffusionTest.csv";
    double time = 0.0;

    tissueExtracellular.GetVolume().SetValue(45.0, VolumeUnit::mL);
    vascular.GetVolume().SetValue(100.0, VolumeUnit::mL);
    tissueExtracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(2.9, MassUnit::g);
    vascular.GetSubstanceQuantity(*sub)->GetMass().SetValue(5.0, MassUnit::g);
    tissueExtracellular.Balance(BalanceLiquidBy::Mass);
    vascular.Balance(BalanceLiquidBy::Mass);

    double capCoverage_cm2 = 90.0; /// \todo Relate capillary coverage to tissue mass.
    double maximumMassFlux = sub->GetMaximumDiffusionFlux(MassPerAreaTimeUnit::g_Per_cm2_s);
    double combinedCoefficient_g_Per_s = maximumMassFlux * capCoverage_cm2;

    double vascularConc_g_Per_mL = vascular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
    double ECconc_g_Per_mL = tissueExtracellular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
    trk1.Track("VascularConc_g_Per_mL", time, vascularConc_g_Per_mL);
    trk1.Track("ExtracellularConc_g_Per_mL", time, ECconc_g_Per_mL);

    for (int i = 0; i < 180; i++)
    {
      tsu.MoveMassByFacilitatedDiffusion(vascular, tissueExtracellular, *sub, combinedCoefficient_g_Per_s, timestep_s);
      vascular.Balance(BalanceLiquidBy::Mass);
      tissueExtracellular.Balance(BalanceLiquidBy::Mass);
      time += timestep_s;
      vascularConc_g_Per_mL = vascular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
      ECconc_g_Per_mL = tissueExtracellular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
      trk1.Track("VascularConc_g_Per_mL", time, vascularConc_g_Per_mL);
      trk1.Track("ExtracellularConc_g_Per_mL", time, ECconc_g_Per_mL);
    }

    trk1.WriteTrackToFile(rptFile.c_str());
  }

  void EngineTest::ActiveTransportTest(SETestSuite& testSuite)
  {
    TimingProfile timer;
    Engine pe;
    Controller& pc = (Controller&)pe.GetController();
    pc.GetSubstances().LoadSubstanceDirectory();
    SESubstance* sub = pc.GetSubstances().GetSubstance("Desflurane");
    pc.GetSubstances().AddActiveSubstance(*sub);
    pc.GetCompartments().CreateTissueCompartment("Tissue");
    SELiquidCompartment& extracellular = pc.GetCompartments().CreateLiquidCompartment("Extracellular");
    SELiquidCompartment& intracellular = pc.GetCompartments().CreateLiquidCompartment("Intracellular");

    // First test case - pump some substance from one cmpt to another to another
    SETestCase& testCase1 = testSuite.CreateTestCase();
    testCase1.SetName("ActivePumpTest");
    timer.Start("Test");

    double ecVol_mL = 20.0;
    double icVol_mL = 50.0;
    double ecMass_g = 1.5;
    double icMass_g = 2.0;

    extracellular.GetVolume().SetValue(ecVol_mL, VolumeUnit::mL);
    intracellular.GetVolume().SetValue(icVol_mL, VolumeUnit::mL);
    extracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(icMass_g, MassUnit::g);
    intracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(ecMass_g, MassUnit::g);
    extracellular.Balance(BalanceLiquidBy::Mass);
    intracellular.Balance(BalanceLiquidBy::Mass);

    testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

    // Second test should check bounds.
  }

  void EngineTest::GenericClearanceTest(SETestSuite& testSuite)
  {
    TimingProfile timer;
    Engine pe;

    // First test case
    SETestCase& testCase1 = testSuite.CreateTestCase();
    testCase1.SetName("GenericClearanceTestLiquid");
    // You at least need 2 test cases here (liquid and tissue)
    testSuite.CreateTestCase();
    timer.Start("Test");
    // Test Logic
    //testCase.GetFailures().push_back("Report any errors like this");
    // Log how long our test took
    testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

    // Second test case
    SETestCase& testCase2 = testSuite.CreateTestCase();
    testCase2.SetName("GenericClearanceTestTissue");
    timer.Start("Test");

    testCase2.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);
    // Subsequent test cases
  }

  void EngineTest::GenericExcretionTest(SETestSuite& testSuite)
  {
    TimingProfile timer;
    Engine pe;

    // First test case
    SETestCase& testCase1 = testSuite.CreateTestCase();
    testCase1.SetName("GenericExcretionTest");
    timer.Start("Test");
    // Test Logic
    //testCase.GetFailures().push_back("Report any errors like this");
    // Log how long our test took
    testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);
  }

  //Set-up the test suite
  void EngineTest::DiffusionClearanceExcretionTests(const std::string& rptDirectory)
  {

    m_Logger->SetLogFile(rptDirectory + "/DiffusionClearanceExcretionTests.log");
    // Set up our test report

    SETestReport testReport = SETestReport(m_Logger);

    SETestSuite& tsDistMass = testReport.CreateTestSuite();
    tsDistMass.SetName("DistributeMass");
    DistributeMass(tsDistMass);

    SETestSuite& ts1 = testReport.CreateTestSuite();
    ts1.SetName("PerfusionLimitedDiffusionTest");
    PerfusionLimitedDiffusionTest(ts1);

    //SETestSuite& ts5 = testReport.CreateTestSuite();
    //ts5.SetName("ActiveTransportTest");
    //ActiveTransportTest(ts5, rptDirectory);

    SETestSuite& ts6 = testReport.CreateTestSuite();
    ts6.SetName("InstantDiffusionTest");
    InstantDiffusionTest(ts6);

    //SETestSuite& ts7 = testReport.CreateTestSuite();
    //ts6.SetName("GenericClearanceTest");
    //GenericClearanceTest(ts7);

    //SETestSuite& ts8 = testReport.CreateTestSuite();
    //ts7.SetName("GenericExcretionTest");
    //GenericExcretionTest(ts8);

    //testReport.WriteFile(rptDirectory + "/GasCompartmentTest.json");
  }
END_NAMESPACE_EX
