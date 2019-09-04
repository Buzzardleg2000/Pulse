/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Respiratory.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "PulseConfiguration.h"
// Conditions 
#include "engine/SEConditionManager.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
// Actions
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEBreathHold.h"
#include "patient/actions/SEForcedExhale.h"
#include "patient/actions/SEForcedInhale.h"
#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SESupplementalOxygen.h"
#include "patient/actions/SETensionPneumothorax.h"
// Assessments
#include "patient/assessments/SEPulmonaryFunctionTest.h"
// Dependent Systems
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEnergySystem.h"
// CDM
#include "patient/SEPatient.h"
#include "engine/SEEventManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceAerosolization.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
// Properties
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarInversePressure.h"
#include "properties/SEScalarInverseVolume.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarNegative1To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SERunningAverage.h"

//jbw - remove
#include "utils/DataTrack.h"


#ifdef _MSC_VER 
#pragma warning( disable : 4305 4244 )  // Disable some warning messages
#endif

//Flag for setting things constant to test
//Should be commented out, unless debugging/tuning
//#define TUNING

Respiratory::Respiratory(PulseController& data) : SERespiratorySystem(data.GetLogger()), m_data(data)
{
  m_BloodPHRunningAverage = new SERunningAverage();
  m_ArterialO2RunningAverage_mmHg = new SERunningAverage();
  m_ArterialCO2RunningAverage_mmHg = new SERunningAverage();

  m_Calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
  m_GasTransporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());
  m_AerosolTransporter = new SELiquidTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger());
  Clear();
}

Respiratory::~Respiratory()
{
  Clear();
  delete m_BloodPHRunningAverage;
  delete m_ArterialO2RunningAverage_mmHg;
  delete m_ArterialCO2RunningAverage_mmHg;

  delete m_Calculator;
  delete m_GasTransporter;
  delete m_AerosolTransporter;
}

void Respiratory::Clear()
{
  SERespiratorySystem::Clear();
  m_Patient = nullptr;
  m_PatientActions = nullptr;

  m_Environment = nullptr;
  m_AerosolMouth = nullptr;
  m_AerosolCarina = nullptr;
  m_AerosolLeftAnatomicDeadSpace = nullptr;
  m_AerosolLeftAlveolarDeadSpace = nullptr;
  m_AerosolLeftAlveoli = nullptr;
  m_AerosolRightAnatomicDeadSpace = nullptr;
  m_AerosolRightAlveolarDeadSpace = nullptr;
  m_AerosolRightAlveoli = nullptr;
  m_Lungs = nullptr;
  m_LeftLungExtravascular = nullptr;
  m_RightLungExtravascular = nullptr;
  m_Carina = nullptr;
  m_AortaO2 = nullptr;
  m_AortaCO2 = nullptr;
  m_MechanicalVentilatorConnection = nullptr;
  m_MechanicalVentilatorAerosolConnection = nullptr;
  m_PleuralCavity = nullptr;

  m_RespiratoryCircuit = nullptr;

  m_Mouth = nullptr;
  m_LeftAlveoli = nullptr;
  m_LeftAnatomicDeadSpace = nullptr;
  m_LeftAlveolarDeadSpace = nullptr;
  m_LeftPleural = nullptr;
  m_RespiratoryMuscle = nullptr;
  m_RightAlveoli = nullptr;
  m_RightAnatomicDeadSpace = nullptr;
  m_RightAlveolarDeadSpace = nullptr;
  m_RightPleural = nullptr;

  m_CarinaToLeftAnatomicDeadSpace = nullptr;
  m_CarinaToRightAnatomicDeadSpace = nullptr;
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = nullptr;
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace = nullptr;
  m_LeftAlveolarDeadSpaceToLeftAlveoli = nullptr;
  m_RightAlveolarDeadSpaceToRightAlveoli = nullptr;
  m_RightPleuralToRespiratoryMuscle = nullptr;
  m_LeftPleuralToRespiratoryMuscle = nullptr;
  m_DriverPressurePath = nullptr;
  m_MouthToCarina = nullptr;
  m_MouthToStomach = nullptr;
  m_EnvironmentToLeftChestLeak = nullptr;
  m_EnvironmentToRightChestLeak = nullptr;
  m_LeftAlveoliLeakToLeftPleural = nullptr;
  m_RightAlveoliLeakToRightPleural = nullptr;
  m_LeftPleuralToEnvironment = nullptr;
  m_RightPleuralToEnvironment = nullptr;
  m_RightAlveoliToRightPleuralConnection = nullptr;
  m_LeftAlveoliToLeftPleuralConnection = nullptr;
  m_RightPulmonaryCapillary = nullptr;
  m_LeftPulmonaryCapillary = nullptr;
  m_ConnectionToMouth = nullptr;
  m_GroundToConnection = nullptr;

  m_BloodPHRunningAverage->Clear();
  m_ArterialO2RunningAverage_mmHg->Clear();
  m_ArterialCO2RunningAverage_mmHg->Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Respiratory::Initialize()
{
  PulseSystem::Initialize();

  //Vital signs
  m_NotBreathing = false;
  m_TopBreathTotalVolume_L = 0.0;
  m_TopBreathAlveoliVolume_L = 0.0;
  m_TopBreathPleuralVolume_L = 0.0;
  m_TopBreathPleuralPressure_cmH2O = 0.0;
  m_TopBreathAlveoliPressure_cmH2O = 0.0;
  m_TopBreathDriverPressure_cmH2O = 0.0;
  m_LastCardiacCycleBloodPH = 7.4;
  m_TopCarinaO2 = 0.2;
  m_TopBreathElapsedTime_min = 0.0;
  m_BottomBreathElapsedTime_min = 0.0;
  m_BottomBreathTotalVolume_L = 0.0;
  m_BottomBreathAlveoliVolume_L = m_BottomBreathTotalVolume_L;
  m_BottomBreathPleuralVolume_L = 0.0;
  m_BottomBreathPleuralPressure_cmH2O = 0.0;
  m_BottomBreathAlveoliPressure_cmH2O = 0.0;
  m_BottomBreathDriverPressure_cmH2O = 0.0;
  m_PeakAlveolarPressure_cmH2O = 0.0;
  m_MaximalAlveolarPressure_cmH2O = 0.0;

  //Driver
  m_MaxDriverPressure_cmH2O = -50.0;
  m_ElapsedBreathingCycleTime_min = 0.0;
  m_TopBreathElapsedTime_min = 0.0;
  m_BreathingCycle = false;
  m_BreathingCycleTime_s = 0.0;
  m_VentilationFrequency_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
  m_DriverPressure_cmH2O = 0.0;
  m_DriverPressureMin_cmH2O = 0.0;
  m_VentilationToTidalVolumeSlope = 30.0;
  //The peak driver pressure is the pressure above the default pressure
  m_PeakRespiratoryDrivePressure_cmH2O = 0.0;
  m_ArterialO2PartialPressure_mmHg = m_AortaO2->GetPartialPressure(PressureUnit::mmHg);
  m_ArterialCO2PartialPressure_mmHg = m_AortaCO2->GetPartialPressure(PressureUnit::mmHg);
  m_PreviousTargetAlveolarVentilation_L_Per_min = m_Patient->GetTidalVolumeBaseline(VolumeUnit::L) * m_VentilationFrequency_Per_min;
  m_AverageLocalTissueBronchodilationEffects = 0.0;

  m_IERatioScaleFactor = 1.0;

  // Conscious Respiration
  m_ConsciousRespirationPeriod_s = 0.0;
  m_ConsciousRespirationRemainingPeriod_s = 0.0;
  m_ExpiratoryReserveVolumeFraction = -1.0;
  m_InspiratoryCapacityFraction = -1.0;
  m_ConsciousStartPressure_cmH2O = 0.0;
  m_ConsciousEndPressure_cmH2O = 0.0;
  m_ConsciousBreathing = false;

  //Patient data
  m_InitialExpiratoryReserveVolume_L = m_Patient->GetExpiratoryReserveVolume(VolumeUnit::L);
  m_InitialInspiratoryCapacity_L = m_Patient->GetInspiratoryCapacity(VolumeUnit::L);
  m_InitialFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);
  m_InitialResidualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  m_InstantaneousFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);
  m_InitialTotalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L);

  //System data
  double TidalVolume_L = m_Patient->GetTidalVolumeBaseline(VolumeUnit::L);
  double RespirationRate_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
  GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);
  GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
  GetCarricoIndex().SetValue(452.0, PressureUnit::mmHg);

  double AnatomicDeadSpace_L = m_LeftAnatomicDeadSpace->GetVolumeBaseline(VolumeUnit::L) + m_RightAnatomicDeadSpace->GetVolumeBaseline(VolumeUnit::L);
  double AlveolarDeadSpace_L = 0.0;
  GetAnatomicDeadSpace().SetValue(AnatomicDeadSpace_L, VolumeUnit::L);
  GetAlveolarDeadSpace().SetValue(AlveolarDeadSpace_L, VolumeUnit::L);
  GetPhysiologicDeadSpace().SetValue(AnatomicDeadSpace_L + AlveolarDeadSpace_L, VolumeUnit::L);

  GetTotalAlveolarVentilation().SetValue(RespirationRate_Per_min * TidalVolume_L, VolumePerTimeUnit::L_Per_min);
  GetTotalPulmonaryVentilation().SetValue(RespirationRate_Per_min * TidalVolume_L, VolumePerTimeUnit::L_Per_min);
  GetTotalDeadSpaceVentilation().SetValue(AnatomicDeadSpace_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
  GetSpecificVentilation().SetValue(0.21);

  GetEndTidalCarbonDioxideFraction().SetValue(0.0827);
  GetEndTidalCarbonDioxidePressure().SetValue(0.0, PressureUnit::mmHg);
  GetEndTidalOxygenFraction().SetValue(0.0);
  GetEndTidalOxygenPressure().SetValue(0.0, PressureUnit::mmHg);

  GetIntrapleuralPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransrespiratoryPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransairwayPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTranspulmonaryPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransalveolarPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransthoracicPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransChestWallPressure().SetValue(0.0, PressureUnit::cmH2O);

  GetPulmonaryCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetLungCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetChestWallCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetPulmonaryElastance().SetValue(1.0 / 0.1, PressurePerVolumeUnit::cmH2O_Per_L);

  // Muscle Pressure Waveform
  m_InspiratoryRiseFraction = 0;
  m_InspiratoryHoldFraction = 0;
  m_InspiratoryReleaseFraction = 0;
  m_InspiratoryToExpiratoryPauseFraction = 0;
  m_ExpiratoryRiseFraction = 0;
  m_ExpiratoryHoldFraction = 0;
  m_ExpiratoryReleaseFraction = 0;
  m_ResidueFraction = 0;

  //Get the fluid mechanics to a good starting point
  TuneCircuit();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Respiratory Class
///
///  \details
///   Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void Respiratory::SetUp()
{
  //Time Step
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_dt_min = m_data.GetTimeStep().GetValue(TimeUnit::min);
  //Patient
  m_Patient = &m_data.GetPatient();
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  //Configuration parameters
  m_DefaultOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_DefaultClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultClosedFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RespOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryOpenResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RespClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryClosedResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_PeripheralControlGainConstant = m_data.GetConfiguration().GetPeripheralVentilatoryControllerGain();
  m_CentralControlGainConstant = m_data.GetConfiguration().GetCentralVentilatoryControllerGain();
  m_VentilationTidalVolumeIntercept = m_data.GetConfiguration().GetVentilationTidalVolumeIntercept(VolumeUnit::L);
  m_VentilatoryOcclusionPressure_cmH2O = m_data.GetConfiguration().GetVentilatoryOcclusionPressure(PressureUnit::cmH2O); //This increases the absolute max driver pressure
  m_PleuralComplianceSensitivity_Per_L = m_data.GetConfiguration().GetPleuralComplianceSensitivity(InverseVolumeUnit::Inverse_L);
  m_MinimumAllowableTidalVolume_L = m_data.GetConfiguration().GetMinimumAllowableTidalVolume(VolumeUnit::L);
  m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s = m_data.GetConfiguration().GetMinimumAllowableInpiratoryAndExpiratoryPeriod(TimeUnit::s);
  //Compartments
  m_Environment = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  m_AerosolMouth = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Mouth);
  m_AerosolCarina = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Carina);
  m_AerosolLeftAnatomicDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAnatomicDeadSpace);
  m_AerosolLeftAlveolarDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveolarDeadSpace);
  m_AerosolLeftAlveoli = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveoli);
  m_AerosolRightAnatomicDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAnatomicDeadSpace);
  m_AerosolRightAlveolarDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveolarDeadSpace);
  m_AerosolRightAlveoli = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli);
  m_Lungs = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Lungs);
  m_LeftLung = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::LeftLung);
  m_RightLung = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::RightLung);
  m_PleuralCavity = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::PleuralCavity);
  m_LeftLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(pulse::ExtravascularCompartment::LeftLungIntracellular);
  m_RightLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(pulse::ExtravascularCompartment::RightLungIntracellular);
  m_Carina = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
  m_CarinaO2 = m_Carina->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  SELiquidCompartment* Aorta = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta);
  m_AortaO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_AortaCO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_LeftAlveoliO2 = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::LeftAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_RightAlveoliO2 = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_MechanicalVentilatorConnection = m_data.GetCompartments().GetGasCompartment(pulse::MechanicalVentilatorCompartment::Connection);
  m_MechanicalVentilatorAerosolConnection = m_data.GetCompartments().GetLiquidCompartment(pulse::MechanicalVentilatorCompartment::Connection);
  // Compartments we will process aerosol effects on
  m_AerosolEffects.clear();
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Carina));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAnatomicDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveolarDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAnatomicDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveolarDeadSpace));
  //Circuits
  m_RespiratoryCircuit = &m_data.GetCircuits().GetRespiratoryCircuit();
  //Nodes
  m_Mouth = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::Mouth);
  m_LeftAlveoli = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAlveoli);
  m_LeftAnatomicDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAnatomicDeadSpace);
  m_LeftAlveolarDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAlveolarDeadSpace);
  m_LeftPleural = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftPleural);
  m_RespiratoryMuscle = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RespiratoryMuscle);
  m_RightAlveoli = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAlveoli);
  m_RightAnatomicDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAnatomicDeadSpace);
  m_RightAlveolarDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAlveolarDeadSpace);
  m_RightPleural = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightPleural);
  m_Ambient = m_RespiratoryCircuit->GetNode(pulse::EnvironmentNode::Ambient);
  m_Stomach = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::Stomach);
  //Paths
  m_CarinaToLeftAnatomicDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::CarinaToLeftAnatomicDeadSpace);
  m_CarinaToRightAnatomicDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::CarinaToRightAnatomicDeadSpace);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  m_LeftAlveolarDeadSpaceToLeftAlveoli = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveolarDeadSpaceToLeftAlveoli);
  m_RightAlveolarDeadSpaceToRightAlveoli = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveolarDeadSpaceToRightAlveoli);
  m_RightPleuralToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightPleuralToRespiratoryMuscle);
  m_LeftPleuralToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftPleuralToRespiratoryMuscle);
  m_DriverPressurePath = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  m_MouthToCarina = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::MouthToCarina);
  m_MouthToStomach = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::MouthToStomach);
  m_EnvironmentToLeftChestLeak = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToLeftChestLeak);
  m_EnvironmentToRightChestLeak = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToRightChestLeak);
  m_LeftAlveoliLeakToLeftPleural = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveoliLeakToLeftPleural);
  m_RightAlveoliLeakToRightPleural = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveoliLeakToRightPleural);
  m_LeftPleuralToEnvironment = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftPleuralToEnvironment);
  m_RightPleuralToEnvironment = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightPleuralToEnvironment);
  m_RightAlveoliToRightPleuralConnection = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveoliToRightPleuralConnection);
  m_LeftAlveoliToLeftPleuralConnection = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveoliToLeftPleuralConnection);
  m_ConnectionToMouth = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::ConnectionToMouth);
  m_GroundToConnection = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::GroundToConnection);

  /// \todo figure out how to modify these resistances without getting the cv circuit - maybe add a parameter, like baroreceptors does
  m_RightPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
  m_LeftPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);

  //jbw - Remove this and do it with the new healthy patient stored values
  m_initialAlveoliDiffusionArea_cm2 = m_Patient->GetAlveoliSurfaceArea(AreaUnit::cm2);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system at steady state
///
/// \details
/// Initializes respiratory conditions if any are present.
///  <UL>
///   <LI>COPD</LI>
///   <LI>Lobar Pneumonia</LI>
///   <LI>ImpairedAlveolarExchange</LI>
///  </UL>
///
//--------------------------------------------------------------------------------------------------
void Respiratory::AtSteadyState()
{
  //jbw - only reset these at healthy stabilization - make member variables for things that change - PFT: need to actually do it
  //jbw - FRC is known now that it's modeled correctly (make sure it can be overwritten) - only TV should be experimentally determined
  //jbw - Make sure CV also follow this methodology
  double respirationRate_Per_min = GetRespirationRate(FrequencyUnit::Per_min);
  double tidalVolume_L = GetTidalVolume(VolumeUnit::L);
  double totalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L);
  double residualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  double vitalCapacity_L = totalLungCapacity_L - residualVolume_L;
  double expiratoryReserveVolume_L = m_InstantaneousFunctionalResidualCapacity_L - residualVolume_L;
  double inspiratoryReserveVolume_L = totalLungCapacity_L - m_InstantaneousFunctionalResidualCapacity_L - tidalVolume_L;
  double inspiratoryCapacity_L = totalLungCapacity_L - m_InstantaneousFunctionalResidualCapacity_L;
  m_Patient->GetRespirationRateBaseline().SetValue(respirationRate_Per_min, FrequencyUnit::Per_min);
  m_Patient->GetTidalVolumeBaseline().SetValue(tidalVolume_L, VolumeUnit::L);
  m_Patient->GetFunctionalResidualCapacity().SetValue(m_InstantaneousFunctionalResidualCapacity_L, VolumeUnit::L);
  m_Patient->GetVitalCapacity().SetValue(vitalCapacity_L, VolumeUnit::L);
  m_Patient->GetExpiratoryReserveVolume().SetValue(expiratoryReserveVolume_L, VolumeUnit::L);
  m_Patient->GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume_L, VolumeUnit::L);
  m_Patient->GetInspiratoryCapacity().SetValue(inspiratoryCapacity_L, VolumeUnit::L);

  std::string typeString = "Initial Stabilization Homeostasis: ";
  if (m_data.GetState() == EngineState::AtSecondaryStableState)
    typeString = "Secondary Stabilization Homeostasis: ";

  std::stringstream ss;
  ss << typeString << "Patient respiration rate = " << respirationRate_Per_min << " bpm.";
  Info(ss);
  ss << typeString << "Patient tidal volume = " << tidalVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient functional residual capacity = " << m_InstantaneousFunctionalResidualCapacity_L << " L.";
  Info(ss);
  ss << typeString << "Patient vital capacity = " << vitalCapacity_L << " L.";
  Info(ss);
  ss << typeString << "Patient expiratory reserve volume = " << expiratoryReserveVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient inspiratory reserve volume = " << inspiratoryReserveVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient inspiratory capacity = " << inspiratoryCapacity_L << " L.";
  Info(ss);

  if (m_data.GetState() == EngineState::AtInitialStableState)
  {// At Resting State, apply conditions if we have them
    COPD();    
    LobarPneumonia();
  //These conditions stack effects
  //If combined, it will be a fraction of the already effected alveolar surface area
  ImpairedAlveolarExchange();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system preprocess function
///
/// \details
/// Calculates the muscle pressure source that drives the respiratory system.
/// Updates airway resistance to account for changes arising from factors 
/// like drugs and respiratory insults and interventions. 
//// Updates the chest wall variable compliance. Handles all respiratory
/// insults and actions.
//--------------------------------------------------------------------------------------------------
void Respiratory::PreProcess()
{
  UpdateChestWallCompliances();
  ProcessAerosolSubstances();
  AirwayObstruction();
  UpdateObstructiveResistance();
  BronchoConstriction();
  BronchoDilation();
  Intubation();
  Pneumothorax();  
  ConsciousRespiration();

  MechanicalVentilation();
  SupplementalOxygen();

  RespiratoryDriver();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system process function
///
/// \details
/// Ensures the selection of the respiratory system with or without the anesthesia machine. 
///  Handles the integration of the anesthesia machine to the respiratory system when the anesthesia machine is turned on.
/// The integration of the anesthesia machine to the respiratory system is handled at run time by constructing a combined circuit of 
/// the respiratory and anesthesia machine.  
/// Handles lung volume changes during alveolar gas transfer. 
/// Calculates physiological parameters such as respiration rate, tidal volume and others that belonging to the respiratory system.
//--------------------------------------------------------------------------------------------------
void Respiratory::Process()
{
  // Respiration circuit changes based on if Anesthesia Machine is on or off
  // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
  SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();
  // Calc the circuits
  m_Calculator->Process(RespirationCircuit, m_dt_s);
  //ModifyPleuralVolume();
  SEGasCompartmentGraph& RespirationGraph = m_data.GetCompartments().GetActiveRespiratoryGraph();
  SELiquidCompartmentGraph& AerosolGraph = m_data.GetCompartments().GetActiveAerosolGraph();
  // Transport substances
  m_GasTransporter->Transport(RespirationGraph, m_dt_s);
  if(m_AerosolMouth->HasSubstanceQuantities())
    m_AerosolTransporter->Transport(AerosolGraph, m_dt_s);
  //Update system data
  CalculateVitalSigns();

  //jbw - remove

  double leftAlveoliPressure = m_LeftAlveoli->GetNextPressure(PressureUnit::cmH2O);
  double leftAlveoliVolume = m_LeftAlveoli->GetNextVolume(VolumeUnit::L);
  double leftPleuralPressure = m_LeftPleural->GetNextPressure(PressureUnit::cmH2O);
  double leftPleuralVolume = m_LeftPleural->GetNextVolume(VolumeUnit::L);
  double leftFlow = m_LeftAlveolarDeadSpaceToLeftAlveoli->GetNextFlow(VolumePerTimeUnit::L_Per_s);
  double leftCompliance = m_LeftPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  double rightAlveoliPressure = m_RightAlveoli->GetNextPressure(PressureUnit::cmH2O);
  double rightAlveoliVolume = m_RightAlveoli->GetNextVolume(VolumeUnit::L);
  double rightPleuralPressure = m_RightPleural->GetNextPressure(PressureUnit::cmH2O);
  double rightPleuralVolume = m_RightPleural->GetNextVolume(VolumeUnit::L);
  double rightFlow = m_RightAlveolarDeadSpaceToRightAlveoli->GetNextFlow(VolumePerTimeUnit::L_Per_s);
  double rightCompliance = m_RightPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  m_data.GetDataTrack().Probe("leftAlveoliPressure", leftAlveoliPressure);
  m_data.GetDataTrack().Probe("leftAlveoliVolume",   leftAlveoliVolume);
  m_data.GetDataTrack().Probe("leftPleuralPressure", leftPleuralPressure);
  m_data.GetDataTrack().Probe("leftPleuralVolume",   leftPleuralVolume);
  m_data.GetDataTrack().Probe("leftFlow",            leftFlow);
  m_data.GetDataTrack().Probe("leftCompliance",      leftCompliance);

  m_data.GetDataTrack().Probe("rightAlveoliPressure", rightAlveoliPressure);
  m_data.GetDataTrack().Probe("rightAlveoliVolume",   rightAlveoliVolume);
  m_data.GetDataTrack().Probe("rightPleuralPressure", rightPleuralPressure);
  m_data.GetDataTrack().Probe("rightPleuralVolume",   rightPleuralVolume);
  m_data.GetDataTrack().Probe("rightFlow",            rightFlow);
  m_data.GetDataTrack().Probe("rightCompliance",      rightCompliance);
    
  double AmbientPresure = 1033.23; // = 1 atm
  m_data.GetDataTrack().Probe("LeftRelativePleuralPressure_cmH2O", leftPleuralPressure - AmbientPresure);
  m_data.GetDataTrack().Probe("RightRelativePleuralPressure_cmH2O", rightPleuralPressure - AmbientPresure);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system postprocess function
///
/// \details
/// Updates the current values of the gas volume fraction and gas volumes for the nodes in the respiratory circuit 
/// or the nodes in the combined (respiratory + anesthesia machine) circuit when the anesthesia machine is turned on.
//--------------------------------------------------------------------------------------------------
void Respiratory::PostProcess()
{
  // Respiration circuit changes based on if Anesthesia Machine is on or off
  // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
  SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();  
  m_Calculator->PostProcess(RespirationCircuit);  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Compute deposited mass, update localized PD effects 
///
/// \details
/// For each aerosol get the SIDE coefficient to determine deposited mass in each respiratory compartment. 
/// Adjust the resistances between compartments as a function of deposited mass to reach validated data.  
/// Liquid and solid aerosols are handeled here. 
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessAerosolSubstances()
{
  m_AverageLocalTissueBronchodilationEffects = 0.0; //No effect

  size_t numAerosols = m_AerosolMouth->GetSubstanceQuantities().size();
  if (numAerosols == 0)
    return;

  double inflammationCoefficient;

  // For this time step
  double mouthDepositied_ug = 0;
  double carinaDepositied_ug = 0;
  double leftDeadSpaceDepositied_ug = 0;
  double leftAlveoliDepositied_ug = 0;
  double rightDeadSpaceDepositied_ug = 0;
  double rightAlveoliDepositied_ug = 0;

  // Total amount deposited (including this time step)
  double mouthTotalDepositied_ug = 0;
  double carinaTotalDepositied_ug = 0;
  double leftDeadSpaceTotalDepositied_ug = 0;
  double leftAlveoliTotalDepositied_ug = 0;
  double rightDeadSpaceTotalDepositied_ug = 0;
  double rightAlveoliTotalDepositied_ug = 0;

  // Resistance Modifier Sum
  double mouthResistanceModifier=1;
  double carinaResistanceModifier=1;
  double leftDeadSpaceResistanceModifier=1;
  double leftAlveoliResistanceModifier=1;
  double rightDeadSpaceResistanceModifier=1;
  double rightAlveoliResistanceModifier=1;

  // Currently, There is no way to clear out depositied particulate out of the respiratory systems.
  // Maybe we could have it to cough or some other excretion related method... 
  
  SELiquidSubstanceQuantity* subQ;
  SELiquidSubstanceQuantity* tSubQ;
  const SizeIndependentDepositionEfficencyCoefficient* SIDECoeff;
  double combinedRightBronchodilationEffects = 0.0;
  double combinedLeftBronchodilationEffects = 0.0;
  for (size_t i=0; i<numAerosols; i++)
  {
    //initialize substance
    subQ = m_AerosolMouth->GetSubstanceQuantities()[i];
    //Adjust inflammation coefficient (scaled down):
    inflammationCoefficient = subQ->GetSubstance().GetAerosolization().GetInflammationCoefficient().GetValue();// Once for each subQ
    inflammationCoefficient *= 0.01;
    //Mouth
    SIDECoeff = &m_data.GetSubstances().GetSizeIndependentDepositionEfficencyCoefficient(subQ->GetSubstance());// Once for each subQ
    mouthDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolMouth->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetMouth();
    if (mouthDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      mouthDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-mouthDepositied_ug, MassUnit::ug); 
    subQ->Balance(BalanceLiquidBy::Mass);
    mouthTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(mouthDepositied_ug, MassUnit::ug);
    mouthResistanceModifier += mouthTotalDepositied_ug*inflammationCoefficient;
    //Carina
    subQ = m_AerosolCarina->GetSubstanceQuantities()[i];
    carinaDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolCarina->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetCarina();
    if (carinaDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      carinaDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-carinaDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    carinaTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(carinaDepositied_ug, MassUnit::ug);
    carinaResistanceModifier += carinaTotalDepositied_ug*inflammationCoefficient;
    //Left DeadSpace
    subQ = m_AerosolLeftAnatomicDeadSpace->GetSubstanceQuantities()[i];
    leftDeadSpaceDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolLeftAnatomicDeadSpace->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetDeadSpace();
    if (leftDeadSpaceDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      leftDeadSpaceDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-leftDeadSpaceDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    leftDeadSpaceTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(leftDeadSpaceDepositied_ug, MassUnit::ug);
    leftDeadSpaceResistanceModifier += leftDeadSpaceTotalDepositied_ug*inflammationCoefficient;
    //Left Alveoli
    subQ = m_AerosolLeftAlveoli->GetSubstanceQuantities()[i];
    leftAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolLeftAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetAlveoli();
    if (leftAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      leftAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-leftAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    leftAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(leftAlveoliDepositied_ug, MassUnit::ug);
    leftAlveoliResistanceModifier += leftAlveoliTotalDepositied_ug*inflammationCoefficient;
    //Right DeadSpace
    subQ = m_AerosolRightAnatomicDeadSpace->GetSubstanceQuantities()[i];
    rightDeadSpaceDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolRightAnatomicDeadSpace->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetDeadSpace();
    if (rightDeadSpaceDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      rightDeadSpaceDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-rightDeadSpaceDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    rightDeadSpaceTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(rightDeadSpaceDepositied_ug, MassUnit::ug);
    rightDeadSpaceResistanceModifier += rightDeadSpaceTotalDepositied_ug*inflammationCoefficient;
    //Right Alveoli
    subQ = m_AerosolRightAlveoli->GetSubstanceQuantities()[i];
    rightAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolRightAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_dt_s*SIDECoeff->GetAlveoli();
    if (rightAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      rightAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-rightAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    rightAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(rightAlveoliDepositied_ug, MassUnit::ug);
    rightAlveoliResistanceModifier += rightAlveoliTotalDepositied_ug*inflammationCoefficient;
    
    // Apply the BronchileModifier dilation effect
    // This is all just tuned to Albuterol - it'll work for other substances, and can be tuned using the other parameters (especially BronchioleModifier)
    if (subQ->GetSubstance().GetState() == eSubstance_State::Liquid)
    {
      // Sum the Bronchiole Effects
      // Must be positive
      double bronchioleModifier = subQ->GetSubstance().GetAerosolization().GetBronchioleModifier().GetValue();

      // Diffuse into Tissues
      // We only process mass deposited on the lungs (dead space and alveoli)
      // We do not currently do anything with the mass in the mouth and carina
      // Could possibly let it go into the stomach somehow... 
      tSubQ = m_LeftLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(leftDeadSpaceDepositied_ug + leftAlveoliDepositied_ug, MassUnit::ug); tSubQ->Balance(BalanceLiquidBy::Mass);    
      combinedLeftBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

      tSubQ = m_RightLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(rightDeadSpaceDepositied_ug + rightAlveoliDepositied_ug, MassUnit::ug); tSubQ->Balance(BalanceLiquidBy::Mass);
      combinedRightBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    }
  }

  //We're going to make the bronchodilation effects be based off of Albuterol.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect really strong with a normal dose.
  //This was tuned using a standard inhaler dose of 90ug
  m_AverageLocalTissueBronchodilationEffects = (combinedLeftBronchodilationEffects + combinedRightBronchodilationEffects) / 2.0;
  combinedLeftBronchodilationEffects = exp(-32894.0 * combinedLeftBronchodilationEffects);
  combinedRightBronchodilationEffects = exp(-32894.0 * combinedRightBronchodilationEffects);  

    // Change resistances due to deposition
  double dTracheaResistance = m_MouthToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*carinaResistanceModifier;
  double dLeftAlveoliResistance = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*leftAlveoliResistanceModifier;
  double dRightAlveoliResistance = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*rightAlveoliResistanceModifier;

  double dLeftBronchiResistance = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dRightBronchiResistance = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  dLeftBronchiResistance *= leftDeadSpaceResistanceModifier * combinedLeftBronchodilationEffects;
  dRightBronchiResistance *= rightDeadSpaceResistanceModifier * combinedRightBronchodilationEffects;
  dLeftBronchiResistance = LIMIT(dLeftBronchiResistance, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  dRightBronchiResistance = LIMIT(dRightBronchiResistance, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);

  m_MouthToCarina->GetNextResistance().SetValue(dTracheaResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(dLeftBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(dRightBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance().SetValue(dLeftAlveoliResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance().SetValue(dRightAlveoliResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies the pressure and/or flow at the mouth
///
/// \details
/// Handles the mechanical ventilation action that adds a flow and pressure source to instantaneously
/// set the respiratory connection (mouth) to user specified values.  
//--------------------------------------------------------------------------------------------------
void Respiratory::MechanicalVentilation()
{
  if (m_data.GetActions().GetPatientActions().HasMechanicalVentilation())
  {
    SEMechanicalVentilation* mv = m_data.GetActions().GetPatientActions().GetMechanicalVentilation();
    // You only get here if action is On
    m_data.SetAirwayMode(eAirwayMode::MechanicalVentilator);

    //Set the substance volume fractions ********************************************
    std::vector<SESubstanceFraction*> gasFractions = mv->GetGasFractions();

    //Reset the substance quantities at the connection
    for (SEGasSubstanceQuantity* subQ : m_MechanicalVentilatorConnection->GetSubstanceQuantities())
      subQ->SetToZero();

    //If no gas fractions specified, assume ambient
    if (gasFractions.empty())
    {
      for (auto s : m_Environment->GetSubstanceQuantities())
      {
        m_MechanicalVentilatorConnection->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
      }
    }
    else
    {
      //Has fractions defined
      for (auto f : gasFractions)
      {
        SESubstance& sub = f->GetSubstance();
        double fraction = f->GetFractionAmount().GetValue();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);

        //Now set it on the connection compartment
        //It has a NaN volume, so this will keep the same volume fraction no matter what's going on around it
        m_MechanicalVentilatorConnection->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
      }
    }

    //Set the aerosol concentrations ********************************************
    std::vector<SESubstanceConcentration*> liquidConcentrations = mv->GetAerosols();

    //Reset the substance quantities at the connection
    for (SELiquidSubstanceQuantity* subQ : m_MechanicalVentilatorAerosolConnection->GetSubstanceQuantities())
      subQ->SetToZero();

    if (!liquidConcentrations.empty())
    {
      //Has fractions defined
      for (auto f : liquidConcentrations)
      {
        SESubstance& sub = f->GetSubstance();
        SEScalarMassPerVolume concentration = f->GetConcentration();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);

        //Now set it on the connection compartment
        //It has a NaN volume, so this will keep the same volume fraction no matter what's going on around it
        m_MechanicalVentilatorAerosolConnection->GetSubstanceQuantity(sub)->GetConcentration().Set(concentration);
      }
    }

    //Apply the instantaneous flow ********************************************
    if (mv->HasFlow())
    {
      m_ConnectionToMouth->GetNextFlowSource().Set(mv->GetFlow());
      //It may or may not be there
      if (!m_ConnectionToMouth->HasFlowSource())
      {
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().StateChange();
      }
    }
    else
    {
      //If there's no flow specified, we need to remove the flow source    
      if (m_ConnectionToMouth->HasNextFlowSource())
      {
        m_ConnectionToMouth->GetNextFlowSource().Invalidate();
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().StateChange();
      }
    }

    //Apply the instantaneous pressure ********************************************  
    if (mv->HasPressure())
    {
      //This is the pressure above ambient
      m_GroundToConnection->GetNextPressureSource().Set(mv->GetPressure());
    }
    else
    {
      //Pressure is same as ambient
      m_GroundToConnection->GetNextPressureSource().SetValue(0.0, PressureUnit::cmH2O);
    }
  }
  else if (m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator)
  {
    // Was just turned off
    m_data.SetAirwayMode(eAirwayMode::Free);
    if (m_ConnectionToMouth->HasNextFlowSource())
    {
      m_ConnectionToMouth->GetNextFlowSource().Invalidate();
      m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().StateChange();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Applys supplemental oxygen equipment
///
/// \details
/// This will provide supplemental oxygen by connecting and updating the circuit and graph for nasal
/// cannula, simple mask, or nonrebreather mask
//--------------------------------------------------------------------------------------------------
void Respiratory::SupplementalOxygen()
{
  ///\todo - Maybe this and mechanical ventilator should be broken out to their own class, like anesthesia machine?

  if (!m_data.GetActions().GetPatientActions().HasSupplementalOxygen())
    return;
  SESupplementalOxygen* so = m_data.GetActions().GetPatientActions().GetSupplementalOxygen();

  // Get flow
  double flow_L_Per_min = so->GetFlow(VolumePerTimeUnit::L_Per_min);
  //Get tank pressure node and flow control resistor path
  SEFluidCircuitPath* Tank = nullptr;
  SEFluidCircuitPath* OxygenInlet = nullptr;
  SEFluidCircuit* RespirationCircuit = nullptr;
  switch (so->GetDevice())
  {
    case eSupplementalOxygen_Device::None:
    {
      m_data.SetAirwayMode(eAirwayMode::Free);
      m_data.GetActions().GetPatientActions().RemoveSupplementalOxygen();
      return;
    }
    case eSupplementalOxygen_Device::NasalCannula:
    {
      m_data.SetAirwayMode(eAirwayMode::NasalCannula);
      if (!so->HasFlow())
      {
        flow_L_Per_min = 3.5;
        so->GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + std::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::NasalCannulaPath::NasalCannulaOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::NasalCannulaPath::NasalCannulaPressure);
      break;
    }
    case eSupplementalOxygen_Device::NonRebreatherMask:
    {
      m_data.SetAirwayMode(eAirwayMode::NonRebreatherMask);
      if (!so->HasFlow())
      {
        flow_L_Per_min = 10.0;
        so->GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + std::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskPressure);
      break;
    }
    case eSupplementalOxygen_Device::SimpleMask:
    {
      m_data.SetAirwayMode(eAirwayMode::SimpleMask);
      if (!so->HasFlow())
      {
        flow_L_Per_min = 7.5;
        so->GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + std::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::SimpleMaskPath::SimpleMaskOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::SimpleMaskPath::SimpleMaskPressure);
      break;
    }
    default:
    {
      Error("Ignoring unsupported supplemental oxygen type : " + eSupplementalOxygen_Device_Name(so->GetDevice()));
      return;
    }
  }

  //Use a default tank volume if it wasn't explicity set
  if (!so->HasVolume())
  {
    so->GetVolume().SetValue(425.0, VolumeUnit::L);
    Info("Supplemental oxygen initial tank volume not set. Using default value of 425 L.");
  }
  

  //Decrement volume from the tank
  //Inf volume is assumed to be a wall connection that will never run out
  if (so->GetVolume(VolumeUnit::L) != std::numeric_limits<double>::infinity())
  {
    so->GetVolume().IncrementValue(-flow_L_Per_min * m_dt_min, VolumeUnit::L);
    //Check if the tank is depleated
    if (so->GetVolume(VolumeUnit::L) <= 0.0)
    {
      so->GetVolume().SetValue(0.0, VolumeUnit::L);
      flow_L_Per_min = 0.0;
      /// \event Supplemental Oxygen: Oxygen bottle is exhausted. There is no longer any oxygen to provide.
      m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, true, m_data.GetSimulationTime());
    }
    else
      m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, false, m_data.GetSimulationTime());
  }
  else
    m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, false, m_data.GetSimulationTime());

  //Nonrebreather mask works differently with the bag and doesn't have a pressure source for the tank
  if (so->GetDevice() != eSupplementalOxygen_Device::NonRebreatherMask)
  {
    //Determine flow control resistance
    //Assume pressure outside tank is comparatively approximately ambient
    double tankPressure_cmH2O = Tank->GetNextPressureSource(PressureUnit::cmH2O);
    double resistance_cmH2O_s_Per_L = tankPressure_cmH2O / (flow_L_Per_min / 60.0); //convert from min to s
    resistance_cmH2O_s_Per_L = LIMIT(resistance_cmH2O_s_Per_L, m_DefaultClosedResistance_cmH2O_s_Per_L, m_DefaultOpenResistance_cmH2O_s_Per_L);
    
    //Apply flow
    OxygenInlet->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  }
  else
  {
    //Handle nonrebreather mask bag
    SEFluidCircuitNode* NonRebreatherMaskBag = RespirationCircuit->GetNode(pulse::NonRebreatherMaskNode::NonRebreatherMaskBag);
    SEFluidCircuitPath* NonRebreatherMaskReservoirValve = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskReservoirValve);

    double flowOut_L_Per_min = 0.0;
    if (NonRebreatherMaskReservoirValve->HasNextFlow())
    {
      flowOut_L_Per_min = NonRebreatherMaskReservoirValve->GetNextFlow(VolumePerTimeUnit::L_Per_min);
    }

    double bagVolume_L = NonRebreatherMaskBag->GetNextVolume(VolumeUnit::L);
    bagVolume_L = bagVolume_L + (flow_L_Per_min - flowOut_L_Per_min) * m_dt_min;
    if (bagVolume_L < 0.0)
    {
      bagVolume_L = 0.0;

      //No air can come from the bag
      OxygenInlet->GetNextResistance().SetValue(m_RespOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      /// \event Supplemental Oxygen: The nonrebreather mask is empty. Oxygen may need to be provided at a faster rate.
      m_data.GetEvents().SetEvent(eEvent::NonRebreatherMaskOxygenBagEmpty, true, m_data.GetSimulationTime());
    }
    else
    {
      if (bagVolume_L > 1.0)
      {
        bagVolume_L = 1.0;
      }
      m_data.GetEvents().SetEvent(eEvent::NonRebreatherMaskOxygenBagEmpty, false, m_data.GetSimulationTime());
    }
    
    NonRebreatherMaskBag->GetNextVolume().SetValue(bagVolume_L, VolumeUnit::L);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory driver pressure source
///
/// \details
/// Calculates the muscle pressure source pressure by using the chemical stimuli as feedback control mechanism.
/// The method reads the arterial O2 and CO2 partial pressures. Using these partial pressures, the method calculates the
/// alveolar ventilation from which the method calculates the target breathing frequency. The target breathing frequency is
/// used in the equation for muscle pressure source. The muscle pressure source is used as a driver for ventilation.
/// This method also calculates the drug modifiers that adjusts the depth and frequency of respiration.
//--------------------------------------------------------------------------------------------------
void Respiratory::RespiratoryDriver()
{
  /// \event Patient: Start of exhale/inhale
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfExhale))
    m_data.GetEvents().SetEvent(eEvent::StartOfExhale, false, m_data.GetSimulationTime());
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfInhale))
    m_data.GetEvents().SetEvent(eEvent::StartOfInhale, false, m_data.GetSimulationTime());

  m_BreathingCycleTime_s += m_dt_s;

  //Keep a running average of the Arterial Partial Pressures  
  m_ArterialO2RunningAverage_mmHg->Sample(m_AortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCO2RunningAverage_mmHg->Sample(m_AortaCO2->GetPartialPressure(PressureUnit::mmHg));
  //Reset at start of cardiac cycle 
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
  {
    m_ArterialO2PartialPressure_mmHg = m_ArterialO2RunningAverage_mmHg->Value();
    m_ArterialCO2PartialPressure_mmHg = m_ArterialCO2RunningAverage_mmHg->Value();

    m_ArterialO2RunningAverage_mmHg->Clear();
    m_ArterialCO2RunningAverage_mmHg->Clear();
  }

#ifdef TUNING
  m_ArterialO2PartialPressure_mmHg = 95.0;
  m_ArterialCO2PartialPressure_mmHg = 40.0;
#endif  

  if (m_ConsciousBreathing) //Conscious breathing 
  {
    SEConsciousRespiration* cr = m_data.GetActions().GetPatientActions().GetConsciousRespiration();
    SEConsciousRespirationCommand* cmd = cr->GetActiveCommand();
    SEUseInhaler* ui = dynamic_cast<SEUseInhaler*>(cmd);
    if (ui != nullptr)
    {
      //We're using the inhaler, so just wait at this driver pressure
      m_DriverPressure_cmH2O = m_DriverPressurePath->GetPressureSource(PressureUnit::cmH2O);
      m_ConsciousBreathing = false;
    }
    else
    {
      //Just increase/decrease the driver pressure linearly to achieve the desired 
      //pressure (attempting to reach a specific volume) at the end of the user specified period
      double Time_s = m_ConsciousRespirationPeriod_s - m_ConsciousRespirationRemainingPeriod_s;
      double Slope = (m_ConsciousEndPressure_cmH2O - m_ConsciousStartPressure_cmH2O) / m_ConsciousRespirationPeriod_s;
      //Form of y=mx+b
      m_DriverPressure_cmH2O = Slope * Time_s + m_ConsciousStartPressure_cmH2O;

      //Make it so we start a fresh cycle when we go back to spontaneous breathing
      //Adding 2.0 * timestamp just makes it greater than the TotalBreathingCycleTime_s
      m_VentilationFrequency_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
      m_BreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min + 2.0 * m_dt_s;
    }
  }
  else //Spontaneous (i.e. not conscious) breathing
  {
    double TotalBreathingCycleTime_s = 0.0;
    if (m_VentilationFrequency_Per_min < 1.0)
    {
      TotalBreathingCycleTime_s = 0.0;
    }
    else
    {
      TotalBreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min; //Total time of one breathing cycle  
    }

    //Prepare for the next cycle -------------------------------------------------------------------------------
    if (m_BreathingCycleTime_s > TotalBreathingCycleTime_s) //End of the cycle or currently not breathing
    {
      UpdateIERatio();

      // Make a cardicArrestEffect that is 1.0 unless cardiac arrest is true
      double cardiacArrestEffect = 1.0;
      // If the cv system parameter is true, then make the cardicArrestEffect = 0
      if (m_data.GetEvents().IsEventActive(eEvent::CardiacArrest))
      {
        cardiacArrestEffect = 0.0;
      }

      //Ventilatory Negative Feedback Control *************************************************************************
      double PeripheralCO2PartialPressureSetPoint = m_data.GetConfiguration().GetPeripheralControllerCO2PressureSetPoint(PressureUnit::mmHg);
      double CentralCO2PartialPressureSetPoint = m_data.GetConfiguration().GetCentralControllerCO2PressureSetPoint(PressureUnit::mmHg);

      double metabolicModifier = 1.0;
      double TMR_W = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W);
      double BMR_W = m_Patient->GetBasalMetabolicRate(PowerUnit::W);
      double metabolicFraction = TMR_W / BMR_W;

      //Get Drug Effects
      SEDrugSystem& Drugs = m_data.GetDrugs();
      double DrugRRChange_Per_min = Drugs.GetRespirationRateChange(FrequencyUnit::Per_min);
      double DrugsTVChange_L = Drugs.GetTidalVolumeChange(VolumeUnit::L);
      double NMBModifier = 1.0;
      double SedationModifier = 1.0;
      //Make changes to Respiration Rate change based on the neuromuscular block level
      if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.135)
      {
        NMBModifier = 0.0;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }
      else if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.11)
      {
        NMBModifier = 0.5;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }
      else if (Drugs.GetSedationLevel().GetValue() > 0.15 && std::abs(m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min) + DrugRRChange_Per_min) < 1.0)
      {
        SedationModifier = 0.0;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }

      //Calculate the target Alveolar Ventilation based on the Arterial O2 and CO2 concentrations
      double dTargetAlveolarVentilation_L_Per_min = m_PeripheralControlGainConstant * exp(-0.05*m_ArterialO2PartialPressure_mmHg)*MAX(0., m_ArterialCO2PartialPressure_mmHg - PeripheralCO2PartialPressureSetPoint); //Peripheral portion
      dTargetAlveolarVentilation_L_Per_min += m_CentralControlGainConstant * MAX(0., m_ArterialCO2PartialPressure_mmHg - CentralCO2PartialPressureSetPoint); //Central portion

      //Metabolic modifier is used to drive the system to reasonable levels achievable during increased metabolic exertion
      //The modifier is tuned to achieve the correct respiratory response for near maximal exercise. A linear relationship is assumed
      // for the respiratory effects due to increased metabolic exertion    
      double tunedVolumeMetabolicSlope = 0.2; //Tuned fractional increase of the tidal volume due to increased metabolic rate
      metabolicModifier = 1.0 + tunedVolumeMetabolicSlope * (metabolicFraction - 1.0);
      dTargetAlveolarVentilation_L_Per_min *= metabolicModifier;

      //Only move it part way to where it wants to be.
      //If you stays there, it will get there, just more controlled/slowly.
      //This is needed so we don't overshoot and introduce low frequency oscillations into the system (i.e. overdamped).
      double targetCO2PP_mmHg = 40.0;
      double changeFraction = 1.0; //jbw - make this independet of timestep (i.e., use 0.02/m_dt_s)
      if (m_data.GetState() <= EngineState::InitialStabilization)
      {
        //This gets it nice and stable
        changeFraction = 0.1;
      }
      else
      {
        //This keeps it from going crazy with modifiers applied
        changeFraction = std::abs(m_ArterialCO2PartialPressure_mmHg - targetCO2PP_mmHg) / targetCO2PP_mmHg * 0.5;
      }
      changeFraction = MIN(changeFraction, 1.0);

#ifdef TUNING
      changeFraction = 1.0;
#endif

      dTargetAlveolarVentilation_L_Per_min = m_PreviousTargetAlveolarVentilation_L_Per_min + (dTargetAlveolarVentilation_L_Per_min - m_PreviousTargetAlveolarVentilation_L_Per_min) * changeFraction;
      m_PreviousTargetAlveolarVentilation_L_Per_min = dTargetAlveolarVentilation_L_Per_min;

      //Target Tidal Volume (i.e. Driver amplitude) *************************************************************************
      //Calculate the target Tidal Volume based on the Alveolar Ventilation
      double dTargetPulmonaryVentilation_L_Per_min = dTargetAlveolarVentilation_L_Per_min;

      double dMaximumPulmonaryVentilationRate = m_data.GetConfiguration().GetPulmonaryVentilationRateMaximum(VolumePerTimeUnit::L_Per_min);
      /// \event Patient: Maximum Pulmonary Ventilation Rate : Pulmonary ventilation exceeds maximum value
      if (dTargetPulmonaryVentilation_L_Per_min > dMaximumPulmonaryVentilationRate)
      {
        dTargetPulmonaryVentilation_L_Per_min = dMaximumPulmonaryVentilationRate;
        m_data.GetEvents().SetEvent(eEvent::MaximumPulmonaryVentilationRate, true, m_data.GetSimulationTime());
      }

      if (dTargetPulmonaryVentilation_L_Per_min < dMaximumPulmonaryVentilationRate && m_data.GetEvents().IsEventActive(eEvent::MaximumPulmonaryVentilationRate))
      {
        m_data.GetEvents().SetEvent(eEvent::MaximumPulmonaryVentilationRate, false, m_data.GetSimulationTime());
      }

      //Calculate the target Tidal Volume based on the calculated target pulmonary ventilation, plot slope (determined during initialization), and x-intercept
      double dTargetTidalVolume_L = dTargetPulmonaryVentilation_L_Per_min / m_VentilationToTidalVolumeSlope + m_VentilationTidalVolumeIntercept;

      //Modify the target tidal volume due to other external effects - probably eventually replaced by the Nervous system
      dTargetTidalVolume_L *= cardiacArrestEffect * NMBModifier;

      //Apply Drug Effects to the target tidal volume
      dTargetTidalVolume_L += DrugsTVChange_L;

      //This is a piecewise function that plateaus at the Tidal Volume equal to 1/2 * Vital Capacity
      //The Respiration Rate will make up for the Alveoli Ventilation difference
      double dHalfVitalCapacity_L = m_Patient->GetVitalCapacity(VolumeUnit::L) / 2;
      dTargetTidalVolume_L = MIN(dTargetTidalVolume_L, dHalfVitalCapacity_L);

      //Map the Target Tidal Volume to the Driver
      double TargetVolume_L = m_InitialFunctionalResidualCapacity_L + dTargetTidalVolume_L;
      m_PeakRespiratoryDrivePressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);
      //There's a maximum force the driver can try to achieve
      m_PeakRespiratoryDrivePressure_cmH2O = MAX(m_PeakRespiratoryDrivePressure_cmH2O, m_MaxDriverPressure_cmH2O);


      //Respiration Rate (i.e. Driver frequency) *************************************************************************
      //Calculate the Respiration Rate given the Alveolar Ventilation and the Target Tidal Volume
      if (dTargetTidalVolume_L == 0.0) //Can't divide by zero
      {
        m_VentilationFrequency_Per_min = 0.0;
        m_NotBreathing = true;
      }
      else
      {
        m_VentilationFrequency_Per_min = dTargetPulmonaryVentilation_L_Per_min / dTargetTidalVolume_L; //breaths/min
        m_VentilationFrequency_Per_min *= NMBModifier * SedationModifier;
        m_VentilationFrequency_Per_min += DrugRRChange_Per_min;
        m_NotBreathing = false;
      }

      if (m_VentilationFrequency_Per_min > dMaximumPulmonaryVentilationRate / dHalfVitalCapacity_L)
        m_VentilationFrequency_Per_min = dMaximumPulmonaryVentilationRate / dHalfVitalCapacity_L;

      if (m_VentilationFrequency_Per_min < 0.0)
        m_VentilationFrequency_Per_min = 0.0;

      //Patient Definition *************************************************************************
      //We need to hit the patient's defined Respiration Rate Baseline, no matter what,
      //so we'll keep adjusting the slope of the function to achieve this
      if (m_data.GetState() <= EngineState::InitialStabilization)
      {
        double dRespirationRateBaseline_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
        double dPercentError = (m_VentilationFrequency_Per_min - dRespirationRateBaseline_Per_min) / dRespirationRateBaseline_Per_min; //negative if too low

        //Amplitude set-point - this will set the Tidal Volume baseline when O2 and CO2 are at the correct/balanced level
        m_VentilationToTidalVolumeSlope = m_VentilationToTidalVolumeSlope - m_VentilationToTidalVolumeSlope * dPercentError;

        //Put bounds on this
        m_VentilationToTidalVolumeSlope = LIMIT(m_VentilationToTidalVolumeSlope, 1.0, 100.0);
      }

#ifdef TUNING
      m_VentilationToTidalVolumeSlope = 40.0;
      m_VentilationFrequency_Per_min = 16.0;
#endif

      SetBreathCycleFractions();

      m_BreathingCycleTime_s = 0.0;

      //KEEP COMMENTED OUT - this is for keeping the driver constant while debugging
      //m_VentilationFrequency_Per_min = 16.0;
      //m_PeakRespiratoryDrivePressure = -11.3;

      m_data.GetEvents().SetEvent(eEvent::StartOfInhale, true, m_data.GetSimulationTime()); ///\todo rename "StartOfConsiousInhale"
    }

    //Run the driver based on the waveform -------------------------------------------------------------------------------
    TotalBreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min; //Recalculate, in case we just calculated a new breath

    double PeakExpiratoryPressure = 0.0; //For potential future implementation

    double InspiratoryRiseTimeStart_s = 0.0;
    double InspiratoryHoldTimeStart_s = InspiratoryRiseTimeStart_s + m_InspiratoryRiseFraction * TotalBreathingCycleTime_s;
    double InspiratoryReleaseTimeStart_s = InspiratoryHoldTimeStart_s + m_InspiratoryHoldFraction * TotalBreathingCycleTime_s;
    double InspiratoryToExpiratoryPauseTimeStart_s = InspiratoryReleaseTimeStart_s + m_InspiratoryReleaseFraction * TotalBreathingCycleTime_s;
    double ExpiratoryRiseTimeStart_s = InspiratoryToExpiratoryPauseTimeStart_s + m_InspiratoryToExpiratoryPauseFraction * TotalBreathingCycleTime_s;
    double ExpiratoryHoldTimeStart_s = ExpiratoryRiseTimeStart_s + m_ExpiratoryRiseFraction * TotalBreathingCycleTime_s;
    double ExpiratoryReleaseTimeStart_s = ExpiratoryHoldTimeStart_s + m_ExpiratoryHoldFraction * TotalBreathingCycleTime_s;
    double ResidueFractionTimeStart_s = ExpiratoryReleaseTimeStart_s + m_ExpiratoryReleaseFraction * TotalBreathingCycleTime_s;

    m_DriverInspirationTime_s = ExpiratoryRiseTimeStart_s;


    if (m_BreathingCycleTime_s >= InspiratoryReleaseTimeStart_s &&
      m_BreathingCycleTime_s < InspiratoryReleaseTimeStart_s + m_dt_s) //Only call this once per cycle
    {
      m_data.GetEvents().SetEvent(eEvent::StartOfExhale, true, m_data.GetSimulationTime());
    }

    double tau = 0.0;
    double tauDenominator = 10.0; //lower = "rounder"... too low (seems to be < 5) starts creating incontinuities
    if (m_BreathingCycleTime_s >= ResidueFractionTimeStart_s)
    {
      m_DriverPressure_cmH2O = 0.0;
    }
    else if (m_BreathingCycleTime_s >= ExpiratoryReleaseTimeStart_s)
    {
      tau = m_ExpiratoryReleaseFraction * TotalBreathingCycleTime_s / tauDenominator;
      m_DriverPressure_cmH2O = exp(-(m_BreathingCycleTime_s - ExpiratoryReleaseTimeStart_s) / tau) * PeakExpiratoryPressure;
    }
    else if (m_BreathingCycleTime_s >= ExpiratoryHoldTimeStart_s)
    {
      m_DriverPressure_cmH2O = PeakExpiratoryPressure;
    }
    else if (m_BreathingCycleTime_s >= ExpiratoryRiseTimeStart_s)
    {
      tau = m_ExpiratoryRiseFraction * TotalBreathingCycleTime_s / tauDenominator;
      m_DriverPressure_cmH2O = 1.0 - (exp(-(m_BreathingCycleTime_s - ExpiratoryRiseTimeStart_s) / tau)) * PeakExpiratoryPressure;
    }
    else if (m_BreathingCycleTime_s >= InspiratoryToExpiratoryPauseTimeStart_s)
    {
      m_DriverPressure_cmH2O = 0.0;
    }
    else if (m_BreathingCycleTime_s >= InspiratoryReleaseTimeStart_s)
    {
      tau = m_InspiratoryReleaseFraction * TotalBreathingCycleTime_s / tauDenominator;
      m_DriverPressure_cmH2O = exp(-(m_BreathingCycleTime_s - InspiratoryReleaseTimeStart_s) / tau) * m_PeakRespiratoryDrivePressure_cmH2O;
    }
    else if (m_BreathingCycleTime_s >= InspiratoryHoldTimeStart_s)
    {
      tau = m_InspiratoryHoldFraction * TotalBreathingCycleTime_s / tauDenominator;
      m_DriverPressure_cmH2O = m_PeakRespiratoryDrivePressure_cmH2O;
    }
    else //(m_BreathingCycleTime_s >= InspiratoryRiseTimeStart_s)
    {
      tau = m_InspiratoryRiseFraction * TotalBreathingCycleTime_s / tauDenominator;
      m_DriverPressure_cmH2O = -(exp(-m_BreathingCycleTime_s / tau) - 1) * m_PeakRespiratoryDrivePressure_cmH2O;
    }

    Dyspnea();

    if (m_NotBreathing)
    {
      m_DriverPressure_cmH2O = 0.0;
    }
  }

  //Push Driving Data to the Circuit -------------------------------------------------------------------------------
  m_DriverPressurePath->GetNextPressureSource().SetValue(m_DriverPressure_cmH2O, PressureUnit::cmH2O);
}

//jbw - Add description
void Respiratory::SetBreathCycleFractions()
{
  ////Healthy = 0.34 remaining, giving ~1:2 IE Ratio
  m_InspiratoryRiseFraction = 0.33 * m_IERatioScaleFactor;
  m_InspiratoryHoldFraction = 0.0;
  m_InspiratoryReleaseFraction = MIN(0.33 / m_IERatioScaleFactor, 1.0 - m_InspiratoryRiseFraction);
  m_InspiratoryToExpiratoryPauseFraction = 0.0;
  m_ExpiratoryRiseFraction = 0.0;
  m_ExpiratoryHoldFraction = 0.0;
  m_ExpiratoryReleaseFraction = 0.0;  
 }

//--------------------------------------------------------------------------------------------------
/// \brief
/// Airway obstruction 
///
/// \details
/// Various factors can trigger upper airway obstruction. The engine handles airway obstruction that arise from 
/// the partial or complete obstruction of the upper airways by foreign objects. The model accounts for such obstruction by setting the 
/// flow resistance of the trachea compartment in proportion to the severity of the obstruction.
/// The function updates the tracheal flow resistance by modifying the resistance across the airway to carina node path.
//--------------------------------------------------------------------------------------------------
void Respiratory::AirwayObstruction()
{
  if (!m_PatientActions->HasAirwayObstruction())
    return;

  double Severity = m_PatientActions->GetAirwayObstruction()->GetSeverity().GetValue();
  double AirwayResistance = m_MouthToCarina->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dClosedResistance = AirwayResistance;
  AirwayResistance = GeneralMath::ResistanceFunction(20.0, m_RespOpenResistance_cmH2O_s_Per_L, dClosedResistance, Severity);
  m_MouthToCarina->GetNextResistance().SetValue(AirwayResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Bronchoconstriction
///
/// \details
/// Bronchoconstriction involves the tightening of smooth muscles surrounding bronchi. The effect of such airway constriction is the 
/// reduction of air flow or increase of flow resistance of the lower airways. This model handles bronchoconstriction by increasing
/// the flow resistances of the bronchi compartments. The function updates the bronchial resistances in proportion to the severity of the 
/// bronchoconstriction. 
//--------------------------------------------------------------------------------------------------
void Respiratory::BronchoConstriction()
{
  if (!m_PatientActions->HasBronchoconstriction())
    return;

  double LeftBronchiResistance = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double RightBronchiResistance = m_CarinaToRightAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dSeverity = m_PatientActions->GetBronchoconstriction()->GetSeverity().GetValue();
  double dClosedResistance = LeftBronchiResistance;
  LeftBronchiResistance = GeneralMath::ResistanceFunction(70.0, m_RespOpenResistance_cmH2O_s_Per_L, dClosedResistance, dSeverity);
  dClosedResistance = RightBronchiResistance;
  RightBronchiResistance = GeneralMath::ResistanceFunction(70.0, m_RespOpenResistance_cmH2O_s_Per_L, dClosedResistance, dSeverity);

  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(LeftBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(RightBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Bronchodilation PD effects.
///
/// \details
/// This reduces the bronchi resitances based on drug PD effects (i.e., plasma concentrations).
//--------------------------------------------------------------------------------------------------
void Respiratory::BronchoDilation()
{
  //Note: this could constrict with a negative number from PD
  
  //We're going to make the bronchodilation effects be based off of Albuterol.
  //A value of 1.0 will be the effective Albuterol EMax (with the current, non-overdose implementation), and -1.0 will be a bronchconstriction the same percentage in the other direction.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect of 1.0 be at the respiratory open resistance.
  //Dilation effect values have max effect at 1 and below -1, so anything outside of that will maintain that effect.
  double bronchoDilationEffect = m_data.GetDrugs().GetBronchodilationLevel().GetValue();
  if (bronchoDilationEffect != 0.0)
  {
    //Note: It'll pretty much always get in here because there's epinephrine present
    // Get the path resistances 
    double dLeftBronchiResistance = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    double dRightBronchiResistance = m_CarinaToRightAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    if (bronchoDilationEffect >= 0.0)// positive, therefore dilation
    {
      bronchoDilationEffect = MIN(bronchoDilationEffect, 1.0);
      dLeftBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, dLeftBronchiResistance, bronchoDilationEffect);
      dRightBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, dRightBronchiResistance, bronchoDilationEffect);
    }
    else //negative, therefore constriction
    {
      bronchoDilationEffect = MIN(-bronchoDilationEffect, 1.0);
      dLeftBronchiResistance = GeneralMath::ResistanceFunction(10.0, dLeftBronchiResistance, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
      dRightBronchiResistance = GeneralMath::ResistanceFunction(10.0, dRightBronchiResistance, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
    }
    m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(dLeftBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(dRightBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Intubation, including esophageal, left mainstem, and right mainstem
///
/// \details
/// During mechanical ventilation, one of the clinical complications of endotracheal intubation is esophageal intubation. This involves the 
/// misplacement of the tube down the esophagus. Such event prohibits air flow into or out of the lungs. The Biogers circuit handles 
/// this respiratory distress by manipulating the tracheal resistance. When esophageal intubation incidence is triggered, significantly large 
/// resistance is assigned to the trachea compartment. Otherwise, the esophageal compartment resistance is set to be significantly
/// large value under normal condition. 
//--------------------------------------------------------------------------------------------------
void Respiratory::Intubation()
{
  //Don't modify the stomach on environment changes
  if (m_Ambient->GetNextPressure(PressureUnit::cmH2O) != m_Ambient->GetPressure(PressureUnit::cmH2O))
  {
    //The environment just changed
    //Keep volume the same by changing the pressure equally on both sides
    double pressureChange_cmH2O = m_Ambient->GetNextPressure(PressureUnit::cmH2O) - m_Ambient->GetPressure(PressureUnit::cmH2O);
    m_Stomach->GetNextPressure().IncrementValue(pressureChange_cmH2O, PressureUnit::cmH2O);
  }

  if (m_PatientActions->HasIntubation())
  {
    m_data.SetIntubation(eSwitch::On);
    SEIntubation* intubation = m_PatientActions->GetIntubation();
    switch (intubation->GetType())
    {
    case eIntubation_Type::Tracheal:
      {// The proper way to intubate
        // Airway mode handles this case by default
        break;
      }
    case eIntubation_Type::Esophageal:
      {        
        // Allow air flow between Airway and Stomach
        ///\todo Make this a modifier (i.e. multiplier), instead of setting it directly
        m_MouthToStomach->GetNextResistance().SetValue(1.2, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
        // Stop air flow between the Airway and Carina
        //This is basically an open switch.  We don't need to worry about anyone else modifying it if this action is on.
        m_MouthToCarina->GetNextResistance().SetValue(m_DefaultOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
        break;
      }      
    case eIntubation_Type::RightMainstem:
      {
        m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(m_RespOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
        break;
      }
    case eIntubation_Type::LeftMainstem:
      {
        m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(m_RespOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
        break;
      }
    }
  }
  else
  {
    m_data.SetIntubation(eSwitch::Off);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Tension pneumothorax
///
/// \details
/// Pneumothorax is an abnormal accumulation of air in the pleural cavity. Tension pneumothorax occurs when 
/// air escapes into the pleural space on inspiration, but cannot return on expiration. Different types of pneumothorces are known.
/// The engine models  open (sucking chest wound) and closed (intact thoracic cage) pnumothoraces whereas occlusive dressing
/// and needle decompression are modeled as interventions. 
/// The function models pneumothorax and needle decompression by opening respiratory circuits that allow air flows into 
/// and out of the pleural cavity. Under normal condition, the path between the lung node or the outside environment and the intrapleural cavity 
/// is blocked via significantly high resistances that serve as open switches. When open or closed pneumothorax is triggered, these resistances 
/// are reduced in proportion to the severity of the pneumothorax. This reduction permits air flow into the pleural cavity a
/// and cause respiratory and cardiovascular distress.
/// The function allows the selection of tension pneumothorax and/or needle decompression events on the left or right lung side. The effects on 
/// the cardiovascular system parameters is accounted through modifiers that scale in proportion to the accumulated gas. 
//--------------------------------------------------------------------------------------------------
void Respiratory::Pneumothorax()
{
  if (m_PatientActions->HasTensionPneumothorax())
  {
    // Minimum flow resistance for the chest cavity or alveoli leak 
    double PneumoMinResistance_cmH2O_s_Per_L = 30.0;
    // Maximum flow resistance for the chest cavity or alveoli leak
    double PneumoMaxResistance_cmH2O_s_Per_L = m_DefaultOpenResistance_cmH2O_s_Per_L;
    // Flow resistance for the decompression needle, if used
    double NeedleResistance_cmH2O_s_Per_L = 100.0;

    if (m_PatientActions->HasLeftOpenTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftOpenTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasLeftChestOcclusiveDressing())
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_EnvironmentToLeftChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasLeftNeedleDecompression())
      {
        DoLeftNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightOpenTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightOpenTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasRightChestOcclusiveDressing())
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_EnvironmentToRightChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasRightNeedleDecompression())
      {
        DoRightNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
      m_EnvironmentToRightChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    }

    if (m_PatientActions->HasLeftClosedTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftClosedTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0)
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_LeftAlveoliLeakToLeftPleural->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasLeftNeedleDecompression())
      {
        DoLeftNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightClosedTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightClosedTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0)
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_RightAlveoliLeakToRightPleural->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasRightNeedleDecompression())
      {
        DoRightNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    //Check for interventions without insult
    if (!m_PatientActions->HasLeftClosedTensionPneumothorax() && !m_PatientActions->HasLeftOpenTensionPneumothorax())
    {
      if (m_PatientActions->HasLeftChestOcclusiveDressing() || m_PatientActions->HasLeftNeedleDecompression())
      {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveLeftChestOcclusiveDressing();
        m_PatientActions->RemoveLeftNeedleDecompression();
        return;
      }
    }
    if (!m_PatientActions->HasRightClosedTensionPneumothorax() && !m_PatientActions->HasRightOpenTensionPneumothorax())
    {
      if (m_PatientActions->HasRightChestOcclusiveDressing() || m_PatientActions->HasRightNeedleDecompression())
      {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveRightChestOcclusiveDressing();
        m_PatientActions->RemoveRightNeedleDecompression();
        return;
      }
    }
  }
  else
  {
    // Check for cases where the needle decompression or occlusive dressing actions are called without having
    // initiated a pneumothorax action
    if (m_PatientActions->HasNeedleDecompression())
    {
      /// \error Patient: can't process needle decompression if no pneumothorax is present
      Error("Cannot perform a Needle Decompression intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftNeedleDecompression();
      m_PatientActions->RemoveRightNeedleDecompression();
      return;
    }
    if (m_PatientActions->HasChestOcclusiveDressing())
    {
      /// \error Patient: can't process a chest occlusive dressing if no pneumothorax is present      
      Error("Cannot perform a Chest Occlusive Dressing intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftChestOcclusiveDressing();
      m_PatientActions->RemoveRightChestOcclusiveDressing();
      return;
    }
  }
}


//--------------------------------------------------------------------------------------------------
/// \brief
/// Conscious respiration
///
/// \param  None
///
/// \return void
///
/// \details
/// This method determines when to process a conscious respiration command and removes the command
/// from the stack.
//--------------------------------------------------------------------------------------------------
void Respiratory::ConsciousRespiration()
{
  if (m_ConsciousBreathing && m_ConsciousRespirationRemainingPeriod_s >= 0.0)
  {
    m_ConsciousRespirationRemainingPeriod_s -= m_dt_s;

    if (m_ConsciousRespirationRemainingPeriod_s <= 0.0)
    {// We are done with this command
      m_ConsciousRespirationRemainingPeriod_s = 0.0;
      m_ConsciousRespirationPeriod_s = 0.0;
      m_ConsciousStartPressure_cmH2O = 0.0;
      m_ConsciousEndPressure_cmH2O = 0.0;
      m_ExpiratoryReserveVolumeFraction = -1.0;
      m_InspiratoryCapacityFraction = -1.0;
      m_ConsciousBreathing = false;
      SEConsciousRespiration* cr = m_PatientActions->GetConsciousRespiration();
      m_ss << "Completed Conscious Respiration Command : " << *cr->GetActiveCommand();
      Info(m_ss);
      cr->RemoveActiveCommand();      
    }
  }

  //We'll wait for any current commands to complete first - only one at a time
  if (!m_ConsciousBreathing && m_PatientActions->HasConsciousRespiration())
  {
    SEConsciousRespiration* cr = m_PatientActions->GetConsciousRespiration();
    SEConsciousRespirationCommand* cmd = cr->GetActiveCommand();
    ProcessConsciousRespiration(*cmd);
  }
}


//--------------------------------------------------------------------------------------------------
/// \brief
/// Process conscious respiration
///
/// \param  cmd - Conscious respiration command
///
/// \details
/// Processes conscious respiration commands.
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessConsciousRespiration(SEConsciousRespirationCommand& cmd)
{
  if (m_ConsciousRespirationRemainingPeriod_s > 0)
  {
    Error("Already processing a Conscious Respiration Command, ignoring this command");
    return;
  }

  m_ConsciousBreathing = true;
  m_ConsciousStartPressure_cmH2O = m_DriverPressurePath->GetPressureSource(PressureUnit::cmH2O);

  SEBreathHold* bh = dynamic_cast<SEBreathHold*>(&cmd);
  if (bh != nullptr)
  {
    m_ConsciousRespirationRemainingPeriod_s = bh->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;
    m_ConsciousEndPressure_cmH2O = m_ConsciousStartPressure_cmH2O;

    return;
  }
  SEForcedExhale* fe = dynamic_cast<SEForcedExhale*>(&cmd);
  if (fe != nullptr)
  {
    m_ExpiratoryReserveVolumeFraction = fe->GetExpiratoryReserveVolumeFraction().GetValue();
    m_ConsciousRespirationRemainingPeriod_s = fe->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;

    //Pressure effects
    double TargetVolume_L = m_InitialResidualVolume_L + m_InitialExpiratoryReserveVolume_L * (1.0 - m_ExpiratoryReserveVolumeFraction);
    m_ConsciousEndPressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);

    return;
  }
  SEForcedInhale* fi = dynamic_cast<SEForcedInhale*>(&cmd);
  if (fi != nullptr)
  {
    m_InspiratoryCapacityFraction = fi->GetInspiratoryCapacityFraction().GetValue();
    m_ConsciousRespirationRemainingPeriod_s = fi->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;

    //Pressure effects
    double TargetVolume_L = m_InitialFunctionalResidualCapacity_L + m_InitialInspiratoryCapacity_L * m_InspiratoryCapacityFraction;
    m_ConsciousEndPressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);

    return;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Left Side Needle Decompression
///
/// \param  dPressureTimePerVolume - Resistance value for air flow through the needle
///
/// \details
/// Used for left side needle decompression. this is an intervention (action) used to treat left 
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoLeftNeedleDecompression(double dPressureTimePerVolume)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dPressureTimePerVolumeLeftNeedle = dScalingFactor * dPressureTimePerVolume;
  m_LeftPleuralToEnvironment->GetNextResistance().SetValue(dPressureTimePerVolumeLeftNeedle, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Right Side Needle Decompression
///
/// \param  dPressureTimePerVolume - Resistance value for air flow through the needle
///
/// \details
/// Used for right side needle decompression. this is an intervention (action) used to treat right
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoRightNeedleDecompression(double dPressureTimePerVolume)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dPressureTimePerVolumeRightNeedle = dScalingFactor * dPressureTimePerVolume;
  m_RightPleuralToEnvironment->GetNextResistance().SetValue(dPressureTimePerVolumeRightNeedle, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

  double AnatomicDeadSpace_L = m_LeftAnatomicDeadSpace->GetNextVolume(VolumeUnit::L) + m_RightAnatomicDeadSpace->GetNextVolume(VolumeUnit::L);
  double LeftAlveolarDeadSpace_L = 0.0;
  if (m_LeftAlveolarDeadSpace->HasNextVolume())
    LeftAlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetNextVolume(VolumeUnit::L);
  double RightAlveolarDeadSpace_L = 0.0;
  if (m_RightAlveolarDeadSpace->HasNextVolume())
    RightAlveolarDeadSpace_L = m_RightAlveolarDeadSpace->GetNextVolume(VolumeUnit::L);
  double AlveolarDeadSpace_L = LeftAlveolarDeadSpace_L + RightAlveolarDeadSpace_L;
  GetAnatomicDeadSpace().SetValue(AnatomicDeadSpace_L, VolumeUnit::L);  
  GetAlveolarDeadSpace().SetValue(AlveolarDeadSpace_L, VolumeUnit::L);
  GetPhysiologicDeadSpace().SetValue(AnatomicDeadSpace_L + AlveolarDeadSpace_L, VolumeUnit::L);

  double tracheaFlow_L_Per_s = m_MouthToCarina->GetNextFlow().GetValue(VolumePerTimeUnit::L_Per_s);
  double previousInspiratoryFlow_L_Per_s = GetInspiratoryFlow(VolumePerTimeUnit::L_Per_s);
  GetInspiratoryFlow().SetValue(tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
  GetExpiratoryFlow().SetValue(-tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);  

  GetRespiratoryMusclePressure().Set(m_RespiratoryMuscle->GetNextPressure());

  /// \cite kacmarek2016egan page 227-228 ---------------------------------------------------
  double airwayOpeningPressure_cmH2O = m_Mouth->GetPressure(PressureUnit::cmH2O);
  double alveolarPressure_cmH2O = (m_LeftAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  double pleuralPressure_cmH2O = (m_LeftPleural->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightPleural->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  double bodySurfacePressure_cmH2O = m_Ambient->GetPressure(PressureUnit::cmH2O);

  double transrespiratoryPressure_cmH2O = airwayOpeningPressure_cmH2O - bodySurfacePressure_cmH2O;
  double transairwayPressure_cmH2O = airwayOpeningPressure_cmH2O - alveolarPressure_cmH2O;
  double transpulmonaryPressure_cmH2O = airwayOpeningPressure_cmH2O - pleuralPressure_cmH2O;
  double transalveolarPressure_cmH2O = alveolarPressure_cmH2O - pleuralPressure_cmH2O;
  double transthoracicPressure_cmH2O = alveolarPressure_cmH2O - bodySurfacePressure_cmH2O;
  double transChestWallPressure_cmH2O = pleuralPressure_cmH2O - bodySurfacePressure_cmH2O;

  GetIntrapleuralPressure().SetValue(transChestWallPressure_cmH2O, PressureUnit::cmH2O);
  GetTransrespiratoryPressure().SetValue(transrespiratoryPressure_cmH2O, PressureUnit::cmH2O);
  GetTransairwayPressure().SetValue(transairwayPressure_cmH2O, PressureUnit::cmH2O);
  GetTranspulmonaryPressure().SetValue(transpulmonaryPressure_cmH2O, PressureUnit::cmH2O);
  GetTransalveolarPressure().SetValue(transalveolarPressure_cmH2O, PressureUnit::cmH2O);
  GetTransthoracicPressure().SetValue(transthoracicPressure_cmH2O, PressureUnit::cmH2O);
  GetTransChestWallPressure().SetValue(transChestWallPressure_cmH2O, PressureUnit::cmH2O);

  if(SEScalar::IsZero(tracheaFlow_L_Per_s, ZERO_APPROX))
    GetPulmonaryResistance().SetValue(std::numeric_limits<double>::infinity(), PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  else
    GetPulmonaryResistance().SetValue((airwayOpeningPressure_cmH2O - alveolarPressure_cmH2O) / tracheaFlow_L_Per_s, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  double averageAlveoliO2PartialPressure_mmHg = (m_LeftAlveoliO2->GetPartialPressure(PressureUnit::mmHg) + m_RightAlveoliO2->GetPartialPressure(PressureUnit::mmHg)) / 2.0;
  GetAlveolarArterialGradient().SetValue(averageAlveoliO2PartialPressure_mmHg - m_AortaO2->GetPartialPressure(PressureUnit::mmHg), PressureUnit::mmHg);

  GetRespiratoryMusclePressure().Set(m_RespiratoryMuscle->GetNextPressure());

  double avgAlveoliO2PP_mmHg = (m_LeftAlveoliO2->GetPartialPressure(PressureUnit::mmHg) + m_RightAlveoliO2->GetPartialPressure(PressureUnit::mmHg)) / 2.0;
  GetAlveolarArterialGradient().SetValue(avgAlveoliO2PP_mmHg - m_AortaO2->GetPartialPressure(PressureUnit::mmHg), PressureUnit::mmHg);

  //It's a pain to figure out how to hold onto this data, so let's just set it at a sensitive transition point
  if (GetInspiratoryFlow(VolumePerTimeUnit::L_Per_s) > 0.0 //We're inhaling
    && previousInspiratoryFlow_L_Per_s <= 0.0) //We were exhaling
  {
    //Transition from exhale to inhale
    // Set the End Tidal Concentration for our gases at the end of a respiration cycle
    for (SEGasSubstanceQuantity* subQ : m_Carina->GetSubstanceQuantities())
    {
      subQ->GetSubstance().GetEndTidalFraction().Set(subQ->GetVolumeFraction());
      subQ->GetSubstance().GetEndTidalPressure().Set(subQ->GetPartialPressure());
    }
    GetEndTidalCarbonDioxideFraction().Set(m_data.GetSubstances().GetCO2().GetEndTidalFraction());
    GetEndTidalCarbonDioxidePressure().Set(m_data.GetSubstances().GetCO2().GetEndTidalPressure());
    GetEndTidalOxygenFraction().Set(m_data.GetSubstances().GetO2().GetEndTidalFraction());
    GetEndTidalOxygenPressure().Set(m_data.GetSubstances().GetO2().GetEndTidalPressure());
  }

  //Record values at the breathing inflection points (i.e. switch between inhale and exhale)  
  // Temporal tolerance to avoid accidental entry in the the inhalation and exhalation code blocks 
  m_ElapsedBreathingCycleTime_min += m_dt_min;

  if (m_BreathingCycle) //Exhaling
  {    
    if (totalLungVolume_L <= m_BottomBreathTotalVolume_L)
    {
      m_BottomBreathTotalVolume_L = totalLungVolume_L;
      m_BottomBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min - m_TopBreathElapsedTime_min;
      m_BottomBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
      m_BottomBreathPleuralVolume_L = m_RightPleural->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftPleural->GetNextVolume().GetValue(VolumeUnit::L);
      m_BottomBreathPleuralPressure_cmH2O = pleuralPressure_cmH2O;
      m_BottomBreathAlveoliPressure_cmH2O = alveolarPressure_cmH2O;
      m_BottomBreathDriverPressure_cmH2O = m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O);
    }

    if (totalLungVolume_L - m_BottomBreathTotalVolume_L > m_MinimumAllowableTidalVolume_L //Volume has transitioned sufficiently
      && m_BottomBreathElapsedTime_min > m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s / 60.0) //We've waited a sufficient amount of time to transition
    {
      //Transition to inhale

      // Calculate Respiration Rate and track time and update cycle flag
      double expirationTime_min = m_BottomBreathElapsedTime_min;
      double inspirationTime_min = m_TopBreathElapsedTime_min;
      double RespirationRate_Per_min = 1.0 / (expirationTime_min + inspirationTime_min);
      GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
      GetInspiratoryExpiratoryRatio().SetValue(inspirationTime_min / expirationTime_min);

      m_ElapsedBreathingCycleTime_min -= (expirationTime_min + inspirationTime_min);
      m_BreathingCycle = false;

      // Calculate the Tidal Volume from the last peak
      m_InstantaneousFunctionalResidualCapacity_L = m_BottomBreathTotalVolume_L;

      double TidalVolume_L = std::abs(m_TopBreathTotalVolume_L - m_BottomBreathTotalVolume_L);
      double AlveoliDeltaVolume_L = std::abs(m_TopBreathAlveoliVolume_L - m_BottomBreathAlveoliVolume_L);
      double PleuralDeltaVolume_L = std::abs(m_TopBreathPleuralVolume_L - m_BottomBreathPleuralVolume_L);
      double PleuralDeltaPressure_cmH2O = std::abs((m_BottomBreathPleuralPressure_cmH2O - m_BottomBreathDriverPressure_cmH2O) - (m_TopBreathPleuralPressure_cmH2O - m_TopBreathDriverPressure_cmH2O));
      double AlveoliDeltaPressure_cmH2O = std::abs((m_BottomBreathAlveoliPressure_cmH2O - m_BottomBreathPleuralPressure_cmH2O) - (m_TopBreathAlveoliPressure_cmH2O - m_TopBreathPleuralPressure_cmH2O));
      double LungDeltaPressure_cmH2O = std::abs((m_BottomBreathAlveoliPressure_cmH2O - m_BottomBreathDriverPressure_cmH2O) - (m_TopBreathAlveoliPressure_cmH2O - m_TopBreathDriverPressure_cmH2O));

      GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);

      double pulmonaryCompiance_L_Per_cmH2O = TidalVolume_L / LungDeltaPressure_cmH2O;
      double lungCompliance_L_Per_cmH2O = AlveoliDeltaVolume_L / AlveoliDeltaPressure_cmH2O;
      double chestWallCompliance_L_Per_cmH2O = PleuralDeltaVolume_L / PleuralDeltaPressure_cmH2O;
      GetPulmonaryCompliance().SetValue(pulmonaryCompiance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetLungCompliance().SetValue(lungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetChestWallCompliance().SetValue(chestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetPulmonaryElastance().SetValue(1.0 / pulmonaryCompiance_L_Per_cmH2O, PressurePerVolumeUnit::cmH2O_Per_L);

      GetPeakInspiratoryPressure().SetValue(m_PeakAlveolarPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);
      GetPositiveEndExpiratoryPressure().SetValue(m_BottomBreathAlveoliPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);
      GetMaximalInspiratoryPressure().SetValue(m_MaximalAlveolarPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);

      // Calculate Ventilations
      GetTotalPulmonaryVentilation().SetValue(TidalVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
      GetSpecificVentilation().SetValue(TidalVolume_L / m_InstantaneousFunctionalResidualCapacity_L);
      GetTotalAlveolarVentilation().SetValue(TidalVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
      GetTotalDeadSpaceVentilation().SetValue((AnatomicDeadSpace_L + AlveolarDeadSpace_L) * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);

      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min;
      m_PeakAlveolarPressure_cmH2O = m_BottomBreathAlveoliPressure_cmH2O;
      m_MaximalAlveolarPressure_cmH2O = m_BottomBreathAlveoliPressure_cmH2O;
    }
  }
  else //Inhaling
  {
    if (totalLungVolume_L >= m_TopBreathTotalVolume_L)
    {
      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min;
      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
      m_TopBreathPleuralVolume_L = m_RightPleural->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftPleural->GetNextVolume().GetValue(VolumeUnit::L);
      m_TopBreathPleuralPressure_cmH2O = pleuralPressure_cmH2O;
      m_TopBreathAlveoliPressure_cmH2O = alveolarPressure_cmH2O;
      m_TopBreathDriverPressure_cmH2O = m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O);
      m_TopCarinaO2 = m_CarinaO2->GetVolumeFraction().GetValue();
    }

    m_PeakAlveolarPressure_cmH2O = MAX(m_PeakAlveolarPressure_cmH2O, alveolarPressure_cmH2O);
    m_MaximalAlveolarPressure_cmH2O = MIN(m_MaximalAlveolarPressure_cmH2O, alveolarPressure_cmH2O);

    if (m_TopBreathTotalVolume_L - totalLungVolume_L > m_MinimumAllowableTidalVolume_L //Volume has transitioned sufficiently
      && m_TopBreathElapsedTime_min > m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s / 60.0) //We've waited a sufficient amount of time to transition
    {
      //Transition to exhaling
      m_BreathingCycle = true;      

      //We want the peak Carina O2 value - this should be the inspired value
      GetCarricoIndex().SetValue(m_ArterialO2PartialPressure_mmHg / m_TopCarinaO2, PressureUnit::mmHg);

      if (m_data.GetState() > EngineState::InitialStabilization)
      {// Don't throw events if we are initializing
      //Check for ARDS - 3 different levels
        if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 100.0)
        {
          /// \event Patient: Severe ARDS: Carrico Index is below 100 mmHg
          m_data.GetEvents().SetEvent(eEvent::SevereAcuteRespiratoryDistress, true, m_data.GetSimulationTime());  /// \cite ranieriacute
          m_data.GetEvents().SetEvent(eEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
          m_data.GetEvents().SetEvent(eEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        }
        else if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 200.0)
        {
          /// \event Patient: Moderate ARDS: Carrico Index is below 200 mmHg
          m_data.GetEvents().SetEvent(eEvent::ModerateAcuteRespiratoryDistress, true, m_data.GetSimulationTime());  /// \cite ranieriacute
          m_data.GetEvents().SetEvent(eEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
          m_data.GetEvents().SetEvent(eEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        }
        else if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 300.0)
        {
          /// \event Patient: Mild ARDS: Carrico Index is below 300 mmHg
          m_data.GetEvents().SetEvent(eEvent::MildAcuteRespiratoryDistress, true, m_data.GetSimulationTime());  /// \cite ranieriacute
          m_data.GetEvents().SetEvent(eEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
          m_data.GetEvents().SetEvent(eEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        }
        else
        {
          /// \event Patient: End ARDS: Carrico Index is above 305 mmHg
          m_data.GetEvents().SetEvent(eEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
          m_data.GetEvents().SetEvent(eEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
          m_data.GetEvents().SetEvent(eEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        }
      }

      m_BottomBreathTotalVolume_L = totalLungVolume_L;
      m_BottomBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min - m_TopBreathElapsedTime_min;
    }
  }

  //Zero out if waiting longer than 15 sec
  if (m_ElapsedBreathingCycleTime_min > 0.25)
  {
    GetRespirationRate().SetValue(0.0, FrequencyUnit::Per_min);
    GetTidalVolume().SetValue(0.0, VolumeUnit::L);
    GetTotalAlveolarVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
    GetTotalPulmonaryVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
  }

 /// \todo Move to blood chemistry
  // Although it is called respiratory acidosis/alkalosis, the detection and event setting is actually a part of the @ref BloodChemistrySystem
  // The terms "metabolic" and "respiratory" refer to the origin of the acid-base disturbance
  // The hydrogen ion concentration is a property of the blood
  // The events related to blood concentrations should be detected and set in blood chemistry.
  //Keep a running average of the pH
  m_BloodPHRunningAverage->Sample(m_data.GetBloodChemistry().GetBloodPH().GetValue());
  //Reset at start of cardiac cycle 
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
  {
    m_LastCardiacCycleBloodPH = m_BloodPHRunningAverage->Value();
    m_BloodPHRunningAverage->Clear();
  }

  if (m_data.GetState() > EngineState::InitialStabilization)
  {// Don't throw events if we are initializing

    //Bradypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) < 10)
    {
      /// \event Patient: Bradypnea: Respiration rate is below 10 breaths per minute
      /// The patient has bradypnea.
      m_data.GetEvents().SetEvent(eEvent::Bradypnea, true, m_data.GetSimulationTime());  /// \cite overdyk2007continuous
    }
    else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) >= 10.5)  // offset by .5 
    {
      /// \event Patient: End Bradypnea Event. The respiration rate has risen above 10. 
      /// The patient is no longer considered to have bradypnea.
      m_data.GetEvents().SetEvent(eEvent::Bradypnea, false, m_data.GetSimulationTime());
    }

    //Tachypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) > 20)
    {
      /// \event Patient: Tachypnea: Respiration rate is above 20 breaths per minute.
      /// The patient has tachypnea.
      m_data.GetEvents().SetEvent(eEvent::Tachypnea, true, m_data.GetSimulationTime());  /// \cite 
    }
    else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) <= 19.5) // offset by .5 
    {
      /// \event Patient: End Tachypnea Event. The respiration rate has fallen below 19.5. 
      /// The patient is no longer considered to have tachypnea.
      m_data.GetEvents().SetEvent(eEvent::Tachypnea, false, m_data.GetSimulationTime());
    }

    double highPh = 8.5;
    double lowPh = 6.5;   // \cite Edge2006AcidosisConscious
    //// Respiratory Acidosis
    if (m_LastCardiacCycleBloodPH < 7.35 && m_ArterialCO2PartialPressure_mmHg > 47.0)
    {
      /// \event Patient: Respiratory Acidosis: event is triggered when blood pH is below 7.36
      /// The patient has respiratory acidosis.
      m_data.GetEvents().SetEvent(eEvent::RespiratoryAcidosis, true, m_data.GetSimulationTime());

      /// \event Patient: arterial blood ph has dropped below 6.5.
      if (m_LastCardiacCycleBloodPH < lowPh)
      {
        ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is below 6.5, Patient is experiencing extreme respiratory Acidosis and is in an irreversible state.";
        Warning(ss);
        /// \irreversible Extreme respiratory Acidosis: blood pH below 6.5.
        m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
      }
    }
    else if (m_LastCardiacCycleBloodPH >= 7.38 && m_ArterialCO2PartialPressure_mmHg < 44.0)
    {
      /// \event Patient: End Respiratory Acidosis Event. The pH value has risen above 7.38. 
      /// The patient is no longer considered to have respiratory acidosis.
      m_data.GetEvents().SetEvent(eEvent::RespiratoryAcidosis, false, m_data.GetSimulationTime());
    }

    ////Respiratory Alkalosis
    if (m_LastCardiacCycleBloodPH > 7.45 && m_ArterialCO2PartialPressure_mmHg < 37.0)
    {
      /// \event Patient: Respiratory Alkalosis: event is triggered when blood pH is above 7.45
      /// The patient has respiratory alkalosis.
      m_data.GetEvents().SetEvent(eEvent::RespiratoryAlkalosis, true, m_data.GetSimulationTime());

      /// \event Patient: arterial blood ph has gotten above 8.5.
      if (m_LastCardiacCycleBloodPH > highPh)
      {
        ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is above 8.5, Patient is experiencing extreme respiratory Alkalosis and is in an irreversible state.";
        Warning(ss);
        /// \irreversible Extreme respiratory Alkalosis: blood pH above 8.5.
        m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
      }
    }
    else if (m_LastCardiacCycleBloodPH <= 7.43 && m_ArterialCO2PartialPressure_mmHg > 39.0)
    {
      /// \event Patient: End Respiratory Alkalosis Event. The pH value has has fallen below 7.45. 
      /// The patient is no longer considered to have respiratory alkalosis.
      m_data.GetEvents().SetEvent(eEvent::RespiratoryAlkalosis, false, m_data.GetSimulationTime());
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update obstructive (airway) resistance 
///
/// \return void
///
/// \details
/// This method takes a resistance scaling factor and lung percentages (left and right) as input
/// variables.  It updates the Carina to Dead Space path resistances in order to model airflow 
/// blockage through the bronchi and bronchioles.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateObstructiveResistance()
{
  if ((!m_PatientActions->HasAsthmaAttack() && !m_data.GetConditions().HasChronicObstructivePulmonaryDisease())
    || GetExpiratoryFlow(VolumePerTimeUnit::L_Per_s) < 0.0) // Only on exhalation
  {
    return;
  }
  
  double combinedResistanceScalingFactor = 1.0;  
  //Asthma attack on
  if (m_PatientActions->HasAsthmaAttack())
  {
    double dSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
    // Resistance function: Base = 10, Min = 1.0, Max = 90.0 (increasing with severity)
    double dResistanceScalingFactor = GeneralMath::ResistanceFunction(10, 90, 1, dSeverity);
    combinedResistanceScalingFactor = dResistanceScalingFactor;
  }
  //COPD on
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease())
  {
    double dSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
    // Resistance function: Base = 10, Min = 1.0, Max = 100.0 (increasing with severity)
    double dResistanceScalingFactor = GeneralMath::ResistanceFunction(10, 90, 1, dSeverity);
    combinedResistanceScalingFactor = MAX(combinedResistanceScalingFactor, dResistanceScalingFactor);
  }
  
  // Get the path resistances 
  double dLeftBronchiResistance = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dRightBronchiResistance = m_CarinaToRightAnatomicDeadSpace->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  dLeftBronchiResistance *= combinedResistanceScalingFactor;
  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(dLeftBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  dRightBronchiResistance *= combinedResistanceScalingFactor;
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(dRightBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update the inspiratory-expiratory ratio
///
/// \return void
///
/// \details
/// The inspiratory-expiratory ratio is modified based on certain actions and conditions, as well as
/// local bronchodilation effects from aerosols.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateIERatio()
{
  // Asthma, Chronic Bronchitis, and Lobar Pneumonia
  //  Adjust the inspiration/expiration ratio based on severity
  double combinedSeverity = 0.0;
  m_IEscaleFactor = 1.0;
  if (m_PatientActions->HasAsthmaAttack())
  {
    combinedSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
  }
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease())
  {
    double dBronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
    double dEmphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    combinedSeverity = MAX(combinedSeverity, dEmphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, dBronchitisSeverity);
  }
  if (m_data.GetConditions().HasLobarPneumonia())
  {
    double severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
    // Get lung fractions
    double dRightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    double dLeftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double dLP_ScaledSeverity = (0.75*severity) + (severity*dLeftLungFraction*dLeftLungRatio) + (severity*dRightLungFraction*dRightLungRatio);
    combinedSeverity = MAX(combinedSeverity, dLP_ScaledSeverity);
  }

  if (combinedSeverity > 0.0)
  {
    //When albuterol is administered, the bronchodilation also causes the IE ratio to correct itself
    m_IEscaleFactor = 1.0 - combinedSeverity;
    m_IEscaleFactor *= exp(7728.4 * m_AverageLocalTissueBronchodilationEffects);

    // IE scale factor is constrained to a minimum of 0.1 and a maximum 1.0. Lower than 0.1 causes simulation instability.
    // Greater than 1.0 is not possible for patients with these conditions
    m_IEscaleFactor = LIMIT(m_IEscaleFactor, 0.1, 1.0);
  }
}


//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Alveoli Compliance 
///
/// \param  dComplianceScalingFactor  Alveoli compliance multiplier
/// \param  dLeftLungFraction      Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction      Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a compliance scaling factor and lung percentages (left and right) as input
/// variables.  It updates the Alveoli to Pleural compliances in order to model changes in alveoli
/// compliance resulting from alveolus membrane damage or alveolus fluid content
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateLungCompliance(double dCompilanceScalingFactor, double dLeftLungFraction, double dRightLungFraction)
{
  // Get path compliances
  double dRightAlveoliBaselineCompliance = m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline().GetValue(VolumePerPressureUnit::L_Per_cmH2O);
  double dLeftAlveoliBaselineCompliance = m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline().GetValue(VolumePerPressureUnit::L_Per_cmH2O);

  // Left lung alveoli
  dLeftAlveoliBaselineCompliance = (dLeftAlveoliBaselineCompliance*(1.0 - dLeftLungFraction)) + (dLeftAlveoliBaselineCompliance*dCompilanceScalingFactor*dLeftLungFraction);
  m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline().SetValue(dLeftAlveoliBaselineCompliance, VolumePerPressureUnit::L_Per_cmH2O);

  // Right lung alveoli
  dRightAlveoliBaselineCompliance = (dRightAlveoliBaselineCompliance*(1.0 - dRightLungFraction)) + (dRightAlveoliBaselineCompliance*dCompilanceScalingFactor*dRightLungFraction);
  m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline().SetValue(dRightAlveoliBaselineCompliance, VolumePerPressureUnit::L_Per_cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Pulmonary Capillary Resistance 
///
/// \param  dResistanceScalingFactor  Pulmonary capillary resistance multiplier
/// \param  dLeftLungFraction      Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction      Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a resistance scaling factor and lung fractions (left and right) as input
/// variables.  It updates the pulmonary capillary to pulmonary vein resistance in order to model the 
/// destruction of capillaries in the alveolus membrane.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdatePulmonaryCapillaryResistance(double dResistanceScalingFactor, double dLeftLungPercent, double dRightLungPercent)
{
  // Get path resistances
  double dRightPulmonaryCapillaryResistance = m_RightPulmonaryCapillary->GetResistanceBaseline().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dLeftPulmonaryCapillaryResistance = m_LeftPulmonaryCapillary->GetResistanceBaseline().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  dLeftPulmonaryCapillaryResistance = (dLeftPulmonaryCapillaryResistance*(1.0 - dLeftLungPercent)) + (dLeftPulmonaryCapillaryResistance*dResistanceScalingFactor*dLeftLungPercent);
  m_LeftPulmonaryCapillary->GetResistanceBaseline().SetValue(dLeftPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  dRightPulmonaryCapillaryResistance = (dRightPulmonaryCapillaryResistance*(1.0 - dRightLungPercent)) + (dRightPulmonaryCapillaryResistance*dResistanceScalingFactor*dRightLungPercent);
  m_RightPulmonaryCapillary->GetResistanceBaseline().SetValue(dRightPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Gas Diffusion Surface Area
///
/// \param  dFractionArea    Fractional change in gas diffusion surface area (0 to 1)
/// \param  dLeftLungFraction  Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction  Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a percent valve and lung percentages (left and right) as input
/// variables.  It updates the gas diffusion surface area in the lungs in order to model the 
/// destruction of alveoli membranes and/or lung consolidation.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateGasDiffusionSurfaceArea(double dFractionArea, double dLeftLungFraction, double dRightLungFraction)
{
  double AlveoliDiffusionArea_cm2 = m_Patient->GetAlveoliSurfaceArea(AreaUnit::cm2);

  // Get the right and left lung ratios
  double RightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1.0 - RightLungRatio;

  // Calculate the surface area contributions for each lung
  double dRightContribution = RightLungRatio*((AlveoliDiffusionArea_cm2*(1.0 - dRightLungFraction)) + (AlveoliDiffusionArea_cm2*dFractionArea*dRightLungFraction));
  double dLeftContribution = LeftLungRatio*((AlveoliDiffusionArea_cm2*(1.0 - dLeftLungFraction)) + (AlveoliDiffusionArea_cm2*dFractionArea*dLeftLungFraction));

  // Calculate the total surface area
  AlveoliDiffusionArea_cm2 = dLeftContribution + dRightContribution;

  m_Patient->GetAlveoliSurfaceArea().SetValue(AlveoliDiffusionArea_cm2, AreaUnit::cm2);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Peak Driver Pressure Based on Target Volume 
///
/// \param  TargetVolume_L        Target volume (L)
///
/// \return Peak Driver Pressure    The driver pressure required to achieve target volume (cm H2O)
///
/// \details
/// This method returns the driver pressure as a function of target volume. The equation
/// used is constructed from respiratory unit test data (see RespiratoryDriverTest). 
//--------------------------------------------------------------------------------------------------
double Respiratory::VolumeToDriverPressure(double TargetVolume_L)
{
  //Calculate as if constant compliances
  double leftHealthyLungCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  double rightHealthyLungCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);

  double leftHealthyChestWallCompliance_L_Per_cmH2O = m_LeftPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  double rightHealthyChestWallCompliance_L_Per_cmH2O = m_RightPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  
  double totalCompliance_L_Per_cmH2O = 1.0 / (1.0 / leftHealthyChestWallCompliance_L_Per_cmH2O + 1.0 / leftHealthyLungCompliance_L_Per_cmH2O) +
    1.0 / (1.0 / rightHealthyChestWallCompliance_L_Per_cmH2O + 1.0 / rightHealthyLungCompliance_L_Per_cmH2O);

  double driverPressure_cmH2O = -(TargetVolume_L - m_InitialFunctionalResidualCapacity_L) / totalCompliance_L_Per_cmH2O;
  return driverPressure_cmH2O;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Populate the Pulmonary Function Test Assessment
///
/// \param  pft        pft data
///
/// \return success        true, if everything worked out
//--------------------------------------------------------------------------------------------------
bool Respiratory::CalculatePulmonaryFunctionTest(SEPulmonaryFunctionTest& pft) const
{
  pft.Clear();
  pft.GetExpiratoryReserveVolume().Set(m_Patient->GetExpiratoryReserveVolume());
  pft.GetFunctionalResidualCapacity().Set(m_Patient->GetFunctionalResidualCapacity());
  pft.GetInspiratoryCapacity().Set(m_Patient->GetInspiratoryCapacity());
  pft.GetInspiratoryReserveVolume().Set(m_Patient->GetInspiratoryReserveVolume());
  pft.GetResidualVolume().Set(m_Patient->GetResidualVolume());
  pft.GetTotalLungCapacity().Set(m_Patient->GetTotalLungCapacity());
  pft.GetVitalCapacity().Set(m_Patient->GetVitalCapacity());

  double rr_Hz = GetRespirationRate(FrequencyUnit::Hz);
  double tv_L = GetTidalVolume(VolumeUnit::L);
  double waveRespirationRate = rr_Hz;
  double pi = 3.14159265359;
  double magnitude = 0.5*tv_L;
  double superPosition = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L) + magnitude;
  double waveTime = 0.0;
  double currentTime = 0.0;

  double numPlotPoint = pft.GetNumberOfPlotPoints();
  SEFunctionVolumeVsTime& plot = pft.GetLungVolumePlot();
  plot.Invalidate();
  std::vector<double>& time = plot.GetTime();
  std::vector<double>& volume = plot.GetVolume();
  plot.SetTimeUnit(TimeUnit::s);
  plot.SetVolumeUnit(VolumeUnit::L);
  if (numPlotPoint > 0)
  {
    double dt = 60.0 / numPlotPoint;
    for (unsigned int i = 0; i < numPlotPoint; i++)
    {
      time.push_back(currentTime);
      if (waveTime >= 12.0 && waveTime < 13.0)
      {//Period dilation due to forced inspiration
        magnitude = 0.5*tv_L + pft.GetInspiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      }
      else if (waveTime >= 13.0 && waveTime < 14.0)
      {//Period dilation due to forced expiration
        magnitude = 0.5*tv_L + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      }
      else
      {
        magnitude = 0.5*tv_L;
        waveRespirationRate = rr_Hz;
      }

      volume.push_back(magnitude*sin(pi*waveTime) + superPosition);
      waveTime += 2.0*waveRespirationRate*dt;
      currentTime += dt;
    }
  }

  /// \todo Insert more PFT calculations here
  /*
  All Known:
  VitalCapacity=TotaLungCapacity-ResidualVolume
  InspiratoryCapacity=TotaLungCapacity-FunctionalResidualCapacity
  InspiratoryReserveVolume=InspiratoryCapacity-TidalVolume
  ExpiratoryReserveVolume=FunctionalResidualCapacity-ResidualVolume

  Need to define:
  ForcedVitalCapacity        |  Increase the pressure driver in the lungs
  ForcedExpiratoryVolume      |  for forced PFT constituents?
  ForcedExpiratoryFlow      |
  PeakExpiratoryFlow        |
  MaximumVoluntaryVentilation    |  The same pressure driver increase should be used here. Calculate as a time integration of lung volume over 1 min.

  SlowVitalCapacity        |

  */
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Run the circuit standalone once to get it into a good starting state 
/// - keep volume fractions constant
//--------------------------------------------------------------------------------------------------
void Respiratory::TuneCircuit()
{
  SEFluidCircuit& RespiratoryCircuit = m_data.GetCircuits().GetRespiratoryCircuit();
  //Set the starting/default driver pressure
  m_DriverPressurePath->GetNextPressureSource().Set(m_DriverPressurePath->GetPressureSourceBaseline());
  m_Calculator->Process(RespiratoryCircuit, m_dt_s);
  m_Calculator->PostProcess(RespiratoryCircuit);

  //Make sure the new volumes are accounted for with all the substance stuff
  //Keep the same volume fraction originally initialized
  SEGasCompartmentGraph* RespiratoryGraph = &m_data.GetCompartments().GetRespiratoryGraph();
  for (SEGasCompartment* compartment : RespiratoryGraph->GetCompartments())
  {
    if (compartment->HasVolume())
      compartment->Balance(BalanceGasBy::VolumeFraction);
  }
}

//jbw - add description
void Respiratory::UpdateChestWallCompliances()
{
  //Settings --------------------------------------------
  double rightLowerInflection =              0.1;       //Fraction between reserve volume and total lung capacity
  double rightUpperInflection =              0.7;       //Fraction between reserve volume and total lung capacity
  double rightLowerSlope_L_Per_cmH20 =       0.03;
  double rightUpperSlope_L_Per_cmH20 =       0.03;
  double rightMiddleCompliance_L_Per_cmH20 = 1.0 / (1.0 / m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O) + 
   1.0 / m_RightPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O));

  double leftLowerInflection =               0.1;       //Fraction between reserve volume and total lung capacity
  double leftUpperInflection =               0.7;       //Fraction between reserve volume and total lung capacity 
  double leftLowerSlope_L_Per_cmH20 =        0.03;
  double leftUpperSlope_L_Per_cmH20 =        0.03;
  double leftMiddleCompliance_L_Per_cmH20 = 1.0 / (1.0 / m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O) +
    1.0 / m_LeftPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O));

  double maxSlope_L_Per_cmH2O = 0.0005;
  //-----------------------------------------------------

  double rightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  double leftLungRatio = 1.0 - rightLungRatio;

  double rightVolume_L = m_RightLung->GetVolume(VolumeUnit::L);
  double leftVolume_L = m_LeftLung->GetVolume(VolumeUnit::L);

  double rightResidualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L) * rightLungRatio;
  double rightTotalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L) * rightLungRatio;
  double rightFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L) * rightLungRatio;
  double leftResidualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L) * leftLungRatio;
  double leftTotalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L) * leftLungRatio;  
  double leftFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L) * leftLungRatio;

  double rightLowerInflection_L = rightResidualVolume_L + (rightTotalLungCapacity_L - rightResidualVolume_L) * rightLowerInflection;
  double rightUpperInflection_L = rightResidualVolume_L + (rightTotalLungCapacity_L - rightResidualVolume_L) * rightUpperInflection;
  double leftLowerInflection_L = leftResidualVolume_L + (leftTotalLungCapacity_L - leftResidualVolume_L) * leftLowerInflection;
  double leftUpperInflection_L = leftResidualVolume_L + (leftTotalLungCapacity_L - leftResidualVolume_L) * leftUpperInflection;

  double rightLowerInflection_cmH2O = (rightLowerInflection_L - rightFunctionalResidualCapacity_L) / rightMiddleCompliance_L_Per_cmH20;
  double rightUpperInflection_cmH2O = (rightUpperInflection_L - rightFunctionalResidualCapacity_L) / rightMiddleCompliance_L_Per_cmH20;
  double leftLowerInflection_cmH2O = (leftLowerInflection_L - leftFunctionalResidualCapacity_L) / leftMiddleCompliance_L_Per_cmH20;
  double leftUpperInflection_cmH2O = (leftUpperInflection_L - leftFunctionalResidualCapacity_L) / leftMiddleCompliance_L_Per_cmH20;

  double rightResidualVolume_cmH2O = rightLowerInflection_cmH2O - (rightLowerInflection_L - rightResidualVolume_L) / rightLowerSlope_L_Per_cmH20;
  double rightTotalLungCapacity_cmH2O = rightUpperInflection_cmH2O + (rightUpperInflection_L - rightTotalLungCapacity_L) / rightUpperSlope_L_Per_cmH20;
  double leftResidualVolume_cmH2O = leftLowerInflection_cmH2O - (leftLowerInflection_L - leftResidualVolume_L) / leftLowerSlope_L_Per_cmH20;
  double leftTotalLungCapacity_cmH2O = leftUpperInflection_cmH2O + (leftUpperInflection_L - leftTotalLungCapacity_L) / leftUpperSlope_L_Per_cmH20;

  double rightSideCompliance_L_Per_cmH2O = 0.0;
  if (rightVolume_L < rightResidualVolume_L)
  {
    double expected_Pressure_cmH2O = rightResidualVolume_cmH2O - (rightResidualVolume_L - rightVolume_L) / maxSlope_L_Per_cmH2O;
    rightSideCompliance_L_Per_cmH2O = (rightVolume_L - rightFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else if (rightVolume_L < rightLowerInflection_L)
  {
    double expected_Pressure_cmH2O = rightLowerInflection_cmH2O - (rightLowerInflection_L - rightVolume_L) / rightLowerSlope_L_Per_cmH20;
    rightSideCompliance_L_Per_cmH2O = (rightVolume_L - rightFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else if (rightVolume_L < rightUpperInflection_L)
  {
    rightSideCompliance_L_Per_cmH2O = rightMiddleCompliance_L_Per_cmH20;
  }
  else if (rightVolume_L < rightTotalLungCapacity_L)
  {
    double expected_Pressure_cmH2O = rightUpperInflection_cmH2O + (rightVolume_L - rightUpperInflection_L) / rightUpperSlope_L_Per_cmH20;
    rightSideCompliance_L_Per_cmH2O = (rightVolume_L - rightFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else
  {
    double expected_Pressure_cmH2O = rightTotalLungCapacity_cmH2O + (rightVolume_L - rightTotalLungCapacity_L) / maxSlope_L_Per_cmH2O;
    rightSideCompliance_L_Per_cmH2O = (rightVolume_L - rightFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }

  double leftSideCompliance_L_Per_cmH2O = 0.0;
  if (leftVolume_L < leftResidualVolume_L)
  {
    double expected_Pressure_cmH2O = leftResidualVolume_cmH2O - (leftResidualVolume_L - leftVolume_L) / maxSlope_L_Per_cmH2O;
    leftSideCompliance_L_Per_cmH2O = (leftVolume_L - leftFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else if (leftVolume_L < leftLowerInflection_L)
  {
    double expected_Pressure_cmH2O = leftLowerInflection_cmH2O - (leftLowerInflection_L - leftVolume_L) / leftLowerSlope_L_Per_cmH20;
    leftSideCompliance_L_Per_cmH2O = (leftVolume_L - leftFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else if (leftVolume_L < leftUpperInflection_L)
  {
    leftSideCompliance_L_Per_cmH2O = leftMiddleCompliance_L_Per_cmH20;
  }
  else if (leftVolume_L < leftTotalLungCapacity_L)
  {
    double expected_Pressure_cmH2O = leftUpperInflection_cmH2O + (leftVolume_L - leftTotalLungCapacity_L) / leftUpperSlope_L_Per_cmH20;
    leftSideCompliance_L_Per_cmH2O = (leftVolume_L - leftFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }
  else
  {
    double expected_Pressure_cmH2O = leftTotalLungCapacity_cmH2O + (leftVolume_L - leftUpperInflection_L) / maxSlope_L_Per_cmH2O;
    leftSideCompliance_L_Per_cmH2O = (leftVolume_L - leftFunctionalResidualCapacity_L) / expected_Pressure_cmH2O;
  }

  //Assume lung/alveoli compliance is a constant value
  double leftHealthyLungCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  double rightHealthyLungCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);

  double leftChestWallCompliance_L_Per_cmH2O = 0.0;
  double rightChestWallCompliance_L_Per_cmH2O = 0.0;
  //Can't divide by zero
  if (leftSideCompliance_L_Per_cmH2O != leftHealthyLungCompliance_L_Per_cmH2O)
  {
    leftChestWallCompliance_L_Per_cmH2O = 1.0 / (1.0 / leftSideCompliance_L_Per_cmH2O - 1.0 / leftHealthyLungCompliance_L_Per_cmH2O);
  }
  if (rightSideCompliance_L_Per_cmH2O != rightHealthyLungCompliance_L_Per_cmH2O)
  {
    rightChestWallCompliance_L_Per_cmH2O = 1.0 / (1.0 / rightSideCompliance_L_Per_cmH2O - 1.0 / rightHealthyLungCompliance_L_Per_cmH2O);
  }

  //Dampen the change to prevent craziness
  double maxChange_L_Per_cmH2O_s = 0.01;

  double rightComplianceChange_L_Per_cmH2O = rightChestWallCompliance_L_Per_cmH2O - m_RightPleuralToRespiratoryMuscle->GetCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double leftComplianceChange_L_Per_cmH2O = leftChestWallCompliance_L_Per_cmH2O - m_LeftPleuralToRespiratoryMuscle->GetCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  if (rightComplianceChange_L_Per_cmH2O > 0.0)
  {
    rightComplianceChange_L_Per_cmH2O = MIN(rightComplianceChange_L_Per_cmH2O, maxChange_L_Per_cmH2O_s * m_dt_s);
  }
  else if (rightComplianceChange_L_Per_cmH2O < 0.0)
  {
    rightComplianceChange_L_Per_cmH2O = MAX(rightComplianceChange_L_Per_cmH2O, -maxChange_L_Per_cmH2O_s * m_dt_s);
  }

  if (leftComplianceChange_L_Per_cmH2O > 0.0)
  {
    leftComplianceChange_L_Per_cmH2O = MIN(leftComplianceChange_L_Per_cmH2O, maxChange_L_Per_cmH2O_s * m_dt_s);
  }
  else if (leftComplianceChange_L_Per_cmH2O < 0.0)
  {
    leftComplianceChange_L_Per_cmH2O = MAX(leftComplianceChange_L_Per_cmH2O, -maxChange_L_Per_cmH2O_s * m_dt_s);
  }

  rightChestWallCompliance_L_Per_cmH2O = m_RightPleuralToRespiratoryMuscle->GetCompliance(VolumePerPressureUnit::L_Per_cmH2O) + rightComplianceChange_L_Per_cmH2O;
  leftChestWallCompliance_L_Per_cmH2O = m_LeftPleuralToRespiratoryMuscle->GetCompliance(VolumePerPressureUnit::L_Per_cmH2O) + leftComplianceChange_L_Per_cmH2O;

  m_RightPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(rightChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_LeftPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(leftChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
}

  ///\ToDo Add work calculation
}

//jbw - Add description
void Respiratory::CalculateFatigue()
{
  ///\ToDo Add fatigue calculation
}


//jbw????
//if (m_data.GetEvents().IsEventActive(eEvent::StartOfInhale))
//  {
//    //Only do this at the end of a breath, otherwise it won't be at the correct spot on the curve and will shift everything
//  }

//jbw - Add description
void Respiratory::UpdateVolumes()
{
  ///\cite guyton2006medical p504: In a person with partially functional or nonfunctional alveoli in some parts of the lungs, 
  //the physiological dead space may be as much as 10 times the volume of the anatomical dead space, or 1 to 2 liters.

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double leftAlveolarDeadSpace_L = 0.0;
    double rightAlveolarDeadSpace_L = 0.0;
    if (m_LeftAlveolarDeadSpace->HasVolumeBaseline())
      leftAlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L);
    if (m_RightAlveolarDeadSpace->HasVolumeBaseline())
      rightAlveolarDeadSpace_L = m_RightAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L);

    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    double combinedSeverity = 0.0;
    combinedSeverity = MAX(combinedSeverity, emphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, bronchitisSeverity);

    //Linear function: Min = 0.0, Max = 2.0 (increasing with severity)
    double alveolarDeadSpace_L = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 2.0, combinedSeverity);

    double rightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;
    
    m_LeftAlveolarDeadSpace->GetNextVolume().SetValue(leftAlveolarDeadSpace_L + alveolarDeadSpace_L * leftLungRatio, VolumeUnit::L);
    m_RightAlveolarDeadSpace->GetNextVolume().SetValue(rightAlveolarDeadSpace_L + alveolarDeadSpace_L * rightLungRatio, VolumeUnit::L);
  }
}

//jbw - Add description
void Respiratory::UpdateResistances()
{
  double tracheaResistance_cmH2O_s_Per_L = m_MouthToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightBronchiResistance_cmH2O_s_Per_L = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftBronchiResistance_cmH2O_s_Per_L = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightAlveoliResistance_cmH2O_s_Per_L = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftAlveoliResistance_cmH2O_s_Per_L = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double esophagusResistance_cmH2O_s_Per_L = m_MouthToStomach->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  
  //------------------------------------------------------------------------------------------------------
  //Artificial Airway
  if (m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator ||
    m_data.GetAirwayMode() == eAirwayMode::AnesthesiaMachine)
  {
    tracheaResistance_cmH2O_s_Per_L *= 8.0;
  }

  //Airway obstruction
  if (m_PatientActions->HasAirwayObstruction())
  {
    double Severity = m_PatientActions->GetAirwayObstruction()->GetSeverity().GetValue();
    tracheaResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(20.0, tracheaResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, Severity);
    tracheaResistance_cmH2O_s_Per_L = MIN(tracheaResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  }

  //Broncho constriction
  if (m_PatientActions->HasBronchoconstriction())
  {
    double dSeverity = m_PatientActions->GetBronchoconstriction()->GetSeverity().GetValue();
    leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(70.0, leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, dSeverity);
    rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(70.0, rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, dSeverity);
    leftBronchiResistance_cmH2O_s_Per_L = MIN(leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
    rightBronchiResistance_cmH2O_s_Per_L = MIN(rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  }

  //------------------------------------------------------------------------------------------------------
  //Bronchodilation
  //Note: this could constrict with a negative number from PD
  //We're going to make the bronchodilation effects be based off of Albuterol.
  //A value of 1.0 will be the effective Albuterol EMax (with the current, non-overdose implementation), and -1.0 will be a bronchconstriction the same percentage in the other direction.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect of 1.0 be at the respiratory open resistance.
  //Dilation effect values have max effect at 1 and below -1, so anything outside of that will maintain that effect.
  double bronchoDilationEffect = m_data.GetDrugs().GetBronchodilationLevel().GetValue();
  if (bronchoDilationEffect != 0.0)
  {
    //It'll pretty much always get in here because there's epinephrine present
    //Get the path resistances 
    if (bronchoDilationEffect >= 0.0)// positive, therefore dilation
    {
      bronchoDilationEffect = MIN(bronchoDilationEffect, 1.0);
      leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialDecayFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, leftBronchiResistance_cmH2O_s_Per_L, bronchoDilationEffect);
      rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialDecayFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, rightBronchiResistance_cmH2O_s_Per_L, bronchoDilationEffect);
    }
    else //negative, therefore constriction
    {
      bronchoDilationEffect = MIN(-bronchoDilationEffect, 1.0);
      leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(10.0, leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
      rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(10.0, rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
    }
    leftBronchiResistance_cmH2O_s_Per_L = BLIM(leftBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
    rightBronchiResistance_cmH2O_s_Per_L = BLIM(rightBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  }

  //------------------------------------------------------------------------------------------------------
  //Obstructive
  double obstructiveResistanceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Asthma
  if (m_PatientActions->HasAsthmaAttack())
  {
    double dSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
    obstructiveResistanceScalingFactor = GeneralMath::ExponentialGrowthFunction(10.0, 1.0, 90.0, dSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    double dResistanceScalingFactor = GeneralMath::ExponentialGrowthFunction(10.0, 1.0, 90.0, bronchitisSeverity);
    obstructiveResistanceScalingFactor = MAX(obstructiveResistanceScalingFactor, dResistanceScalingFactor);
  }

  leftBronchiResistance_cmH2O_s_Per_L *= obstructiveResistanceScalingFactor;
  rightBronchiResistance_cmH2O_s_Per_L *= obstructiveResistanceScalingFactor;
  leftBronchiResistance_cmH2O_s_Per_L = BLIM(leftBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  rightBronchiResistance_cmH2O_s_Per_L = BLIM(rightBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);

  //------------------------------------------------------------------------------------------------------
  //Restrictive - No change
  //ARDS
  //LobarPneumonia

  //------------------------------------------------------------------------------------------------------
  //Set new values
  m_MouthToCarina->GetNextResistance().SetValue(tracheaResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(rightBronchiResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(leftBronchiResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance().SetValue(rightAlveoliResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance().SetValue(leftAlveoliResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_MouthToStomach->GetNextResistance().SetValue(esophagusResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//jbw - Add description
void Respiratory::UpdateCompliances()
{
  //Artificial airway

  //------------------------------------------------------------------------------------------------------
  //Artificial Airway
  if (m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator ||
    m_data.GetAirwayMode() == eAirwayMode::AnesthesiaMachine)
  {
    rightAlveoliCompliance_L_Per_cmH2O *= 0.4;
    leftAlveoliCompliance_L_Per_cmH2O *= 0.4;
  }

  //------------------------------------------------------------------------------------------------------
  //Obstructive = Increase
  double leftObstructiveComplianceScalingFactor = 1.0;
  double rightObstructiveComplianceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Asthma
  //ToDo

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    double complianceScalingFactor = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 5.0, emphysemaSeverity);
    
    leftObstructiveComplianceScalingFactor = MAX(leftObstructiveComplianceScalingFactor, complianceScalingFactor);
    rightObstructiveComplianceScalingFactor = MAX(rightObstructiveComplianceScalingFactor, complianceScalingFactor);
  }

  //------------------------------------------------------------------------------------------------------
  //Restrictive = Decrease
  double leftRestrictiveComplianceScalingFactor = 1.0;
  double rightRestrictiveComplianceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Lobar Pneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    }
    
    double complianceScalingFactor = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 0.4, severity);

    leftRestrictiveComplianceScalingFactor = MIN(leftRestrictiveComplianceScalingFactor, complianceScalingFactor * leftLungFraction);
    rightRestrictiveComplianceScalingFactor = MIN(rightRestrictiveComplianceScalingFactor, complianceScalingFactor * rightLungFraction);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetRightLungAffected().GetValue();
    }

    double complianceScalingFactor = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 0.4, severity);

    leftRestrictiveComplianceScalingFactor = MIN(leftRestrictiveComplianceScalingFactor, complianceScalingFactor * leftLungFraction);
    rightRestrictiveComplianceScalingFactor = MIN(rightRestrictiveComplianceScalingFactor, complianceScalingFactor * rightLungFraction);
  }

  //------------------------------------------------------------------------------------------------------
  //Set new values
  leftAlveoliCompliance_L_Per_cmH2O = leftAlveoliCompliance_L_Per_cmH2O * leftObstructiveComplianceScalingFactor * leftRestrictiveComplianceScalingFactor;
  rightAlveoliCompliance_L_Per_cmH2O = rightAlveoliCompliance_L_Per_cmH2O * rightObstructiveComplianceScalingFactor * rightRestrictiveComplianceScalingFactor;
  
  m_RightAlveoliToRightPleuralConnection->GetNextCompliance().SetValue(rightAlveoliCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance().SetValue(leftAlveoliCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
}

//jbw - Add description
void Respiratory::UpdateInspiratoryExpiratoryRatio()
{
  //Adjust the inspiration/expiration ratio based on severity
  double combinedSeverity = 0.0;
  m_IERatioScaleFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Asthma
  if (m_PatientActions->HasAsthmaAttack())
  {
    combinedSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    combinedSeverity = MAX(combinedSeverity, emphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, bronchitisSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double scaledSeverity = severity * leftLungFraction * dLeftLungRatio + severity * rightLungFraction * dRightLungRatio;
    combinedSeverity = MAX(combinedSeverity, scaledSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double scaledSeverity = severity * leftLungFraction * dLeftLungRatio + severity * rightLungFraction * dRightLungRatio;
    combinedSeverity = MAX(combinedSeverity, scaledSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //Set new value & Drugs/PD
  if (combinedSeverity > 0.0)
  {
    //When albuterol is administered, the bronchodilation also causes the IE ratio to correct itself
    m_IERatioScaleFactor = 1.0 - combinedSeverity;
    m_IERatioScaleFactor *= exp(7728.4 * m_AverageLocalTissueBronchodilationEffects);

    // IE scale factor is constrained to a minimum of 0.1 and a maximum 1.0. Lower than 0.1 causes simulation instability.
    // Greater than 1.0 is not possible for patients with these conditions
    m_IERatioScaleFactor = LIMIT(m_IERatioScaleFactor, 0.1, 1.0);
  }
}

//jbw - Add description
void Respiratory::UpdateDiffusion()
{
  //These fractions all stack
  double alveoliDiffusionArea_cm2 = m_initialAlveoliDiffusionArea_cm2;

  //------------------------------------------------------------------------------------------------------
  //Impaired Alveolar Exchange
  if (m_data.GetConditions().HasImpairedAlveolarExchange())
  {

    if (!m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea() && !m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction())
    {
      /// \error Fatal: The Impaired Alveolar Exchange action must include either a surface area of fraction.
      Fatal("The Impaired Alveolar Exchange action must include either a surface area of fraction.");
    }
    
    if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea() && m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction())
    {
      /// \error Error: The Impaired Alveolar Exchange action cannot have both an impaired surface area and impaired fraction defined. Defaulting to the surface area value.
      Warning("The Impaired Alveolar Exchange action cannot have both an impaired surface area and impaired fraction defined. Defaulting to the surface area value.");
      alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedSurfaceArea(AreaUnit::cm2);
    }
    else if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea())
    {
      alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedSurfaceArea(AreaUnit::cm2);
    }
    else if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction())
    {
      alveoliDiffusionArea_cm2 = (1.0 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedFraction().GetValue()) * alveoliDiffusionArea_cm2;
    }
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, emphysemaSeverity);

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 *= gasDiffusionScalingFactor;
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);

    // Calculate the surface area contributions for each lung
    double dRightContribution = dRightLungRatio * alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * rightLungFraction;
    double dLeftContribution = dLeftLungRatio * alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * leftLungFraction;

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 = dLeftContribution + dRightContribution;
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);

    // Calculate the surface area contributions for each lung
    double dRightContribution = dRightLungRatio * alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * rightLungFraction;
    double dLeftContribution = dLeftLungRatio * alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * leftLungFraction;

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 = dLeftContribution + dRightContribution;
  }

  //------------------------------------------------------------------------------------------------------
  //Set new value
  //jbw - this will change it perminently and won't work with exacerbation - need to do it differently
  m_Patient->GetAlveoliSurfaceArea().SetValue(alveoliDiffusionArea_cm2, AreaUnit::cm2);
}

//jbw - Add description
void Respiratory::UpdatePulmonaryCapillary()
{
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    // Calculate Pulmonary Capillary Resistance Multiplier based on severities
    double dMaxSeverity = MAX(bronchitisSeverity, bronchitisSeverity);
    // Resistance is based on a a simple line fit where severity = 0 is resistance multiplier = 1.0
    // and severity = 0.6 is resistance multiplier = 2.0.
    double dSlopePulResist = 1.66666; // hard-coded slope for line
    double dPulmonaryResistanceMultiplier = 1.0 + (dMaxSeverity*dSlopePulResist);

    double dRightPulmonaryCapillaryResistance = m_RightPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    double dLeftPulmonaryCapillaryResistance = m_LeftPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::cmH2O_s_Per_L);

    dRightPulmonaryCapillaryResistance *= dPulmonaryResistanceMultiplier;
    dLeftPulmonaryCapillaryResistance *= dPulmonaryResistanceMultiplier;
    
    m_RightPulmonaryCapillary->GetNextResistance().SetValue(dRightPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    m_LeftPulmonaryCapillary->GetNextResistance().SetValue(dLeftPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  }
}

//jbw - Add description
void Respiratory::UpdateFatigue()
{
  double dyspneaSeverity = 0.0;

  //Dyspnea
  if (m_PatientActions->HasDyspnea())
  {
    dyspneaSeverity = m_PatientActions->GetDyspnea()->GetSeverity().GetValue();    
  }

  //jbw - Add fatigue

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation()->GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    }

    double combinedSeverity = 0.0;
    combinedSeverity = MAX(combinedSeverity, emphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, bronchitisSeverity);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double combinedSeverity = severity * (dLeftLungRatio * leftLungFraction + dRightLungRatio * rightLungFraction);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation()->GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome()->GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double combinedSeverity = severity * (dLeftLungRatio * leftLungFraction + dRightLungRatio * rightLungFraction);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  if (dyspneaSeverity == 1.0)
  {
    m_NotBreathing = true;
  }

  //Just reduce the tidal volume by the percentage given
  m_DriverPressure_cmH2O = m_DriverPressure_cmH2O * (1 - dyspneaSeverity);
}
*/
