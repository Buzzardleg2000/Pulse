/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "blackbox/SEBlackBoxManager.h"
#include "blackbox/fluid/SEFluidBlackBox.h"

#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "circuit/SECircuitManager.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "utils/DataTrack.h"
#include "utils/TimingProfile.h"

void CommonDataModelTest::BasicBlackBoxComparisonTest(const std::string& sOutputDirectory)
{
  std::cout << "BasicBlackBoxComparisonTest\n";

  m_Logger->SetLogFile(sOutputDirectory + "/BasicBlackBoxComparisonTest.log");
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
  DataTrack trk2;
  SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

  //Test Circuit
  SEFluidCircuit* fluidCircuit = &m_Circuits->CreateFluidCircuit("Fluid");
  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit->CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit->CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit->CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit->CreateNode("Node4");
  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit->AddNode(Node4); Node4.SetAsReferenceNode();
  Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  SEFluidCircuitPath& Path2 = fluidCircuit->CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit->CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit->CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit->CreatePath(Node2, Node4, "Path5");
  Path5.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit->CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
  fluidCircuit->StateChange();

  bool serialized = false;
  double sample = 0;
  while (currentTime_s < 100)
  {
    //PreProcess - to be done by the systems
    FluidPreProcess(*fluidCircuit, currentTime_s);
    //Process
    fluidCalculator.Process(*fluidCircuit, timeStep_s);
    //PostProcess
    fluidCalculator.PostProcess(*fluidCircuit);
    currentTime_s += timeStep_s;
    sample += timeStep_s;
    if (sample > 0.1)// every 0.1 seconds, track state of circuit
    {
      sample = 0;
      trk1.Track(currentTime_s, *fluidCircuit);
    }
    if (!serialized && currentTime_s > 80)
    {
      serialized = true;
      std::string jsonDir = sOutputDirectory + "/BasicBlackBoxComparisonSerializationTest.json";
      TestCircuitSerialization(jsonDir);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
  }
  std::string sOutputFile = sOutputDirectory + "/BasicBlackBoxComparisonTest.csv";
  trk1.WriteTrackToFile(sOutputFile.c_str());
  m_Circuits->Clear();
}

void CommonDataModelTest::BasicBlackBoxTest(const std::string& sOutputDirectory)
{
  std::cout << "BasicBlackBoxTest\n";
  m_Logger->SetLogFile(sOutputDirectory + "/BasicBlackBoxTest.log");
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
  DataTrack trk2;

  SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

  //Circuit 1
  SEFluidCircuit* fluidCircuit1 = &m_Circuits->CreateFluidCircuit("fluidCircuit1");

  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit1->CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit1->CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit1->CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit1->CreateNode("Node4");

  Node4.SetAsReferenceNode();
  Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);

  //Paths
  SEFluidCircuitPath& Path2 = fluidCircuit1->CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit1->CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit1->CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit1->CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  //Circuit 2
  SEFluidCircuit* fluidCircuit2 = &m_Circuits->CreateFluidCircuit("fluidCircuit2");

  SEFluidCircuitNode& Node5 = fluidCircuit2->CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit2->CreateNode("Node6");

  Node6.SetAsReferenceNode();
  Node6.GetNextPressure().SetValue(0, PressureUnit::Pa);

  SEFluidCircuitPath& Path5 = fluidCircuit2->CreatePath(Node5, Node6, "Path5");
  Path5.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Black Boxes
  SEBlackBoxManager BBmgr(m_Logger);

  //Black Box Elements
  SEFluidCircuitNode& BB1Node = fluidCircuit1->CreateNode("BB1Node");
  SEFluidCircuitPath& BB1Path1 = fluidCircuit1->CreatePath(Node2, BB1Node, "BB1Path1");
  SEFluidCircuitPath& BB1Path2 = fluidCircuit1->CreatePath(BB1Node, Node4, "BB1Path2");
  SEFluidBlackBox& BlackBox1 = *BBmgr.GetLiquidBlackBox("Node2", "Node4", "BB1");
  BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(BlackBox1, BB1Path1, BB1Path2);

  SEFluidCircuitNode& BB2Node = fluidCircuit2->CreateNode("BB2Node");
  SEFluidCircuitPath& BB2Path1 = fluidCircuit2->CreatePath(Node6, BB2Node, "BB2Path1");
  SEFluidCircuitPath& BB2Path2 = fluidCircuit2->CreatePath(BB2Node, Node5, "BB2Path2");
  SEFluidBlackBox& BlackBox2 = *BBmgr.GetLiquidBlackBox("Node6", "Node5", "BB2");
  BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(BlackBox2, BB2Path1, BB2Path2);

  fluidCircuit1->StateChange();
  fluidCircuit2->StateChange();

  Node2.GetPressure().SetValue(0.0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0.0, PressureUnit::Pa);
  BB2Path2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);

  //Run -----------------------------------------------------------------------------
  double sample = 0;
  while (currentTime_s < 100)
  {
    //Black Box settings ------------------------------------------------------------
    //Imposed = black box forces it on circuit
    //BlackBox2.ImposeSourcePotential(Node4.GetPressure()); //Not on a reference node
    BlackBox2.ImposeTargetPotential(Node2.GetPressure());
    BlackBox1.ImposeSourceFlux(BB2Path2.GetFlow());
    BlackBox1.ImposeTargetFlux(BB2Path2.GetFlow());

    //Circuit1 has the source
    FluidPreProcess(*fluidCircuit1, currentTime_s);

    fluidCalculator.Process(*fluidCircuit1, timeStep_s);
    fluidCalculator.Process(*fluidCircuit2, timeStep_s);

    fluidCalculator.PostProcess(*fluidCircuit1);
    fluidCalculator.PostProcess(*fluidCircuit2);

    currentTime_s += timeStep_s;
    sample += timeStep_s;
    if (sample > 0.1) //every 0.1 seconds, track state of circuit
    {
      sample = 0;
      trk1.Track(currentTime_s, *fluidCircuit1);
      trk2.Track(currentTime_s, *fluidCircuit2);
    }
  }

  std::string sOutputFile1 = sOutputDirectory + "/BasicBlackBoxTestCircuit1.csv";
  std::string sOutputFile2 = sOutputDirectory + "/BasicBlackBoxTestCircuit2.csv";
  trk1.WriteTrackToFile(sOutputFile1.c_str());
  trk2.WriteTrackToFile(sOutputFile2.c_str());
  m_Circuits->Clear();
}

void CommonDataModelTest::WindkesselBlackBoxTest(const std::string& sOutputDirectory)
{
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  DataTrack trkWindkessel;
  DataTrack trkDriver;
  DataTrack trkCombined;
  std::cout << "WindkesselBlackBoxTest\n";
  m_Logger->SetLogFile(sOutputDirectory + "/WindkesselBlackBoxTest.log");
  m_Logger->LogToConsole(true);

  for (unsigned int iter = 0; iter < (1 << 8); ++iter)
  {
    bool isCombined = (iter == 0) ? true : false;

    bool imposeWindkesselSourcePressure = iter & 0x001;
    bool imposeWindkesselTargetPressure = iter & 0x002;
    bool imposeWindkesselSourceFlow = iter & 0x004;
    bool imposeWindkesselTargetFlow = iter & 0x008;

    bool imposeDriverSourcePressure = iter & 0x010;
    bool imposeDriverTargetPressure = iter & 0x020;
    bool imposeDriverSourceFlow = iter & 0x040;
    bool imposeDriverTargetFlow = iter & 0x080;

    bool isValid = int(imposeWindkesselSourcePressure) + int(imposeWindkesselSourceFlow) == 1 &&
      int(imposeWindkesselTargetPressure) + int(imposeWindkesselTargetFlow) == 1 &&
      int(imposeDriverSourcePressure) + int(imposeDriverSourceFlow) == 1 &&
      int(imposeDriverTargetPressure) + int(imposeDriverTargetFlow) == 1;

    //only use valid combinations
    if (!isCombined && !isValid)
    {
      continue;
    }

    SEBlackBoxManager managerBlackBox(m_Logger);
    SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

    SEFluidCircuit* circuitCombined = &m_Circuits->CreateFluidCircuit("circuitCombined");
    SEFluidCircuit* circuitWindkessel = &m_Circuits->CreateFluidCircuit("circuitWindkessel");
    SEFluidCircuit* circuitDriver = &m_Circuits->CreateFluidCircuit("circuitDriver");

    //Windkessel Circuit
    //Nodes
    SEFluidCircuitNode& nodeWindkesselResistance1 = circuitWindkessel->CreateNode("nodeWindkesselResistance1");
    SEFluidCircuitNode& nodeWindkesselResistance2 = circuitWindkessel->CreateNode("nodeWindkesselResistance2");
    SEFluidCircuitNode& nodeWindkesselCompliance = circuitWindkessel->CreateNode("nodeWindkesselCompliance");
    nodeWindkesselCompliance.GetPressure().SetValue(0, PressureUnit::Pa);
    nodeWindkesselCompliance.GetVolume().SetValue(0.0, VolumeUnit::m3);
    SEFluidCircuitNode& nodeWindkesselGround = circuitWindkessel->CreateNode("nodeWindkesselGround");
    nodeWindkesselGround.SetAsReferenceNode();
    nodeWindkesselGround.GetPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathWindkesselResistance1 = circuitWindkessel->CreatePath(nodeWindkesselResistance1, nodeWindkesselCompliance, "pathWindkesselResistance1");
    pathWindkesselResistance1.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathWindkesselResistance2 = circuitWindkessel->CreatePath(nodeWindkesselCompliance, nodeWindkesselResistance2, "pathWindkesselResistance2");
    pathWindkesselResistance2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathWindkesselCompliance = circuitWindkessel->CreatePath(nodeWindkesselCompliance, nodeWindkesselGround, "pathWindkesselCompliance");
    pathWindkesselCompliance.GetNextCompliance().SetValue(1.0, VolumePerPressureUnit::m3_Per_Pa);

    //Driver Circuit
    //Nodes
    SEFluidCircuitNode& nodeDriverResistance1 = circuitDriver->CreateNode("nodeDriverResistance1");
    SEFluidCircuitNode& nodeDriverResistance2 = circuitDriver->CreateNode("nodeDriverResistance2");
    SEFluidCircuitNode& nodeDriverSource = circuitDriver->CreateNode("nodeDriverSource");
    SEFluidCircuitNode& nodeDriverGround = circuitDriver->CreateNode("nodeDriverGround");
    nodeDriverGround.SetAsReferenceNode();
    nodeDriverGround.GetPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathDriverResistance1 = circuitDriver->CreatePath(nodeDriverResistance1, nodeDriverSource, "pathDriverResistance1");
    pathDriverResistance1.GetNextResistance().SetValue(12, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverResistance2 = circuitDriver->CreatePath(nodeDriverSource, nodeDriverResistance2, "pathDriverResistance2");
    pathDriverResistance2.GetNextResistance().SetValue(8, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverSource = circuitDriver->CreatePath(nodeDriverGround, nodeDriverSource, "pathDriverSource");
    pathDriverSource.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

    SEFluidBlackBox* blackBoxWindkessel = nullptr;
    SEFluidBlackBox* blackBoxDriver = nullptr;
    if (!isCombined)
    {
      //Replaces the driver
      SEFluidCircuitNode& nodeBlackBoxDriver = circuitWindkessel->CreateNode("nodeBlackBoxDriver");
      SEFluidCircuitPath& pathBlackBoxDriverSource = circuitWindkessel->CreatePath(nodeWindkesselResistance2, nodeBlackBoxDriver, "pathBlackBoxDriverSource");
      SEFluidCircuitPath& pathBlackBoxDriverTarget = circuitWindkessel->CreatePath(nodeBlackBoxDriver, nodeWindkesselResistance1, "pathBlackBoxDriverTarget");
      blackBoxDriver = managerBlackBox.GetLiquidBlackBox("nodeWindkesselResistance2", "nodeWindkesselResistance1", "BlackBoxDriver");
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxDriver, pathBlackBoxDriverSource, pathBlackBoxDriverTarget);

      //Replaces the Windkessel
      SEFluidCircuitNode& nodeBlackBoxWindkessel = circuitDriver->CreateNode("nodeBlackBoxWindkessel");
      SEFluidCircuitPath& pathBlackBoxWindkesselSource = circuitDriver->CreatePath(nodeDriverResistance2, nodeBlackBoxWindkessel, "pathBlackBoxWindkesselSource");
      SEFluidCircuitPath& pathBlackBoxWindkesselTarget = circuitDriver->CreatePath(nodeBlackBoxWindkessel, nodeDriverResistance1, "pathBlackBoxWindkesselTarget");
      blackBoxWindkessel = managerBlackBox.GetLiquidBlackBox("nodeDriverResistance2", "nodeDriverResistance1", "BlackBoxWindkessel");
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxWindkessel, pathBlackBoxWindkesselSource, pathBlackBoxWindkesselTarget);

      // Imposed = black box forces it on circuit
      // Provided = circuit calculates it
      // Reset everything to provided
      blackBoxWindkessel->RemoveImposedPotential();
      blackBoxWindkessel->RemoveImposedSourcePotential();
      blackBoxWindkessel->RemoveImposedTargetPotential();
      blackBoxWindkessel->RemoveImposedSourceFlux();
      blackBoxWindkessel->RemoveImposedTargetFlux();

      blackBoxDriver->RemoveImposedPotential();
      blackBoxDriver->RemoveImposedSourcePotential();
      blackBoxDriver->RemoveImposedTargetPotential();
      blackBoxDriver->RemoveImposedSourceFlux();
      blackBoxDriver->RemoveImposedTargetFlux();

      //Initialize all potential black box inputs
      nodeWindkesselResistance1.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeWindkesselResistance2.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeDriverResistance1.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeDriverResistance2.GetPressure().SetValue(0.0, PressureUnit::Pa);
      pathWindkesselResistance1.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathWindkesselResistance2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathDriverResistance1.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathDriverResistance2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
    }

    circuitWindkessel->StateChange();
    circuitDriver->StateChange();

    if (isCombined)
    {
      //Combined circuit for comparison
      circuitCombined->AddCircuit(*circuitWindkessel);
      circuitCombined->AddCircuit(*circuitDriver);
      circuitCombined->CreatePath(nodeDriverResistance2, nodeWindkesselResistance1, "pathDriverResistance2ToWindkesselResistance1");
      circuitCombined->CreatePath(nodeWindkesselResistance2, nodeDriverResistance1, "pathWindkesselResistance2ToDriverResistance1");
      circuitCombined->StateChange();
    }

    //Run -----------------------------------------------------------------------------
    double currentTime_s = 0.0;
    double sample = 0;
    bool hasError = false;
    while (currentTime_s < 100)
    {
      double dPotential = 20 + 20 * sin(currentTime_s);
      pathDriverSource.GetNextPressureSource().SetValue(dPotential, PressureUnit::Pa);

      if (!isCombined)
      {
        if (imposeDriverSourcePressure)
        {
          blackBoxDriver->ImposeSourcePotential(nodeDriverResistance1.GetPressure());
        }
        if (imposeDriverTargetPressure)
        {
          blackBoxDriver->ImposeTargetPotential(nodeDriverResistance2.GetPressure());
        }
        if (imposeDriverSourceFlow)
        {
          blackBoxDriver->ImposeSourceFlux(pathDriverResistance1.GetFlow());
        }
        if (imposeDriverTargetFlow)
        {
          blackBoxDriver->ImposeTargetFlux(pathDriverResistance2.GetFlow());
        }

        if (imposeWindkesselSourcePressure)
        {
          blackBoxWindkessel->ImposeSourcePotential(nodeWindkesselResistance1.GetPressure());
        }
        if (imposeWindkesselTargetPressure)
        {
          blackBoxWindkessel->ImposeTargetPotential(nodeWindkesselResistance2.GetPressure());
        }
        if (imposeWindkesselSourceFlow)
        {
          blackBoxWindkessel->ImposeSourceFlux(pathWindkesselResistance1.GetFlow());
        }
        if (imposeWindkesselTargetFlow)
        {
          blackBoxWindkessel->ImposeTargetFlux(pathWindkesselResistance2.GetFlow());
        }

        try
        {
          fluidCalculator.Process(*circuitWindkessel, timeStep_s);
          fluidCalculator.Process(*circuitDriver, timeStep_s);
        }
        catch (std::exception& /*ex*/)
        {
          hasError = true;
          break;
        }

        fluidCalculator.PostProcess(*circuitWindkessel);
        fluidCalculator.PostProcess(*circuitDriver);
      }
      else
      {
        fluidCalculator.Process(*circuitCombined, timeStep_s);
        fluidCalculator.PostProcess(*circuitCombined);
      }

      currentTime_s += timeStep_s;
      sample += timeStep_s;
      if (sample > 0.1) //every 0.1 seconds, track state of circuit
      {
        sample = 0;

        if (currentTime_s > timeStep_s)
        {
          if (!isCombined)
          {
            trkDriver.Track(currentTime_s, *circuitDriver);
            trkWindkessel.Track(currentTime_s, *circuitWindkessel);
          }
          else
          {
            trkCombined.Track(currentTime_s, *circuitCombined);
          }
        }
      }
    }

    std::string outputFile;

    if (imposeDriverSourcePressure)
    {
      outputFile += "_imposeDriverSourcePressure";
    }
    if (imposeDriverTargetPressure)
    {
      outputFile += "_imposeDriverTargetPressure";
    }
    if (imposeDriverSourceFlow)
    {
      outputFile += "_imposeDriverSourceFlow";
    }
    if (imposeDriverTargetFlow)
    {
      outputFile += "_imposeDriverTargetFlow";
    }

    if (imposeWindkesselSourcePressure)
    {
      outputFile += "_imposeWindkesselSourcePressure";
    }
    if (imposeWindkesselTargetPressure)
    {
      outputFile += "_imposeWindkesselTargetPressure";
    }
    if (imposeWindkesselSourceFlow)
    {
      outputFile += "_imposeWindkesselSourceFlow";
    }
    if (imposeWindkesselTargetFlow)
    {
      outputFile += "_imposeWindkesselTargetFlow";
    }

    if (hasError)
    {
      m_Logger->Info("Failure: " + outputFile);
      std::stringstream ss;
      ss << "Time(s): " << currentTime_s;
      m_Logger->Info(ss);
      m_Circuits->Clear();
      continue;
    }

    Info("Writing Windkessel for " + outputFile);
    std::string sOutputFileWindkessel = sOutputDirectory + "/BlackBoxWindkessel" + outputFile + ".csv";
    std::string sOutputFileDriver = sOutputDirectory + "/BlackBoxDriver" + outputFile + ".csv";
    std::string sOutputFileCombined = sOutputDirectory + "/BlackBoxWindkesselCombined.csv";

    if (!isCombined)
    {
      trkWindkessel.WriteTrackToFile(sOutputFileWindkessel.c_str());
      trkDriver.WriteTrackToFile(sOutputFileDriver.c_str());
    }
    else
    {
      trkCombined.WriteTrackToFile(sOutputFileCombined.c_str());
    }

    m_Circuits->Clear();
  }
}

void CommonDataModelTest::BlackBoxComplianceTest(const std::string& sOutputDirectory)
{
  for (unsigned int iter = 0; iter < 2; iter++)
  {
    std::string name = "";
    if (iter == 0)
    {
      name = "BlackBoxComplianceTest1";
    }
    else
    {
      name = "BlackBoxComplianceTest2";
    }

    std::cout << name << "\n";
    m_Logger->SetLogFile(sOutputDirectory + "/" + name + ".log");

    TimingProfile p;
    double timeStep_s = 1.0 / 50.0;
    double currentTime_s = 0.0;
    DataTrack trk;

    SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

    //Circuit 1
    SEFluidCircuit* fluidCircuit = &m_Circuits->CreateFluidCircuit("fluidCircuit");

    //Nodes
    SEFluidCircuitNode& Node1 = fluidCircuit->CreateNode("Node1");
    SEFluidCircuitNode& Node2 = fluidCircuit->CreateNode("Node2");
    SEFluidCircuitNode& Node3 = fluidCircuit->CreateNode("Node3");
    SEFluidCircuitNode& Node4 = fluidCircuit->CreateNode("Node4");
    SEFluidCircuitNode& BBNode = fluidCircuit->CreateNode("BBNode");

    Node4.SetAsReferenceNode();
    Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);

    //Pressure source
    SEFluidCircuitPath& Path1 = fluidCircuit->CreatePath(Node4, Node1, "Path1");
    Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
    //Paths
    /*SEFluidCircuitPath& Path2 = */fluidCircuit->CreatePath(Node1, Node2, "Path2");
    SEFluidCircuitPath& Path3 = fluidCircuit->CreatePath(Node2, BBNode, "Path3");
    SEFluidCircuitPath& Path4 = fluidCircuit->CreatePath(BBNode, Node3, "Path4");
    SEFluidCircuitPath& Path5 = fluidCircuit->CreatePath(Node3, Node4, "Path5");
    Path5.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath* Path6;
    if (iter == 0)
    {
      Path6 = &fluidCircuit->CreatePath(Node2, Node4, "Path6");
    }
    else
    {
      Path6 = &fluidCircuit->CreatePath(Node1, Node4, "Path6");
    }
    Path6->GetNextCompliance().SetValue(0.1, VolumePerPressureUnit::m3_Per_Pa);

    //Black Boxes
    SEBlackBoxManager BBmgr(m_Logger);

    //Black Box Elements
    SEFluidBlackBox& BlackBox = *BBmgr.GetLiquidBlackBox("Node2", "Node3", "BlackBox");
    BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(BlackBox, Path3, Path4);

    fluidCircuit->StateChange();

    Node2.GetPressure().SetValue(0.0, PressureUnit::Pa);
    Node3.GetPressure().SetValue(0.0, PressureUnit::Pa);

    double blackBoxResistance_Pa_s_Per_m3 = 10;

    //Run -----------------------------------------------------------------------------
    double sample = 0;
    while (currentTime_s < 20)
    {
      //Black Box settings ------------------------------------------------------------
      //Imposed = black box forces it on circuit
      double flow_m3_Per_s = Node2.GetPressure(PressureUnit::Pa) / blackBoxResistance_Pa_s_Per_m3;
      BlackBox.ImposeSourceFlux(flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);
      BlackBox.ImposeTargetFlux(flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);

      //Circuit1 has the source
      double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
      fluidCircuit->GetPath("Path1")->GetNextPressureSource().SetValue(potential_Pa, PressureUnit::Pa);

      fluidCalculator.Process(*fluidCircuit, timeStep_s);

      fluidCalculator.PostProcess(*fluidCircuit);

      currentTime_s += timeStep_s;
      sample += timeStep_s;
      if (sample > 0.1) //every 0.1 seconds, track state of circuit
      {
        sample = 0;
        trk.Track(currentTime_s, *fluidCircuit);
      }
    }

    std::string sOutputFile = sOutputDirectory + "/" + name + "Circuit.csv";
    trk.WriteTrackToFile(sOutputFile.c_str());
    m_Circuits->Clear();
  }
}

void CommonDataModelTest::BlackBoxSourcesTest(const std::string& sOutputDirectory)
{
  for (unsigned int iterType = 0; iterType < 2; iterType++)
  {
    std::string typeName;
    std::string firstSourceTypeName;
    std::string secondSourceTypeName;
    std::string firstDirectionName;
    std::string secondDirectionName;

    if (iterType == 0)
    {
      typeName = "_Sources";
    }
    else
    {
      typeName = "_BlackBox";
    }

    for (unsigned int iterFirstSourceType = 0; iterFirstSourceType < 3; iterFirstSourceType++)
    {
      if (iterFirstSourceType == 0)
      {
        firstSourceTypeName = "_FlowFirst";
      }
      else if (iterFirstSourceType == 1)
      {
        firstSourceTypeName = "_PressureFirst";
      }
      else
      {
        firstSourceTypeName = "_FlowAndPressureFirst";
      }

      for (unsigned int iterSecondSourceType = 0; iterSecondSourceType < 3; iterSecondSourceType++)
      {
        if (iterSecondSourceType == 0)
        {
          secondSourceTypeName = "_FlowSecond";
        }
        else if (iterSecondSourceType == 1)
        {
          secondSourceTypeName = "_PressureSecond";
        }
        else
        {
          secondSourceTypeName = "_FlowAndPressureSecond";
        }

        double firstDirectionMultiplier = 0.0;
        double secondDirectionMultiplier = 0.0;
        for (unsigned int iterFirstDirection = 0; iterFirstDirection < 2; iterFirstDirection++)
        {
          if (iterFirstDirection == 0)
          {
            firstDirectionName = "_PositiveFirst";
            firstDirectionMultiplier = 1.0;
          }
          else
          {
            firstDirectionName = "_NegativeFirst";
            firstDirectionMultiplier = -1.0;
          }

          for (unsigned int iterSecondDirection = 0; iterSecondDirection < 2; iterSecondDirection++)
          {
            if (iterSecondDirection == 0)
            {
              secondDirectionName = "_PositiveSecond";
              secondDirectionMultiplier = 1.0;
            }
            else
            {
              secondDirectionName = "_NegativeSecond";
              secondDirectionMultiplier = -1.0;
            }

            //For individual testing
            //iterType = 1;
            //iterFirstSourceType = 1;
            //iterSecondSourceType = 1;
            //iterFirstDirection = 0;
            //firstDirectionMultiplier = 1.0;
            //iterSecondDirection = 1;
            //secondDirectionMultiplier = -1.0;

            std::string name = "BlackBoxSourcesTest" + typeName + firstSourceTypeName + secondSourceTypeName + firstDirectionName + secondDirectionName;
            std::cout << name << "\n";
            m_Logger->SetLogFile(sOutputDirectory + "/" + name + ".log");

            TimingProfile p;
            double timeStep_s = 1.0 / 50.0;
            double currentTime_s = 0.0;
            DataTrack trk;

            SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

            //Circuit 1
            SEFluidCircuit* fluidCircuit = &m_Circuits->CreateFluidCircuit("fluidCircuit");

            //Nodes
            SEFluidCircuitNode& Ground = fluidCircuit->CreateNode("Ground");

            SEFluidCircuitNode& Windkessel1Node1 = fluidCircuit->CreateNode("Windkessel1Node1");
            SEFluidCircuitNode& Windkessel1Node2 = fluidCircuit->CreateNode("Windkessel1Node2");
            SEFluidCircuitNode& Windkessel2Node1 = fluidCircuit->CreateNode("Windkessel2Node1");
            SEFluidCircuitNode& Windkessel2Node2 = fluidCircuit->CreateNode("Windkessel2Node2");

            Ground.SetAsReferenceNode();
            Ground.GetNextPressure().SetValue(0, PressureUnit::Pa);

            SEFluidCircuitPath& Windkessel1ResistancePath1 = fluidCircuit->CreatePath(Windkessel1Node1, Windkessel1Node2, "Windkessel1ResistancePath1");
            Windkessel1ResistancePath1.GetNextResistance().SetValue(10.0, PressureTimePerVolumeUnit::Pa_s_Per_m3);
            SEFluidCircuitPath& Windkessel1ResistancePath2 = fluidCircuit->CreatePath(Windkessel1Node2, Ground, "Windkessel1ResistancePath2");
            Windkessel1ResistancePath2.GetNextResistance().SetValue(10.0, PressureTimePerVolumeUnit::Pa_s_Per_m3);
            SEFluidCircuitPath& Windkessel1CompliancePath = fluidCircuit->CreatePath(Windkessel1Node2, Ground, "Windkessel1CompliancePath");
            Windkessel1CompliancePath.GetNextCompliance().SetValue(10.0, VolumePerPressureUnit::m3_Per_Pa);

            SEFluidCircuitPath& Windkessel2ResistancePath1 = fluidCircuit->CreatePath(Windkessel2Node1, Windkessel2Node2, "Windkessel2ResistancePath1");
            Windkessel2ResistancePath1.GetNextResistance().SetValue(10.0, PressureTimePerVolumeUnit::Pa_s_Per_m3);
            SEFluidCircuitPath& Windkessel2ResistancePath2 = fluidCircuit->CreatePath(Windkessel2Node2, Ground, "Windkessel2ResistancePath2");
            Windkessel2ResistancePath2.GetNextResistance().SetValue(10.0, PressureTimePerVolumeUnit::Pa_s_Per_m3);
            SEFluidCircuitPath& Windkessel2CompliancePath = fluidCircuit->CreatePath(Windkessel2Node2, Ground, "Windkessel2CompliancePath");
            Windkessel2CompliancePath.GetNextCompliance().SetValue(10.0, VolumePerPressureUnit::m3_Per_Pa);

            SEFluidCircuitPath* Windkessel1SourcePath = nullptr;
            SEFluidCircuitPath* Windkessel2SourcePath = nullptr;
            SEFluidCircuitPath* Windkessel1SourcePath2 = nullptr;
            SEFluidCircuitPath* Windkessel2SourcePath2 = nullptr;
            SEFluidCircuitPath* BlackBoxSourcePath = nullptr;
            SEFluidCircuitPath* BlackBoxTargetPath = nullptr;
            SEFluidBlackBox* BlackBox = nullptr;
            SEBlackBoxManager BBmgr(m_Logger);
            if (iterType == 0)
            {
              Windkessel1SourcePath = &fluidCircuit->CreatePath(Ground, Windkessel1Node1, "Windkessel1SourcePath");
              if (iterFirstSourceType == 0)
              {
                Windkessel1SourcePath->GetNextFlowSource().SetValue(firstDirectionMultiplier * 20.0, VolumePerTimeUnit::m3_Per_s);
              }
              else if (iterFirstSourceType == 1)
              {
                Windkessel1SourcePath->GetNextPressureSource().SetValue(firstDirectionMultiplier * 20.0, PressureUnit::Pa);
              }
              else
              {
                Windkessel1SourcePath->GetNextFlowSource().SetValue(firstDirectionMultiplier * 20.0, VolumePerTimeUnit::m3_Per_s);
                Windkessel1SourcePath2 = &fluidCircuit->CreatePath(Ground, Windkessel1Node1, "Windkessel1SourcePath2");
                Windkessel1SourcePath2->GetNextPressureSource().SetValue(firstDirectionMultiplier * 20.0, PressureUnit::Pa);
              }

              Windkessel2SourcePath = &fluidCircuit->CreatePath(Ground, Windkessel2Node1, "Windkessel2SourcePath");
              if (iterSecondSourceType == 0)
              {
                Windkessel2SourcePath->GetNextFlowSource().SetValue(secondDirectionMultiplier * 20.0, VolumePerTimeUnit::m3_Per_s);
              }
              else if (iterSecondSourceType == 1)
              {
                Windkessel2SourcePath->GetNextPressureSource().SetValue(secondDirectionMultiplier * 20.0, PressureUnit::Pa);
              }
              else
              {
                Windkessel2SourcePath->GetNextFlowSource().SetValue(secondDirectionMultiplier * 20.0, VolumePerTimeUnit::m3_Per_s);
                Windkessel2SourcePath2 = &fluidCircuit->CreatePath(Ground, Windkessel2Node1, "Windkessel2SourcePath2");
                Windkessel2SourcePath2->GetNextPressureSource().SetValue(secondDirectionMultiplier * 20.0, PressureUnit::Pa);
              }
            }
            else
            {
              SEFluidCircuitNode& BlackBoxNode = fluidCircuit->CreateNode("BlackBoxNode");
              BlackBoxSourcePath = &fluidCircuit->CreatePath(BlackBoxNode, Windkessel1Node1, "BlackBoxSourcePath");
              BlackBoxTargetPath = &fluidCircuit->CreatePath(BlackBoxNode, Windkessel2Node1, "BlackBoxTargetPath");
              BlackBox = BBmgr.GetLiquidBlackBox("Windkessel1Node1", "Windkessel2Node1", "BlackBox");
              BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(*BlackBox, *BlackBoxSourcePath, *BlackBoxTargetPath);
            }

            fluidCircuit->StateChange();

            //Run -----------------------------------------------------------------------------
            double sample = 0;
            while (currentTime_s < 20)
            {
              if (iterType == 0)
              {
                if (iterFirstSourceType == 0)
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel1SourcePath->GetNextFlowSource().SetValue(firstDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);
                }
                else if (iterFirstSourceType == 1)
                {
                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel1SourcePath->GetNextPotentialSource().SetValue(firstDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
                else
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel1SourcePath->GetNextFlowSource().SetValue(firstDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);

                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel1SourcePath2->GetNextPotentialSource().SetValue(firstDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }

                if (iterSecondSourceType == 0)
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel2SourcePath->GetNextFlowSource().SetValue(secondDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);
                }
                else if (iterSecondSourceType == 1)
                {
                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel2SourcePath->GetNextPotentialSource().SetValue(secondDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
                else
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel2SourcePath->GetNextFlowSource().SetValue(secondDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);

                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  Windkessel2SourcePath2->GetNextPotentialSource().SetValue(secondDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
              }
              else
              {
                if (iterFirstSourceType == 0)
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeSourceFlux(firstDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);
                }
                else if (iterFirstSourceType == 1)
                {
                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeSourcePotential(firstDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
                else
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeSourceFlux(firstDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);

                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeSourcePotential(firstDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }

                if (iterSecondSourceType == 0)
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeTargetFlux(secondDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);
                }
                else if (iterSecondSourceType == 1)
                {
                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeTargetPotential(secondDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
                else
                {
                  double flow_m3_Per_s = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeTargetFlux(secondDirectionMultiplier * flow_m3_Per_s, VolumePerTimeUnit::m3_Per_s);

                  double potential_Pa = 20.0 + 20.0 * sin(currentTime_s);
                  BlackBox->ImposeTargetPotential(secondDirectionMultiplier * potential_Pa, PressureUnit::Pa);
                }
              }

              fluidCalculator.Process(*fluidCircuit, timeStep_s);
              fluidCalculator.PostProcess(*fluidCircuit);

              currentTime_s += timeStep_s;
              sample += timeStep_s;
              if (sample > 0.1) //every 0.1 seconds, track state of circuit
              {
                sample = 0;
                trk.Track(currentTime_s, *fluidCircuit);
              }
            }

            std::string sOutputFile = sOutputDirectory + "/" + name + "_Circuit.csv";
            trk.WriteTrackToFile(sOutputFile.c_str());
            m_Circuits->Clear();
          }
        }
      }
    }
  }
}
