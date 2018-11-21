/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"
#include "controller/Controller.h"
#include "controller/Circuits.h"
#include "physiology/Cardiovascular.h"
#include "PulseConfiguration.h"

#include "patient/SEPatient.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "substance/SESubstanceFraction.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarFlowResistance.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarTime.h"
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

void CalculateMultipliers(double& dsResistanceMultiplier, double& usResistanceMultiplier, const double& map_mmHg, const double& cvp_mmHg, const SEFluidCircuitPath* brainResistanceDownstream, const SEFluidCircuitPath* brainResistanceUpstream)
{
  //The brain portion of the CV circuit at steady state can be approximated as:
  //
  //  ----R2----R1----
  //  |              |
  //  +              +
  // CVP            MAP
  //  -              -
  //  |              |
  //  ----------------
  //
  //We can use the tuned CVP, MAP, and R values to determine multipliers needed to hit 25 mmHg for ICP and 1.8 mL/s for CBF

  double totalPressure = map_mmHg - cvp_mmHg;
  double i = 1.8; //The desired CBF for highest injury
  double v2 = 22; //The desired ICP for highest injury (we really want 25, but it tends to overshoot a bit)
  double v1 = totalPressure - v2;
  double r1 = brainResistanceUpstream->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  double r2 = brainResistanceDownstream->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  double newr1 = v1 / i;
  double newr2 = v2 / i;

  dsResistanceMultiplier = newr2 / r2;
  usResistanceMultiplier = newr1 / r1;

}

void PulseEngineTest::BrainInjuryTest(const std::string& sTestDirectory)
{
  std::string tName = "BrainInjuryTest";

  DataTrack outTrk;
  std::ofstream file;

  PulseController pc(sTestDirectory + "/" + tName + ".log");
  pc.GetLogger()->Info("Running " + tName);
  pc.GetPatient().SerializeFromFile("./patients/StandardMale.json",JSON);
  pc.SetupPatient();

  //Renal and Tissue are on
  pc.m_Config->EnableRenal(eSwitch::On);
  pc.m_Config->EnableTissue(eSwitch::On);
  pc.CreateCircuitsAndCompartments();

  Cardiovascular& cv = (Cardiovascular&)pc.GetCardiovascular();
  cv.m_TuneCircuit = true;
  SEFluidCircuit& cvCircuit = pc.GetCircuits().GetActiveCardiovascularCircuit();

  cv.Initialize();

  double time_s = 0.0;
  double testTime_s = 800;
  double timeStep_s = 1.0 / 90;
  double dsResistanceMultiplier = 1;
  double usResistanceMultiplier = 1;
  double complianceMultiplier = 1;  //Can change this if we find supporting sources, but note that it will increase "spikiness" of plots

  SEFluidCircuitNode* brain = cvCircuit.GetNode(pulse::CardiovascularNode::Brain1);
  SEFluidCircuitPath* brainResistanceDownstream = cvCircuit.GetPath(pulse::CardiovascularPath::Brain1ToBrain2);
  SEFluidCircuitPath* brainResistanceUpstream = cvCircuit.GetPath(pulse::CardiovascularPath::Aorta1ToBrain1);
  SEFluidCircuitPath* brainCompliance = cvCircuit.GetPath(pulse::CardiovascularPath::Brain1ToGround);

  double map_mmHg = cv.GetMeanArterialPressure(PressureUnit::mmHg);
  double cvp_mmHg = cv.GetCentralVenousPressure(PressureUnit::mmHg);

  CalculateMultipliers(dsResistanceMultiplier, usResistanceMultiplier, map_mmHg, cvp_mmHg, brainResistanceDownstream, brainResistanceUpstream);

  std::stringstream ss;
  ss << "Downstream brain resistance multiplier is: " << dsResistanceMultiplier << ". Upstream resistance multiplier is: " << usResistanceMultiplier;
  pc.GetLogger()->Info(ss);

  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, pc.GetLogger());

  for (unsigned int i = 0; i < (testTime_s / timeStep_s); i++)
  {
    cv.HeartDriver();

    if (time_s > 1 * 60)
    {
      brainResistanceDownstream->GetNextResistance().SetValue(dsResistanceMultiplier * brainResistanceDownstream->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
      brainResistanceUpstream->GetNextResistance().SetValue(usResistanceMultiplier * brainResistanceUpstream->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
      brainCompliance->GetNextCompliance().SetValue(complianceMultiplier * brainCompliance->GetComplianceBaseline().GetValue(FlowComplianceUnit::mL_Per_mmHg), FlowComplianceUnit::mL_Per_mmHg);
    }

    calc.Process(cvCircuit, timeStep_s);//Process - Execute the circuit  

    cv.CalculateVitalSigns();
    calc.PostProcess(cvCircuit);//convert 'Next' values to current 

    map_mmHg = cv.GetMeanArterialPressure(PressureUnit::mmHg);
    double sys_mmHg = cv.GetSystolicArterialPressure(PressureUnit::mmHg);
    double dia_mmHg = cv.GetDiastolicArterialPressure(PressureUnit::mmHg);
    double hr = cv.GetHeartRate(FrequencyUnit::Per_min);

    outTrk.Track("MeanArterialPressure(mmHg)", time_s, map_mmHg);
    outTrk.Track("SystolicArterialPressure(mmHg)", time_s, sys_mmHg);
    outTrk.Track("DiastolicArterialPressure(mmHg)", time_s, dia_mmHg);
    outTrk.Track("HeartRate(Per_Min)", time_s, hr);
    outTrk.Track("BrainResistanceDownstream(mmHg_s_Per_mL)", time_s, brainResistanceDownstream->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL));
    outTrk.Track("BrainResistanceUpstream(mmHg_s_Per_mL)", time_s, brainResistanceUpstream->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL));
    outTrk.Track("BrainCompliance(mL_Per_mmHg)", time_s, brainCompliance->GetCompliance(FlowComplianceUnit::mL_Per_mmHg));
    outTrk.Track("BrainVolume(mL)", time_s, brain->GetVolume(VolumeUnit::mL));
    outTrk.Track("IntracranialPressure(mmHg)", time_s, brain->GetPressure(PressureUnit::mmHg));
    outTrk.Track("CerebralBloodFlow(mL_Per_s)", time_s, brainResistanceDownstream->GetFlow(VolumePerTimeUnit::mL_Per_s));
    outTrk.Track("CerebralPerfusionPressure(mmHg)", time_s, map_mmHg - brain->GetPressure(PressureUnit::mmHg));

    if (i == 0)
    {
      outTrk.CreateFile(std::string(sTestDirectory + "/" + tName + ".csv").c_str(), file);
    }

    time_s += timeStep_s;

  }
  outTrk.StreamTrackToFile(file);
}

