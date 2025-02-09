/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "circuit/SECircuitManager.h"
#include "circuit/electrical/SEElectricalCircuit.h"
#include "circuit/electrical/SEElectricalCircuitNode.h"
#include "circuit/electrical/SEElectricalCircuitPath.h"
#include "circuit/electrical/SEElectricalCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/thermal/SEThermalCircuitNode.h"
#include "circuit/thermal/SEThermalCircuitPath.h"
#include "circuit/thermal/SEThermalCircuitCalculator.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressureTimeSquaredPerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "utils/DataTrack.h"
#include "utils/FileUtils.h"
#include "io/protobuf/PBCircuit.h"

void CommonDataModelTest::TestSetup7SeriesRCDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup7SeriesRCSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup7SeriesRCPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup8SeriesRDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);

}

void CommonDataModelTest::TestSetup8SeriesRSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup8SeriesRPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup9SeriesRLDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(0.001, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup9SeriesRLSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(0.001, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup9SeriesRLPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3);
  Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(0.001, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup10ParallelRDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup10ParallelRSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup10ParallelRPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(20, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(30, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup11ParallelRCDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node1.GetPressure().SetValue(0, PressureUnit::Pa); // not sure if this is necessary since the pressure source is pushing a pressure here

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup11ParallelRCSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node1.GetPressure().SetValue(0, PressureUnit::Pa); // not sure if this is necessary since the pressure source is pushing a pressure here

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup11ParallelRCPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node1.GetPressure().SetValue(0, PressureUnit::Pa); // not sure if this is necessary since the pressure source is pushing a pressure here

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup12ParallelRLDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup12ParallelRLSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s); 
}

void CommonDataModelTest::TestSetup12ParallelRLPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  //Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup12ParallelRLSINCentered()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2);
  Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup13SeriesRLCDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup13SeriesRLCSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup13SeriesRLCPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4);
  Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup14ParallelRLCDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode(); 
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup14ParallelRLCSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup14ParallelRLCPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup15SwitchRCDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup15SwitchRCSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3); 
}

void CommonDataModelTest::TestSetup15SwitchRCPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup16Comprehensive1DC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextInertance().SetValue(0.1, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path4.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node4,"Path5");
  Path5.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path6.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup16Comprehensive1SIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextInertance().SetValue(0.1, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path4.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node4,"Path5");
  Path5.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path6.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup16Comprehensive1PULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextInertance().SetValue(0.1, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path4.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node4,"Path5");
  Path5.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path6.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup17BasicDiodeDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup17BasicDiodeSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup17BasicDiodePULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
  // Basic Description
  // Scenario 1 DC Pressure Source => Capacitor charges and turns into an infinite impedance circuit element (0 flow across at steady state)
  // Scenario 2 Sinusoidal Pressure Source => Capacitor Flow will be relative to the derivative of the pressure source, should see volume change at nodes 2 and 3
  // Scenario 3 Pulse Train Pressure Source => Capacitor will charge during DC flow according to capacitor behavior (halfpower @ time = RC), and discharge during the times with no Pressure. Should see volume change at nodes 2 and 3
  // TODO: Add scenarios for flow sources
}

//void TestSetup18Comprehensive2DC()
//{
//
//  //10. Comprehensive1
//  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
//
//  //-----------------------------------------------------------
//  //Nodes
//  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
//  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
//  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
//  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
//
//  //Reference node
//  //This works like ground - I added code to allow it to have a value other than 0
//  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
//  //We have to initialize this pressure if it's attached to a Compliance
//  //But it doesn't necessarily have to be 0
//  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
//  Node4.GetPressure().SetValue(0, PressureUnit::Pa);
//
//  //We need to initialize volumes
//  Node1.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
//  Node2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
//  Node3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);  
//  //Node4.GetSourceNode().GetNextVolume().SetValue(1e10, VolumeUnit::m3);  //infinite ground
//
//  //-----------------------------------------------------------
//  //Paths
//  //Pressure source
//  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
//  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
//
//  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
//  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
//  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
//  Path3.SetSwitch(eGate::Open);
//  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
//  Path4.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
//  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node4,"Path5");
//  Path5.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
//  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
//  Path6.GetNextCompliance().SetValue(1000, VolumePerPressureUnit::m3_Per_Pa);
//  Path6.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
//  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
//
//  //Flow Source
//  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");
//  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
//
//  // Basic Description
//  // DC only, switch controlled from preprocess.
//  // Scenario 1 DC Pressure Source => Should see similar behavior to pulse train in test case 1, SeriesRC
//  // Scenario 2 Sinusoidal Pressure Source =>  N/A
//  // Scenario 3 Pulse Train Pressure Source => N/A
//  // TODO: Add scenarios for flow sources 
//}




void CommonDataModelTest::TestSetup18RCSeriesDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup18RCSeriesSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup18RCSeriesPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup19RCParallelDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup19RCParallelSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup19RCParallelPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup20SeriesCapDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  
  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure Source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup20SeriesCapSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup20SeriesCapPulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup21ParallelRDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow Source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup21ParallelRSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup21ParallelRPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup22CurrentCompDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node4,"Path5");
  Path5.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup22CurrentCompSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node4,"Path5");
  Path5.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup22CurrentCompPulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  // calling current sources on path 7 b/c of how preprocess is set up
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);  // TODO: Clean up methodology if these are to be released at any point.

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node4,"Path5");
  Path5.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node2,Node4,"Path6");
  Path6.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup23SeriesRLDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup23SeriesRLSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s); 
}

void CommonDataModelTest::TestSetup23SeriesRLPULSECurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (or inductor?)
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup24ParallelRLDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup24ParallelRLSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup24ParallelRLPULSECurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance (inductor?)
  //But it doesn't necessarily have to be 0
  //Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.001, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
}

void CommonDataModelTest::TestSetup25BasicDiodeDCCurrent()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup25BasicDiodeSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup25BasicDiodePULSECurrent()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  //We have to initialize pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");  
  Path3.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);
}

void CommonDataModelTest::TestSetup26SwitchRCDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup26SwitchRCSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup26SwitchRCPULSECurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.SetNextSwitch(eGate::Open);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup27SeriesRLCDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup27SeriesRLCSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup27SeriesRLCPULSECurrent()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(100, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup28ParallelRLCDCCurrent()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup28ParallelRLCSINCurrent()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup28ParallelRLCPULSECurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path4.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup29SeriesPressureSourceAdditionDC()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup29SeriesPressureSourceAdditionSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup29SeriesPressureSourceAdditionPULSE()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node3,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3); 
}

void CommonDataModelTest::TestSetup30SeriesCurrentSourceAdditionDC()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  
  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node2,"Path8");  
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup30SeriesCurrentSourceAdditionSIN()
{
  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node2,"Path8");  
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup30SeriesCurrentSourceAdditionPULSE()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node3); Node3.SetAsReferenceNode();
  Node3.GetNextPressure().SetValue(0,PressureUnit::Pa);


  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node3,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node2,"Path8");  
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node2,Node3,"Path4");
  Path4.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup31ParallelPressureSourceAdditionDC()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node1,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup31ParallelPressureSourceAdditionSIN()
{  
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node1,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup31ParallelPressureSourceAdditionPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node1,"Path3");
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup32ParallelCurrentSourceAdditionDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node2,Node1,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500,PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup32ParallelCurrentSourceAdditionSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node2,Node1,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup32ParallelCurrrentSourceAdditionPULSE()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  Node2.GetNextPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node2,Node1,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextResistance().SetValue(500,PressureTimePerVolumeUnit::Pa_s_Per_m3); 
}

void CommonDataModelTest::TestSetup33SeriesCapDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup33SeriesCapSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup33SeriesCapPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup34ParallelCapDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup34ParallelCapSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup34ParallelCapPulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup35ParallelCapDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup35ParallelCapSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup35ParallelCapPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path2.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path3.GetSourceNode().GetNextVolume().SetValue(5, VolumeUnit::m3);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup36SeriesIndDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup36SeriesIndSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup36SeriesIndPulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node4,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup37SeriesIndDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup37SeriesIndSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node4); Node4.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup37SeriesIndPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  Node4.SetAsReferenceNode();
  fluidCircuit.AddNode(Node4);
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetPressure().SetValue(0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node3,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node4,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup38ParallelIndDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup38ParallelIndSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup38ParallelIndPulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup39ParallelIndDCCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup39ParallelIndSINCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup39ParallelIndPulseCurrent()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node1.GetPressure().SetValue(0, PressureUnit::Pa);
  Node2.GetPressure().SetValue(0, PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node2,Node1,"Path7");  
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node1,Node2,"Path2");
  Path2.GetNextInertance().SetValue(0.01, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path2.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextInertance().SetValue(0.05, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  Path3.GetFlow().SetValue(0, VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node1,Node2,"Path4");
  Path4.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
}

void CommonDataModelTest::TestSetup40BadDiodeDC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
 

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node1,"Path2");
  Path2.SetNextValve(eGate::Closed);

  // should fail if working properly
}

void CommonDataModelTest::TestSetup40BadDiodeSIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node1,"Path2");
  Path2.SetNextValve(eGate::Closed);

  // should fail if working properly
}

void CommonDataModelTest::TestSetup40BadDiodePulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node2); Node2.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node2.GetPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node2,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node1,"Path2");
  Path2.SetNextValve(eGate::Closed);

  // should fail if working properly
}

void CommonDataModelTest::TestSetup41ValidationComprehensive1DC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = fluidCircuit.CreateNode("Node7");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node7); Node7.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node7.GetPressure().SetValue(0,PressureUnit::Pa);
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node3.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node7,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node5,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  //Switch
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node5,Node6,"Path2");
  Path2.SetNextSwitch(eGate::Open);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node1,Node2,"Path11");
  Path11.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(2000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node3,"Path5");
  Path5.GetNextResistance().SetValue(1500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node3,Node4,"Path6");
  Path6.GetNextResistance().SetValue(400, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node5,Node7,"Path10");
  Path10.GetNextResistance().SetValue(200, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node6,Node7,"Path12");
  Path12.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node3,Node4,"Path8");
  Path8.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path8.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node5,Node7,"Path9");
  Path9.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup41ValidationComprehensive1SIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = fluidCircuit.CreateNode("Node7");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node7); Node7.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node7.GetPressure().SetValue(0,PressureUnit::Pa);
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node3.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node7,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node5,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  //Switch
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node5,Node6,"Path2");
  Path2.SetNextSwitch(eGate::Open);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node1,Node2,"Path11");
  Path11.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(2000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node3,"Path5");
  Path5.GetNextResistance().SetValue(1500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node3,Node4,"Path6");
  Path6.GetNextResistance().SetValue(400, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node5,Node7,"Path10");
  Path10.GetNextResistance().SetValue(200, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node6,Node7,"Path12");
  Path12.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node3,Node4,"Path8");
  Path8.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path8.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node5,Node7,"Path9");
  Path9.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup41ValidationComprehensive1Pulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = fluidCircuit.CreateNode("Node7");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node7); Node7.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node7.GetPressure().SetValue(0,PressureUnit::Pa);
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node3.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node7,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node5,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  //Switch
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node5,Node6,"Path2");
  Path2.SetNextSwitch(eGate::Open);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node2,Node1,"Path11");
  Path11.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node2,Node1,"Path3");
  Path3.GetNextResistance().SetValue(2000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node3,Node2,"Path5");
  Path5.GetNextResistance().SetValue(1500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node3,Node4,"Path6");
  Path6.GetNextResistance().SetValue(400, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node5,Node7,"Path10");
  Path10.GetNextResistance().SetValue(200, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node6,Node7,"Path12");
  Path12.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node3,Node4,"Path8");
  Path8.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path8.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node5,Node7,"Path9");
  Path9.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup41ValidationComprehensive1SINCentered()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");
  SEFluidCircuitNode& Node7 = fluidCircuit.CreateNode("Node7");


  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node7); Node7.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node7.GetPressure().SetValue(0,PressureUnit::Pa);
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node3.GetPressure().SetValue(0,PressureUnit::Pa);

  //-----------------------------------------------------------
  //Paths
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node7,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);

  //Flow source
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node4,Node5,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);

  //Switch
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node5,Node6,"Path2");
  Path2.SetNextSwitch(eGate::Open);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node2,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node1,Node2,"Path11");
  Path11.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");
  Path3.GetNextResistance().SetValue(2000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node2,Node3,"Path5");
  Path5.GetNextResistance().SetValue(1500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node3,Node4,"Path6");
  Path6.GetNextResistance().SetValue(400, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node5,Node7,"Path10");
  Path10.GetNextResistance().SetValue(200, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node6,Node7,"Path12");
  Path12.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node3,Node4,"Path8");
  Path8.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path8.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node5,Node7,"Path9");
  Path9.GetNextCompliance().SetValue(0.0001, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup42ValidationComprehensive2DC()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node6); Node6.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node1.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node6.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure sources
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node6,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");  
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);

  //Flow sources
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node6,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node5,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node1,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node3,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node1,Node5,"Path6");
  Path6.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node5,Node6,"Path11");
  Path11.GetNextResistance().SetValue(450, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node1,Node6,"Path12");
  Path12.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path12.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node5,"Path5");
  Path5.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path5.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node1,Node4,"Path9");
  Path9.GetNextCompliance().SetValue(0.003, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node4,Node5,"Path10");
  Path10.GetNextCompliance().SetValue(0.0005, VolumePerPressureUnit::m3_Per_Pa);
  Path10.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup42ValidationComprehensive2SIN()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node6); Node6.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node1.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node6.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure sources
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node6,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");  
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);

  //Flow sources
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node6,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node5,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node1,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node3,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node1,Node5,"Path6");
  Path6.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node5,Node6,"Path11");
  Path11.GetNextResistance().SetValue(450, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node1,Node6,"Path12");
  Path12.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path12.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node5,"Path5");
  Path5.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path5.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node1,Node4,"Path9");
  Path9.GetNextCompliance().SetValue(0.003, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node4,Node5,"Path10");
  Path10.GetNextCompliance().SetValue(0.0005, VolumePerPressureUnit::m3_Per_Pa);
  Path10.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup42ValidationComprehensive2Pulse()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");
  

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node6); Node6.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node1.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node6.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure sources
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node6,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");  
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);

  //Flow sources
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node6,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node5,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node1,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node2,Node3,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node1,Node5,"Path6");
  Path6.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node5,Node6,"Path11");
  Path11.GetNextResistance().SetValue(450, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node1,Node6,"Path12");
  Path12.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path12.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node5,"Path5");
  Path5.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path5.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node1,Node4,"Path9");
  Path9.GetNextCompliance().SetValue(0.003, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node4,Node5,"Path10");
  Path10.GetNextCompliance().SetValue(0.0005, VolumePerPressureUnit::m3_Per_Pa);
  Path10.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

void CommonDataModelTest::TestSetup42ValidationComprehensive2SINCentered()
{
  SEFluidCircuit& fluidCircuit = m_Circuits->CreateFluidCircuit("Fluid");

  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit.CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit.CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit.CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit.CreateNode("Node4");
  SEFluidCircuitNode& Node5 = fluidCircuit.CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit.CreateNode("Node6");

  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit.AddNode(Node6); Node6.SetAsReferenceNode();
  //We have to initialize this pressure if it's attached to a Compliance
  //But it doesn't necessarily have to be 0
  Node5.GetPressure().SetValue(0,PressureUnit::Pa);
  Node1.GetPressure().SetValue(0,PressureUnit::Pa);
  Node4.GetPressure().SetValue(0,PressureUnit::Pa);
  Node6.GetPressure().SetValue(0,PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  //Pressure sources
  SEFluidCircuitPath& Path1 = fluidCircuit.CreatePath(Node6,Node1,"Path1");  
  Path1.GetNextPressureSource().SetValue(20,PressureUnit::Pa);
  SEFluidCircuitPath& Path3 = fluidCircuit.CreatePath(Node1,Node2,"Path3");  
  Path3.GetNextPressureSource().SetValue(10,PressureUnit::Pa);

  //Flow sources
  SEFluidCircuitPath& Path7 = fluidCircuit.CreatePath(Node6,Node1,"Path7");
  Path7.GetNextFlowSource().SetValue(3,VolumePerTimeUnit::m3_Per_s);
  SEFluidCircuitPath& Path8 = fluidCircuit.CreatePath(Node1,Node5,"Path8");
  Path8.GetNextFlowSource().SetValue(2,VolumePerTimeUnit::m3_Per_s);

  //Diode
  SEFluidCircuitPath& Path4 = fluidCircuit.CreatePath(Node3,Node1,"Path4");
  Path4.SetNextValve(eGate::Closed);

  //Resistors
  SEFluidCircuitPath& Path2 = fluidCircuit.CreatePath(Node3,Node2,"Path2");
  Path2.GetNextResistance().SetValue(1000, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path6 = fluidCircuit.CreatePath(Node1,Node5,"Path6");
  Path6.GetNextResistance().SetValue(500, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path11 = fluidCircuit.CreatePath(Node5,Node6,"Path11");
  Path11.GetNextResistance().SetValue(450, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Compliances
  SEFluidCircuitPath& Path12 = fluidCircuit.CreatePath(Node6,Node1,"Path12");
  Path12.GetNextCompliance().SetValue(0.005, VolumePerPressureUnit::m3_Per_Pa);
  Path12.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path5 = fluidCircuit.CreatePath(Node1,Node5,"Path5");
  Path5.GetNextCompliance().SetValue(0.001, VolumePerPressureUnit::m3_Per_Pa);
  Path5.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path9 = fluidCircuit.CreatePath(Node1,Node4,"Path9");
  Path9.GetNextCompliance().SetValue(0.003, VolumePerPressureUnit::m3_Per_Pa);
  Path9.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
  SEFluidCircuitPath& Path10 = fluidCircuit.CreatePath(Node4,Node5,"Path10");
  Path10.GetNextCompliance().SetValue(0.0005, VolumePerPressureUnit::m3_Per_Pa);
  Path10.GetSourceNode().GetNextVolume().SetValue(0.0,VolumeUnit::m3);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Performs the preprocess math for any DC circuit in the circuit unit tests.
///
/// \param  dT  contains the information about how much time has passed in the test
/// \param  i   holds the logic for which test is running
///
/// \details
/// TestPreProcess1 is used for the tests that have DC drivers. It also contains the logic necessary
/// to control a switch if the test calls for it.
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::TestPreProcess1(double dT, int i)
{

  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.

  // ALL PRESSURE SOURCES MUST EXIST ON PATH 1 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE
  // ALL FLOW SOURCES MUST EXIST ON PATH 7 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE
  //DC source
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE  = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if(i==22||i==24||i==35)
  {
    MULTISOURCEPRESSURE = true;
  }
  if(i==23||i==25||i==35)
  {
    MULTISOURCEFLOW = true;
  }
  if(i<11||i==13||i==27||i==29||i==31||i==33||i==34)  //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if(i==9||i==11||i==12||(i>=14&&i<=21)||i==26||i==28||i==30||i==32||i==34)  // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if(i==8||i==9||i==15||i==19||i==34)  // check for switches
  {
    SWITCHPRESENT = true;
  }
  if(MULTISOURCEPRESSURE)
  {
    double dPressure1 = 20;
    double dPressure2 = 10;
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1,PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2,PressureUnit::Pa);
  }
  if(MULTISOURCEFLOW)
  {
    double dFlow1 = 3;
    double dFlow2 = 2;
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1,VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2,VolumePerTimeUnit::m3_Per_s);
  }
  if(PRESSURESOURCE)
  {
    double dPressure = 20;
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure,PressureUnit::Pa);
  }
  if(FLOWSOURCE)
  {
    double dFlow = 3;
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);
  }
  if(false)  //manually set negative DC flow if desired
  {
    double dFlow = -3;
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);
  }
  //Switch Test
  if(SWITCHPRESENT)
  {
    if(i == 8 || i == 34 || i == 19)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Closed);
      }
    }
    else if(i==9||i==15)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Closed);
      }
    }

  }
  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Performs the preprocess math for the circuits that use sinusoidal sources.
///
/// \param  dT  contains the information about how much time has passed in the test
/// \param  i   holds the logic for which test is running
///
/// \details
/// TestPreProcess2 is used for the tests that have sinusoidal drivers. It also contains the logic necessary
/// to control a switch if the test calls for it. The pressure sources in this preprocess are all offset 
/// from 0 so they never go negative.
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::TestPreProcess2(double dT, int i)
{

  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.

  // ALL PRESSURE SOURCES MUST EXIST ON PATH 1 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE  = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if(i==22||i==24||i==35)
  {
    MULTISOURCEPRESSURE = true;
  }
  if(i==23||i==25||i==35)
  {
    MULTISOURCEFLOW = true;
  }
  if(i<11||i==13||i==27||i==29||i==31||i==33||i==34)  //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if(i==9||i==11||i==12||(i>=14&&i<=21)||i==26||i==28||i==30||i==32||i==34)  // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if(i==8||i==9||i==15||i==19||i==34)  // check for switches
  {
    SWITCHPRESENT = true;
  }
  if(MULTISOURCEPRESSURE)
  {
    double dPressure1 = 20+20*sin(dT);
    double dPressure2 = 10+10*sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1,PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2,PressureUnit::Pa);
  }
  if(MULTISOURCEFLOW)
  {
    double dFlow1 = 3*sin(dT);
    double dFlow2 = 2*sin(dT);
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1,VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2,VolumePerTimeUnit::m3_Per_s);
  }
  //Sinusoidal source
  if(PRESSURESOURCE)
    //Modify the pressure source
  {
    double dPressure = 20+20*sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure,PressureUnit::Pa);  // need to adjust this so path 4 isn't always the source path
  }
  if(FLOWSOURCE)
    //Modify the flow source
  {
    double dFlow = 3*sin(dT);  //can go negative to test negative flows
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);  // need to adjust this so path 4 isn't always the source path
  }

  //Switch Test
  if(SWITCHPRESENT)
  {
    if(i==8 || i == 34|| i == 19)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Closed);
      }
    }
    else if(i==9||i==15)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Closed);
      }
    }

  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Performs the preprocess math for the circuits that use pulse train sources.
///
/// \param  dT  contains the information about how much time has passed in the test
/// \param  i   holds the logic for which test is running
///
/// \details3 is used for the tests that have pulse train drivers. It also contains the logic necessary
/// to control a switch if the test calls for it. 
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::TestPreProcess3(double dT, int i)
{

  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.

  // ALL PRESSURE SOURCES MUST EXIST ON PATH 1 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE

  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE  = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if(i==22||i==24||i==35)
  {
    MULTISOURCEPRESSURE = true;
  }
  if(i==23||i==25||i==35)
  {
    MULTISOURCEFLOW = true;
  }
  if(i<11||i==13||i==27||i==29||i==31||i==33||i==34)  //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if(i==9||i==11||i==12||(i>=14&&i<=21)||i==26||i==28||i==30||i==32||i==34)  // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if(i==8||i==9||i==15||i==19||i==34)  // check for switches
  {
    SWITCHPRESENT = true;
  }

  int pulseWidth = 2;
  int intDT = int(floor(dT));  //rounding dT
  bool pulseStatus = true;
  //Pulse train
  //May need to rework this logic, but it should be passable for now.
  if(true)
  {
    if((intDT % pulseWidth) >= (pulseWidth/2) )  // seems to work more or less alright
    {
      pulseStatus = false;
    }

    if(pulseStatus)
    {
      if(PRESSURESOURCE)
      {
        double dPressure = 20;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure,PressureUnit::Pa);
      }

      if(FLOWSOURCE)
      {
        double dFlow = 3;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);
      }
      if(MULTISOURCEPRESSURE)
      {
        double dPressure1 = 20;
        double dPressure2 = 10;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1,PressureUnit::Pa);
        m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2,PressureUnit::Pa);
      }
      if(MULTISOURCEFLOW)
      {
        double dFlow1 = 3;
        double dFlow2 = 2;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1,VolumePerTimeUnit::m3_Per_s);
        m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2,VolumePerTimeUnit::m3_Per_s);
      }
    }
    else
    {
      if(PRESSURESOURCE)
      {
         double dPressure = 0;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, PressureUnit::Pa);
      }
      if(FLOWSOURCE)
      {
        double dFlow = 0;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);
      }
      if(MULTISOURCEPRESSURE)
      {
        double dPressure1 = 0;
        double dPressure2 = 0;
        m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1,PressureUnit::Pa);
        m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2,PressureUnit::Pa);
      }
      if(MULTISOURCEFLOW)
      {
        double dFlow1 = 0;
        double dFlow2 = 0;
        m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1,VolumePerTimeUnit::m3_Per_s);
        m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2,VolumePerTimeUnit::m3_Per_s);
      }
    }
  }
  //Switch Test
  if(SWITCHPRESENT)
  {
    if(i==8 || i == 34 || i == 19)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Closed);
      }
    }
    else if(i==9||i==15)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Closed);
      }
    }

  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Performs the preprocess math for the circuits that use sinusoidal sources with pressure sources 
/// centered around 0.
///
/// \param  dT  contains the information about how much time has passed in the test
/// \param  i   holds the logic for which test is running
///
/// \details
/// TestPreProcess4 is used for the tests that have sinusoidal drivers. It also contains the logic necessary
/// to control a switch if the test calls for it. The pressure sources in this preprocess are not offset, so
/// they can go negative.
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::TestPreProcess4(double dT, int i)
{

  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.

  // ALL PRESSURE SOURCES MUST EXIST ON PATH 1 UNTIL THIS IS CHANGED TO BE MORE FLEXIBLE
  bool FLOWSOURCE = false;
  bool SWITCHPRESENT = false;
  bool PRESSURESOURCE  = false;
  bool MULTISOURCEPRESSURE = false;
  bool MULTISOURCEFLOW = false;
  if(i==22||i==24||i==35)
  {
    MULTISOURCEPRESSURE = true;
  }
  if(i==23||i==25||i==35)
  {
    MULTISOURCEFLOW = true;
  }
  if(i<11||i==13||i==27||i==29||i==31||i==33||i==34)  //check for pressure sources... re order the test order when all are in so these aren't pieces of junk
  {
    PRESSURESOURCE = true;
  }
  if(i==9||i==11||i==12||(i>=14&&i<=21)||i==26||i==28||i==30||i==32||i==34)  // check for flow sources
  {
    FLOWSOURCE = true;
  }
  if(i==8||i==9||i==15||i==19||i==34)  // check for switches
  {
    SWITCHPRESENT = true;
  }
  if(MULTISOURCEPRESSURE)
  {
    double dPressure1 = 20*sin(dT);
    double dPressure2 = 10*sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure1,PressureUnit::Pa);
    m_Circuits->GetFluidPath("Path3")->GetNextPressureSource().SetValue(dPressure2,PressureUnit::Pa);
  }
  if(MULTISOURCEFLOW)
  {
    double dFlow1 = 3*sin(dT);
    double dFlow2 = 2*sin(dT);
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow1,VolumePerTimeUnit::m3_Per_s);
    m_Circuits->GetFluidPath("Path8")->GetNextFlowSource().SetValue(dFlow2,VolumePerTimeUnit::m3_Per_s);
  }
  //Sinusoidal source
  if(PRESSURESOURCE)
    //Modify the pressure source
  {
    double dPressure = 20*sin(dT);
    m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure,PressureUnit::Pa);  // need to adjust this so path 4 isn't always the source path
  }
  if(FLOWSOURCE)
    //Modify the flow source
  {
    double dFlow = 3*sin(dT);  //can go negative to test negative flows
    m_Circuits->GetFluidPath("Path7")->GetNextFlowSource().SetValue(dFlow,VolumePerTimeUnit::m3_Per_s);  // need to adjust this so path 4 isn't always the source path
  }

  //Switch Test
  if(SWITCHPRESENT)
  {
    if(i==8 || i== 34)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path2")->SetNextSwitch(eGate::Closed);
      }
    }
    else if(i==9||i==15)
    {
      if(dT<5)
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Open);
      }
      else
      {
        m_Circuits->GetFluidPath("Path3")->SetNextSwitch(eGate::Closed);
      }
    }

  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Controls the PreProcess logic for the BasicCircuit test
///
/// \param  dT  contains the information about how much time has passed in the test
///
/// <DETAILED_DESCRIPTION>
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::BasicCircuitPreProcess(double dT)
{
  //Note: If you don't modify a value, it will remain the same.
  //    Only assign a value for Next if you're changing it.
  //    Never change "current" values, only next.
  double dPressure = 20+20*sin(dT);
  m_Circuits->GetFluidPath("Path1")->GetNextPressureSource().SetValue(dPressure, PressureUnit::Pa);
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Runs the basic circuit unit test
///
/// \param  outputDirectory  points to the unit test directory
///
/// \details
/// This is a unit test for the circuit math. It runs for 100 seconds as opposed to the suite of other
/// circuit unit tests that only run for 10 seconds each. It uses a pressure source outputting a 
/// sinusoidal waveform.
//--------------------------------------------------------------------------------------------------
void CommonDataModelTest::BasicCircuitTest(const std::string& outputDirectory)
{
  m_Logger = new Logger(outputDirectory + "/BasicCircuit.log");
  double timeStep_s = 1.0/165.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
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

  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit->CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
  fluidCircuit->StateChange();
  fluidCircuit->SetNextAndCurrentFromBaselines();

  double sample = 0;
  bool serialized = false;
  while(currentTime_s < 100)
  {
    //PreProcess - to be done by the systems
    BasicCircuitPreProcess(currentTime_s);

    //Process
    fluidCalculator.Process(*fluidCircuit, timeStep_s);

    //PostProcess
    fluidCalculator.PostProcess(*fluidCircuit);

    currentTime_s += timeStep_s;
    //sampleDT += dDT;

    sample+= timeStep_s;
    if(sample>0.1)// every 0.1 seconds, track state of circuit
    {
      sample=0;
      trk1.Track(currentTime_s, *fluidCircuit);
    }

    if (currentTime_s > 80 && !serialized)
    {
      serialized = true;
      std::string jsonDir = outputDirectory + "/BasicCircuit.json";
      TestCircuitSerialization(jsonDir);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
  }

  m_Circuits->Clear();
  std::string sOutputFile = outputDirectory + "/BasicCircuit.csv";
  trk1.WriteTrackToFile(sOutputFile.c_str());
}

void CommonDataModelTest::MultiphysicsElectricalCircuitTest(const std::string& outputDirectory)
{
  m_Logger = new Logger(outputDirectory + "/MultiphysicsCircuit.log");
  double timeStep_s = 1.0 / 50.0;
  DataTrack dataTrack;
  SEElectricalCircuitCalculator electricalCalculator(DefaultElectricalCircuitCalculatorUnits, m_Logger);

  //Create circuit
  SEElectricalCircuit* electricalCircuit = &m_Circuits->CreateElectricalCircuit("Electrical");

  //Create nodes
  SEElectricalCircuitNode& Ground = electricalCircuit->CreateNode("Ground");
  SEElectricalCircuitNode& Node1 = electricalCircuit->CreateNode("Node1");
  SEElectricalCircuitNode& Node2 = electricalCircuit->CreateNode("Node2");
  SEElectricalCircuitNode& Node3 = electricalCircuit->CreateNode("Node3");
  SEElectricalCircuitNode& Node4 = electricalCircuit->CreateNode("Node4");
  Ground.SetAsReferenceNode();
  Ground.GetNextVoltage().SetValue(0, ElectricPotentialUnit::V);
  Node3.GetNextCharge().SetValue(1, ElectricChargeUnit::C);

  //Create paths with components
  SEElectricalCircuitPath& Path1 = electricalCircuit->CreatePath(Ground, Node1, "Path1");
  Path1.GetVoltageSourceBaseline().SetValue(0, ElectricPotentialUnit::V);
  SEElectricalCircuitPath& Path2 = electricalCircuit->CreatePath(Node1, Node2, "Path2");
  Path2.GetResistanceBaseline().SetValue(0.5, ElectricResistanceUnit::Ohm);
  SEElectricalCircuitPath& Path3 = electricalCircuit->CreatePath(Node2, Node3, "Path3");
  Path3.GetInductanceBaseline().SetValue(0.3, ElectricInductanceUnit::H);
  SEElectricalCircuitPath& Path4 = electricalCircuit->CreatePath(Node3, Ground, "Path4");
  Path4.GetCapacitanceBaseline().SetValue(5, ElectricCapacitanceUnit::F);
  SEElectricalCircuitPath& Path5 = electricalCircuit->CreatePath(Node2, Node4, "Path5");
  Path5.SetNextDiode(eGate::Closed);
  SEElectricalCircuitPath& Path6 = electricalCircuit->CreatePath(Ground, Node4, "Path6");
  Path6.SetNextValve(eGate::Open);

  electricalCircuit->SetNextAndCurrentFromBaselines();
  electricalCircuit->StateChange();

  double currentTime_s = 0.0;
  double endTime_s = 20;
  while (currentTime_s < endTime_s)
  {
    //PreProcess - Update the circuit
    //Update the potential source sinusoid
    double potential = 25 * sin(currentTime_s);
    Path1.GetNextVoltageSource().SetValue(potential, ElectricPotentialUnit::V);

    //Flip the diode at the right time
    if (currentTime_s > 15)
    {
      Path5.SetNextDiode(eGate::Open);
    }

    //Process - Solve the circuit
    electricalCalculator.Process(*electricalCircuit, timeStep_s);

    //PostProcess - Advance time
    electricalCalculator.PostProcess(*electricalCircuit);

    //Track state of circuit
    dataTrack.Track(currentTime_s, *electricalCircuit);

    currentTime_s += timeStep_s;
  }

  m_Circuits->Clear();
  std::string sOutputFile = outputDirectory + "/MultiphysicsCircuitElectrical.csv";
  dataTrack.WriteTrackToFile(sOutputFile.c_str());
}

void CommonDataModelTest::MultiphysicsFluidCircuitTest(const std::string& outputDirectory)
{
  m_Logger = new Logger(outputDirectory + "/MultiphysicsCircuit.log");
  double timeStep_s = 1.0 / 50.0;
  DataTrack dataTrack;
  SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);

  //Create circuit
  SEFluidCircuit* fluidCircuit = &m_Circuits->CreateFluidCircuit("Fluid");

  //Create nodes
  SEFluidCircuitNode& Ground = fluidCircuit->CreateNode("Ground");
  SEFluidCircuitNode& Node1 = fluidCircuit->CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit->CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit->CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit->CreateNode("Node4");
  Ground.SetAsReferenceNode();
  Ground.GetNextPressure().SetValue(0, PressureUnit::Pa);
  Node3.GetNextVolume().SetValue(1, VolumeUnit::m3);

  //Create paths with components
  SEFluidCircuitPath& Path1 = fluidCircuit->CreatePath(Ground, Node1, "Path1");
  Path1.GetPressureSourceBaseline().SetValue(0, PressureUnit::Pa);
  SEFluidCircuitPath& Path2 = fluidCircuit->CreatePath(Node1, Node2, "Path2");
  Path2.GetResistanceBaseline().SetValue(0.5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit->CreatePath(Node2, Node3, "Path3");
  Path3.GetInductanceBaseline().SetValue(0.3, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit->CreatePath(Node3, Ground, "Path4");
  Path4.GetComplianceBaseline().SetValue(5, VolumePerPressureUnit::m3_Per_Pa);
  SEFluidCircuitPath& Path5 = fluidCircuit->CreatePath(Node2, Node4, "Path5");
  Path5.SetNextSwitch(eGate::Closed);
  SEFluidCircuitPath& Path6 = fluidCircuit->CreatePath(Ground, Node4, "Path6");
  Path6.SetNextValve(eGate::Open);

  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();

  double currentTime_s = 0.0;
  double endTime_s = 20;
  while (currentTime_s < endTime_s)
  {
    //PreProcess - Update the circuit
    //Update the potential source sinusoid
    double potential = 25 * sin(currentTime_s);
    Path1.GetNextPressureSource().SetValue(potential, PressureUnit::Pa);

    //Flip the switch at the right time
    if (currentTime_s > 15)
    {
      Path5.SetNextSwitch(eGate::Open);
    }

    //Process - Solve the circuit
    fluidCalculator.Process(*fluidCircuit, timeStep_s);

    //PostProcess - Advance time
    fluidCalculator.PostProcess(*fluidCircuit);

    //Track state of circuit
    dataTrack.Track(currentTime_s, *fluidCircuit);

    currentTime_s += timeStep_s;
  }

  m_Circuits->Clear();
  std::string sOutputFile = outputDirectory + "/MultiphysicsCircuitFluid.csv";
  dataTrack.WriteTrackToFile(sOutputFile.c_str());
}

void CommonDataModelTest::MultiphysicsThermalCircuitTest(const std::string& outputDirectory)
{
  m_Logger = new Logger(outputDirectory + "/MultiphysicsCircuit.log");
  double timeStep_s = 1.0 / 50.0;
  DataTrack dataTrack;
  SEThermalCircuitCalculator thermalCalculator(DefaultThermalCircuitCalculatorUnits, m_Logger);

  //Create circuit
  SEThermalCircuit* thermalCircuit = &m_Circuits->CreateThermalCircuit("Thermal");

  //Create nodes
  SEThermalCircuitNode& Ground = thermalCircuit->CreateNode("Ground");
  SEThermalCircuitNode& Node1 = thermalCircuit->CreateNode("Node1");
  SEThermalCircuitNode& Node2 = thermalCircuit->CreateNode("Node2");
  SEThermalCircuitNode& Node3 = thermalCircuit->CreateNode("Node3");
  SEThermalCircuitNode& Node4 = thermalCircuit->CreateNode("Node4");
  Ground.SetAsReferenceNode();
  Ground.GetNextTemperature().SetValue(0, TemperatureUnit::K);
  Node3.GetNextHeat().SetValue(1, EnergyUnit::J);

  //Create paths with components
  SEThermalCircuitPath& Path1 = thermalCircuit->CreatePath(Ground, Node1, "Path1");
  Path1.GetTemperatureSourceBaseline().SetValue(0, TemperatureUnit::K);
  SEThermalCircuitPath& Path2 = thermalCircuit->CreatePath(Node1, Node2, "Path2");
  Path2.GetResistanceBaseline().SetValue(0.5, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& Path3 = thermalCircuit->CreatePath(Node2, Node3, "Path3");
  Path3.GetInductanceBaseline().SetValue(0.3, HeatInductanceUnit::K_s_Per_W);
  SEThermalCircuitPath& Path4 = thermalCircuit->CreatePath(Node3, Ground, "Path4");
  Path4.GetCapacitanceBaseline().SetValue(5, HeatCapacitanceUnit::J_Per_K);
  SEThermalCircuitPath& Path5 = thermalCircuit->CreatePath(Node2, Node4, "Path5");
  Path5.SetNextSwitch(eGate::Closed);
  SEThermalCircuitPath& Path6 = thermalCircuit->CreatePath(Ground, Node4, "Path6");
  Path6.SetNextValve(eGate::Open);

  thermalCircuit->SetNextAndCurrentFromBaselines();
  thermalCircuit->StateChange();

  double currentTime_s = 0.0;
  double endTime_s = 20;
  while (currentTime_s < endTime_s)
  {
    //PreProcess - Update the circuit
    //Update the potential source sinusoid
    double potential = 25 * sin(currentTime_s);
    Path1.GetNextTemperatureSource().SetValue(potential, TemperatureUnit::K);

    //Flip the switch at the right time
    if (currentTime_s > 15)
    {
      Path5.SetNextSwitch(eGate::Open);
    }

    //Process - Solve the circuit
    thermalCalculator.Process(*thermalCircuit, timeStep_s);

    //PostProcess - Advance time
    thermalCalculator.PostProcess(*thermalCircuit);

    //Track state of circuit
    dataTrack.Track(currentTime_s, *thermalCircuit);

    currentTime_s += timeStep_s;
  }

  m_Circuits->Clear();
  std::string sOutputFile = outputDirectory + "/MultiphysicsCircuitThermal.csv";
  dataTrack.WriteTrackToFile(sOutputFile.c_str());
}

void CommonDataModelTest::SeriesRCDCTest(const std::string& outputDirectory)
{
  TestSetup7SeriesRCDC();
  RunTest(outputDirectory, "SeriesRCDC", DC, 0);
}

void CommonDataModelTest::SeriesRCSINTest(const std::string& outputDirectory)
{
  TestSetup7SeriesRCSIN();
  RunTest(outputDirectory, "SeriesRCSIN", SIN, 0);
}

void CommonDataModelTest::SeriesRCPULSETest(const std::string& outputDirectory)
{
  TestSetup7SeriesRCPULSE();
  RunTest(outputDirectory, "SeriesRCPULSE", PULSE, 0);
}

void CommonDataModelTest::SeriesRDCTest(const std::string& outputDirectory)
{
  TestSetup8SeriesRDC();
  RunTest(outputDirectory, "SeriesRDC", DC, 1);
}

void CommonDataModelTest::SeriesRSINTest(const std::string& outputDirectory)
{
  TestSetup8SeriesRSIN();
  RunTest(outputDirectory, "SeriesRSIN", SIN, 1);
}

void CommonDataModelTest::SeriesRPULSETest(const std::string& outputDirectory)
{
  TestSetup8SeriesRPULSE();
  RunTest(outputDirectory, "SeriesRPULSE", PULSE, 1);
}

void CommonDataModelTest::SeriesRLDCTest(const std::string& outputDirectory)
{
  TestSetup9SeriesRLDC();
  RunTest(outputDirectory, "SeriesRLDC", DC, 2);
}

void CommonDataModelTest::SeriesRLSINTest(const std::string& outputDirectory)
{
  TestSetup9SeriesRLSIN();
  RunTest(outputDirectory, "SeriesRLSIN", SIN, 2);
}

void CommonDataModelTest::SeriesRLPULSETest(const std::string& outputDirectory)
{
  TestSetup9SeriesRLPULSE();
  RunTest(outputDirectory, "SeriesRLPULSE", PULSE, 2);
}

void CommonDataModelTest::ParallelRDCTest(const std::string& outputDirectory)
{
  TestSetup10ParallelRDC();
  RunTest(outputDirectory, "ParallelRDC", DC, 3);
}

void CommonDataModelTest::ParallelRSINTest(const std::string& outputDirectory)
{
  TestSetup10ParallelRSIN();
  RunTest(outputDirectory, "ParallelRSIN", SIN, 3);
}

void CommonDataModelTest::ParallelRPULSETest(const std::string& outputDirectory)
{
  TestSetup10ParallelRPULSE();
  RunTest(outputDirectory, "ParallelRPULSE", PULSE, 3);
}

void CommonDataModelTest::ParallelRCDCTest(const std::string& outputDirectory)
{
  TestSetup11ParallelRCDC();
  RunTest(outputDirectory, "ParallelRCDC", DC, 4);
}

void CommonDataModelTest::ParallelRCSINTest(const std::string& outputDirectory)
{
  TestSetup11ParallelRCSIN();
  RunTest(outputDirectory, "ParallelRCSIN", SIN, 4);
}

void CommonDataModelTest::ParallelRCPULSETest(const std::string& outputDirectory)
{
  TestSetup11ParallelRCPULSE();
  RunTest(outputDirectory, "ParallelRCPULSE", PULSE, 4);
}

void CommonDataModelTest::ParallelRLDCTest(const std::string& outputDirectory)
{
  TestSetup12ParallelRLDC();
  RunTest(outputDirectory, "ParallelRLDC", DC, 5);
}

void CommonDataModelTest::ParallelRLSINTest(const std::string& outputDirectory)
{
  TestSetup12ParallelRLSIN();
  RunTest(outputDirectory, "ParallelRLSIN", SIN, 5);
}

void CommonDataModelTest::ParallelRLPULSETest(const std::string& outputDirectory)
{
  TestSetup12ParallelRLPULSE();
  RunTest(outputDirectory, "ParallelRLPULSE", PULSE, 5);
}

void CommonDataModelTest::ParallelRLSINCenteredTest(const std::string& outputDirectory)
{
  TestSetup12ParallelRLSINCentered();
  RunTest(outputDirectory, "ParallelRLSINCentered", ZEROCENTEREDSIN, 5);
}

void CommonDataModelTest::SeriesRLCDCTest(const std::string& outputDirectory)
{
  TestSetup13SeriesRLCDC();
  RunTest(outputDirectory, "SeriesRLCDC", DC, 6);
}

void CommonDataModelTest::SeriesRLCSINTest(const std::string& outputDirectory)
{
  TestSetup13SeriesRLCSIN();
  RunTest(outputDirectory, "SeriesRLCSIN", SIN, 6);
}

void CommonDataModelTest::SeriesRLCPULSETest(const std::string& outputDirectory)
{
  TestSetup13SeriesRLCPULSE();
  RunTest(outputDirectory, "SeriesRLCPULSE", PULSE, 6);
}

void CommonDataModelTest::ParallelRLCDCTest(const std::string& outputDirectory)
{
  TestSetup14ParallelRLCDC();
  RunTest(outputDirectory, "ParallelRLCDC", DC, 7);
}

void CommonDataModelTest::ParallelRLCSINTest(const std::string& outputDirectory)
{
  TestSetup14ParallelRLCSIN();
  RunTest(outputDirectory, "ParallelRLCSIN", SIN, 7);
}

void CommonDataModelTest::ParallelRLCPULSETest(const std::string& outputDirectory)
{
  TestSetup14ParallelRLCPULSE();
  RunTest(outputDirectory, "ParallelRLCPULSE", PULSE, 7);
}

void CommonDataModelTest::SwitchRCDCTest(const std::string& outputDirectory)
{
  TestSetup15SwitchRCDC();
  RunTest(outputDirectory, "SwitchRCDC", DC, 8);
}

void CommonDataModelTest::SwitchRCSINTest(const std::string& outputDirectory)
{
  TestSetup15SwitchRCSIN();
  RunTest(outputDirectory, "SwitchRCSIN", SIN, 8);
}

void CommonDataModelTest::SwitchRCPULSETest(const std::string& outputDirectory)
{
  TestSetup15SwitchRCPULSE();
  RunTest(outputDirectory, "SwitchRCPULSE", PULSE, 8);
}

void CommonDataModelTest::Comprehensive1DCTest(const std::string& outputDirectory)
{
  TestSetup16Comprehensive1DC();
  RunTest(outputDirectory, "Comprehensive1DC", DC, 9);
}

void CommonDataModelTest::Comprehensive1SINTest(const std::string& outputDirectory)
{
  TestSetup16Comprehensive1SIN();
  RunTest(outputDirectory, "Comprehensive1SIN", SIN, 9);
}

void CommonDataModelTest::Comprehensive1PULSETest(const std::string& outputDirectory)
{
  TestSetup16Comprehensive1PULSE();
  RunTest(outputDirectory, "Comprehensive1PULSE", PULSE, 9);
}

void CommonDataModelTest::SimpleDiodeDCTest(const std::string& outputDirectory)
{
  TestSetup17BasicDiodeDC();
  RunTest(outputDirectory, "SimpleDiodeDC", DC, 10);
}

void CommonDataModelTest::SimpleDiodeSINTest(const std::string& outputDirectory)
{
  TestSetup17BasicDiodeSIN();
  RunTest(outputDirectory, "SimpleDiodeSIN", SIN, 10);
}

void CommonDataModelTest::SimpleDiodePULSETest(const std::string& outputDirectory)
{
  TestSetup17BasicDiodePULSE();
  RunTest(outputDirectory, "SimpleDiodePULSE", PULSE, 10);
}

void CommonDataModelTest::SeriesRCDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup18RCSeriesDCCurrent();
  RunTest(outputDirectory, "SeriesRCDCCurrent", DC, 11);
}

void CommonDataModelTest::SeriesRCSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup18RCSeriesSINCurrent();
  RunTest(outputDirectory, "SeriesRCSINCurrent", SIN, 11);
}

void CommonDataModelTest::SeriesRCPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup18RCSeriesPulseCurrent();
  RunTest(outputDirectory, "SeriesRCPULSECurrent", PULSE, 11);
}

void CommonDataModelTest::ParallelRCDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup19RCParallelDCCurrent();
  RunTest(outputDirectory, "ParallelRCDCCurrent", DC, 12);
}

void CommonDataModelTest::ParallelRCSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup19RCParallelSINCurrent();
  RunTest(outputDirectory, "ParallelRCSINCurrent", SIN, 12);
}

void CommonDataModelTest::ParallelRCPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup19RCParallelPulseCurrent();
  RunTest(outputDirectory, "ParallelRCPULSECurrent", PULSE, 12);
}

void CommonDataModelTest::SeriesCapDCTest(const std::string& outputDirectory)
{
  TestSetup20SeriesCapDC();
  RunTest(outputDirectory, "SeriesCapDC", DC, 13);
}

void CommonDataModelTest::SeriesCapSINTest(const std::string& outputDirectory)
{
  TestSetup20SeriesCapSIN();
  RunTest(outputDirectory, "SeriesCapSIN", SIN, 13);
}

void CommonDataModelTest::SeriesCapPULSETest(const std::string& outputDirectory)
{
  TestSetup20SeriesCapPulse();
  RunTest(outputDirectory, "SeriesCapPULSE", PULSE, 13);
}

void CommonDataModelTest::ParallelRDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup21ParallelRDCCurrent();
  RunTest(outputDirectory, "ParallelRDCCurrent", DC, 14);
}

void CommonDataModelTest::ParallelRSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup21ParallelRSINCurrent();
  RunTest(outputDirectory, "ParallelRSINCurrent", SIN, 14);
}

void CommonDataModelTest::ParallelRPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup21ParallelRPulseCurrent();
  RunTest(outputDirectory, "ParallelRPULSECurrent", PULSE, 14);
}

void CommonDataModelTest::CurrentCompDCTest(const std::string& outputDirectory)
{
  TestSetup22CurrentCompDC();
  RunTest(outputDirectory, "CurrentCompDC", DC, 15);
}

void CommonDataModelTest::CurrentCompSINTest(const std::string& outputDirectory)
{
  TestSetup22CurrentCompSIN();
  RunTest(outputDirectory, "CurrentCompSIN", SIN, 15);
}

void CommonDataModelTest::CurrentCompPULSETest(const std::string& outputDirectory)
{
  TestSetup22CurrentCompPulse();
  RunTest(outputDirectory, "CurrentCompPULSE", PULSE, 15);
}

void CommonDataModelTest::SeriesRLDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup23SeriesRLDCCurrent();
  RunTest(outputDirectory, "SeriesRLDCCurrent", DC, 16);
}

void CommonDataModelTest::SeriesRLSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup23SeriesRLSINCurrent();
  RunTest(outputDirectory, "SeriesRLSINCurrent", SIN, 16);
}

void CommonDataModelTest::SeriesRLPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup23SeriesRLPULSECurrent();
  RunTest(outputDirectory, "SeriesRLPULSECurrent", PULSE, 16);
}

void CommonDataModelTest::ParallelRLDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup24ParallelRLDCCurrent();
  RunTest(outputDirectory, "ParallelRLDCCurrent", DC, 17);
}

void CommonDataModelTest::ParallelRLSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup24ParallelRLSINCurrent();
  RunTest(outputDirectory, "ParallelRLSINCurrent", SIN, 17);
}

void CommonDataModelTest::ParallelRLPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup24ParallelRLPULSECurrent();
  RunTest(outputDirectory, "ParallelRLPULSECurrent", PULSE, 17);
}

void CommonDataModelTest::BasicDiodeDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup25BasicDiodeDCCurrent();
  RunTest(outputDirectory, "BasicDiodeDCCurrent", DC, 18);
}

void CommonDataModelTest::BasicDiodeSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup25BasicDiodeSINCurrent();
  RunTest(outputDirectory, "BasicDiodeSINCurrent", SIN, 18);
}

void CommonDataModelTest::BasicDiodePULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup25BasicDiodePULSECurrent();
  RunTest(outputDirectory, "BasicDiodePULSECurrent", PULSE, 18);
}

//NOTE: These next three tests seem to give errors while running, but they produce output...
void CommonDataModelTest::SwitchRCDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup26SwitchRCDCCurrent();
  RunTest(outputDirectory, "SwitchRCDCCurrent", DC, 19);
}

void CommonDataModelTest::SwitchRCSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup26SwitchRCSINCurrent();
  RunTest(outputDirectory, "SwitchRCSINCurrent", SIN, 19);
}

void CommonDataModelTest::SwitchRCPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup26SwitchRCPULSECurrent();
  RunTest(outputDirectory, "SwitchRCPULSECurrent", PULSE, 19);
}

void CommonDataModelTest::SeriesRLCDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup27SeriesRLCDCCurrent();
  RunTest(outputDirectory, "SeriesRLCDCCurrent", DC, 20);
}

void CommonDataModelTest::SeriesRLCSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup27SeriesRLCSINCurrent();
  RunTest(outputDirectory, "SeriesRLCSINCurrent", SIN, 20);
}

void CommonDataModelTest::SeriesRLCPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup27SeriesRLCPULSECurrent();
  RunTest(outputDirectory, "SeriesRLCPULSECurrent", PULSE, 20);
}

void CommonDataModelTest::ParallelRLCDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup28ParallelRLCDCCurrent();
  RunTest(outputDirectory, "ParallelRLCDCCurrent", DC, 21);
}

void CommonDataModelTest::ParallelRLCSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup28ParallelRLCSINCurrent();
  RunTest(outputDirectory, "ParallelRLCSINCurrent", SIN, 21);
}

void CommonDataModelTest::ParallelRLCPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup28ParallelRLCPULSECurrent();
  RunTest(outputDirectory, "ParallelRLCPULSECurrent", PULSE, 21);
}

void CommonDataModelTest::SeriesPressureSourceAdditionDCTest(const std::string& outputDirectory)
{
  TestSetup29SeriesPressureSourceAdditionDC();
  RunTest(outputDirectory, "SeriesPressureSourceAdditionDC", DC, 22);
}

void CommonDataModelTest::SeriesPressureSourceAdditionSINTest(const std::string& outputDirectory)
{
  TestSetup29SeriesPressureSourceAdditionSIN();
  RunTest(outputDirectory, "SeriesPressureSourceAdditionSIN", SIN, 22);
}

void CommonDataModelTest::SeriesPressureSourceAdditionPULSETest(const std::string& outputDirectory)
{
  TestSetup29SeriesPressureSourceAdditionPULSE();
  RunTest(outputDirectory, "SeriesPressureSourceAdditionPULSE", PULSE, 22);
}

//NOTE: These next six tests seem to give errors while running, but it's intentional
void CommonDataModelTest::SeriesCurrentSourceAdditionDCTest(const std::string& outputDirectory)
{
  TestSetup30SeriesCurrentSourceAdditionDC();
  RunTest(outputDirectory, "SeriesCurrentSourceAdditionDC", DC, 23);
}

void CommonDataModelTest::SeriesCurrentSourceAdditionSINTest(const std::string& outputDirectory)
{
  TestSetup30SeriesCurrentSourceAdditionSIN();
  RunTest(outputDirectory, "SeriesCurrentSourceAdditionSIN", SIN, 23);
}

void CommonDataModelTest::SeriesCurrentSourceAdditionPULSETest(const std::string& outputDirectory)
{
  TestSetup30SeriesCurrentSourceAdditionPULSE();
  RunTest(outputDirectory, "SeriesCurrentSourceAdditionPULSE", PULSE, 23);
}

void CommonDataModelTest::ParallelPressureSourceAdditionDCTest(const std::string& outputDirectory)
{
  TestSetup31ParallelPressureSourceAdditionDC();
  RunTest(outputDirectory, "ParallelPressureSourceAdditionDC", DC, 24);
}

void CommonDataModelTest::ParallelPressureSourceAdditionSINTest(const std::string& outputDirectory)
{
  TestSetup31ParallelPressureSourceAdditionSIN();
  RunTest(outputDirectory, "ParallelPressureSourceAdditionSIN", SIN, 24);
}

void CommonDataModelTest::ParallelPressureSourceAdditionPULSETest(const std::string& outputDirectory)
{
  TestSetup31ParallelPressureSourceAdditionPULSE();
  RunTest(outputDirectory, "ParallelPressureSourceAdditionPULSE", PULSE, 24);
}

void CommonDataModelTest::ParallelCurrentSourceAdditionDCTest(const std::string& outputDirectory)
{
  TestSetup32ParallelCurrentSourceAdditionDC();
  RunTest(outputDirectory, "ParallelCurrentSourceAdditionDC", DC, 25);
}

void CommonDataModelTest::ParallelCurrentSourceAdditionSINTest(const std::string& outputDirectory)
{
  TestSetup32ParallelCurrentSourceAdditionSIN();
  RunTest(outputDirectory, "ParallelCurrentSourceAdditionSIN", SIN, 25);
}

void CommonDataModelTest::ParallelCurrentSourceAdditionPULSETest(const std::string& outputDirectory)
{
  TestSetup32ParallelCurrrentSourceAdditionPULSE();
  RunTest(outputDirectory, "ParallelCurrentSourceAdditionPULSE", PULSE, 25);
}

void CommonDataModelTest::SeriesCapDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup33SeriesCapDCCurrent();
  RunTest(outputDirectory, "SeriesCapDCCurrent", DC, 26);
}

void CommonDataModelTest::SeriesCapSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup33SeriesCapSINCurrent();
  RunTest(outputDirectory, "SeriesCapSINCurrent", SIN, 26);
}

void CommonDataModelTest::SeriesCapPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup33SeriesCapPulseCurrent();
  RunTest(outputDirectory, "SeriesCapPULSECurrent", PULSE, 26);
}

void CommonDataModelTest::ParallelCapDCTest(const std::string& outputDirectory)
{
  TestSetup34ParallelCapDC();
  RunTest(outputDirectory, "ParallelCapDC", DC, 27);
}

void CommonDataModelTest::ParallelCapSINTest(const std::string& outputDirectory)
{
  TestSetup34ParallelCapSIN();
  RunTest(outputDirectory, "ParallelCapSIN", SIN, 27);
}

void CommonDataModelTest::ParallelCapPULSETest(const std::string& outputDirectory)
{
  TestSetup34ParallelCapPulse();
  RunTest(outputDirectory, "ParallelCapPULSE", PULSE, 27);
}

void CommonDataModelTest::ParallelCapDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup35ParallelCapDCCurrent();
  RunTest(outputDirectory, "ParallelCapDCCurrent", DC, 28);
}

void CommonDataModelTest::ParallelCapSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup35ParallelCapSINCurrent();
  RunTest(outputDirectory, "ParallelCapSINCurrent", SIN, 28);
}

void CommonDataModelTest::ParallelCapPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup35ParallelCapPulseCurrent();
  RunTest(outputDirectory, "ParallelCapPULSECurrent", PULSE, 28);
}

void CommonDataModelTest::SeriesIndDCTest(const std::string& outputDirectory)
{
  TestSetup36SeriesIndDC();
  RunTest(outputDirectory, "SeriesIndDC", DC, 29);
}

void CommonDataModelTest::SeriesIndSINTest(const std::string& outputDirectory)
{
  TestSetup36SeriesIndSIN();
  RunTest(outputDirectory, "SeriesIndSIN", SIN, 29);
}

void CommonDataModelTest::SeriesIndPULSETest(const std::string& outputDirectory)
{
  TestSetup36SeriesIndPulse();
  RunTest(outputDirectory, "SeriesIndPULSE", PULSE, 29);
}

void CommonDataModelTest::SeriesIndDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup37SeriesIndDCCurrent();
  RunTest(outputDirectory, "SeriesIndDCCurrent", DC, 30);
}

void CommonDataModelTest::SeriesIndSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup37SeriesIndSINCurrent();
  RunTest(outputDirectory, "SeriesIndSINCurrent", SIN, 30);
}

void CommonDataModelTest::SeriesIndPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup37SeriesIndPulseCurrent();
  RunTest(outputDirectory, "SeriesIndPULSECurrent", PULSE, 30);
}

void CommonDataModelTest::ParallelIndDCTest(const std::string& outputDirectory)
{
  TestSetup38ParallelIndDC();
  RunTest(outputDirectory, "ParallelIndDC", DC, 31);
}

void CommonDataModelTest::ParallelIndSINTest(const std::string& outputDirectory)
{
  TestSetup38ParallelIndSIN();
  RunTest(outputDirectory, "ParallelIndSIN", SIN, 31);
}

void CommonDataModelTest::ParallelIndPULSETest(const std::string& outputDirectory)
{
  TestSetup38ParallelIndPulse();
  RunTest(outputDirectory, "ParallelIndPULSE", PULSE, 31);
}

void CommonDataModelTest::ParallelIndDCCurrentTest(const std::string& outputDirectory)
{
  TestSetup39ParallelIndDCCurrent();
  RunTest(outputDirectory, "ParallelIndDCCurrent", DC, 32);
}

void CommonDataModelTest::ParallelIndSINCurrentTest(const std::string& outputDirectory)
{
  TestSetup39ParallelIndSINCurrent();
  RunTest(outputDirectory, "ParallelIndSINCurrent", SIN, 32);
}

void CommonDataModelTest::ParallelIndPULSECurrentTest(const std::string& outputDirectory)
{
  TestSetup39ParallelIndPulseCurrent();
  RunTest(outputDirectory, "ParallelIndPULSECurrent", PULSE, 32);
}

void CommonDataModelTest::BadDiodeDCTest(const std::string& outputDirectory)
{
  TestSetup40BadDiodeDC();
  RunTest(outputDirectory, "BadDiodeDC", DC, 33);
}

void CommonDataModelTest::BadDiodeSINTest(const std::string& outputDirectory)
{
  TestSetup40BadDiodeSIN();
  RunTest(outputDirectory, "BadDiodeSIN", SIN, 33);
}

void CommonDataModelTest::BadDiodePULSETest(const std::string& outputDirectory)
{
  TestSetup40BadDiodePulse();
  RunTest(outputDirectory, "BadDiodePULSE", PULSE, 33);
}

void CommonDataModelTest::ValidationComprehensive1DCTest(const std::string& outputDirectory)
{
  TestSetup41ValidationComprehensive1DC();
  RunTest(outputDirectory, "ValidationComprehensive1DC", DC, 34);
}

void CommonDataModelTest::ValidationComprehensive1SINTest(const std::string& outputDirectory)
{
  TestSetup41ValidationComprehensive1SIN();
  RunTest(outputDirectory, "ValidationComprehensive1SIN", SIN, 34);
}

void CommonDataModelTest::ValidationComprehensive1PULSETest(const std::string& outputDirectory)
{
  TestSetup41ValidationComprehensive1Pulse();
  RunTest(outputDirectory, "ValidationComprehensive1PULSE", PULSE, 34);
}

void CommonDataModelTest::ValidationComprehensive1SINCenteredTest(const std::string& outputDirectory)
{
  TestSetup41ValidationComprehensive1SINCentered();
  RunTest(outputDirectory, "ValidationComprehensive1SINCentered", ZEROCENTEREDSIN, 34);
}

void CommonDataModelTest::ValidationComprehensive2DCTest(const std::string& outputDirectory)
{
  TestSetup42ValidationComprehensive2DC();
  RunTest(outputDirectory, "ValidationComprehensive2DC", DC, 35);
}

void CommonDataModelTest::ValidationComprehensive2SINTest(const std::string& outputDirectory)
{
  TestSetup42ValidationComprehensive2SIN();
  RunTest(outputDirectory, "ValidationComprehensive2SIN", SIN, 35);
}

void CommonDataModelTest::ValidationComprehensive2PULSETest(const std::string& outputDirectory)
{
  TestSetup42ValidationComprehensive2Pulse();
  RunTest(outputDirectory, "ValidationComprehensive2PULSE", PULSE, 35);
}

void CommonDataModelTest::ValidationComprehensive2SINCenteredTest(const std::string& outputDirectory)
{
  TestSetup42ValidationComprehensive2SINCentered();
  RunTest(outputDirectory, "ValidationComprehensive2SINCentered", ZEROCENTEREDSIN, 35);
}

void CommonDataModelTest::RunTest(const std::string& outputDirectory, const std::string& testName, enumCDMTestSourceType sourceType, int testIndex)
{
  m_Logger = new Logger(outputDirectory + "/" + testName + ".log");
  double timeStep_s = 1.0 / 165.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
  SEFluidCircuitCalculator fluidCalculator(DefaultFluidCircuitCalculatorUnits, m_Logger);
  SEFluidCircuit* fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");

  //Set up the circuit
  //Circuit setup should be done before RunTest is called
  fluidCircuit->SetNextAndCurrentFromBaselines();
  fluidCircuit->StateChange();

  bool serialized = false;
  double outputTime_s = 0;
  while (currentTime_s < 10) // Run for 10 secs
  {
    //PreProcess
    switch (sourceType)
    {
      case DC:
        TestPreProcess1(currentTime_s, testIndex);
        break;
      case SIN:
        TestPreProcess2(currentTime_s, testIndex);
        break;
      case PULSE:
        TestPreProcess3(currentTime_s, testIndex);
        break;
      case ZEROCENTEREDSIN:
        TestPreProcess4(currentTime_s, testIndex);
        break;
      default:
        break;
    }
    
    //Process
    fluidCalculator.Process(*fluidCircuit, timeStep_s);

    //PostProcess
    fluidCalculator.PostProcess(*fluidCircuit);

    //Record and Output
    trk1.Track(currentTime_s, *fluidCircuit);

    if (!serialized && currentTime_s > 8)
    {
      serialized = true;
      std::string jsonDir = outputDirectory + "/" + testName + ".json";
      TestCircuitSerialization(jsonDir); 
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }

    currentTime_s += timeStep_s;
    outputTime_s += timeStep_s;

    if (outputTime_s >= 10) // outputs every 10 secs
    {
      outputTime_s = 0;
      std::string sOutputFile = outputDirectory + "/" + testName + ".csv";
      trk1.WriteTrackToFile(sOutputFile.c_str());
    }
  }
  m_Circuits->Clear();
}

void CommonDataModelTest::TestCircuitSerialization(const std::string& fileName)
{
  PBCircuit::SaveCircuitManagerFile(*m_Circuits,fileName);
  if (!PBCircuit::LoadCircuitManagerFile(*m_Circuits,fileName))
  {
    m_Circuits->Clear();
    m_Circuits->Error("Unable to load file " + fileName);
  }
}
