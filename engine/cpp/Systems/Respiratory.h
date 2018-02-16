/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "Controller/System.h"
#include "system/physiology/SERespiratorySystem.h"
class SEPatient;
class SEPatientActionCollection;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEFluidCircuitCalculator;
class SEGasTransporter;
class SELiquidTransporter;
class RunningAverage;
class SEConsciousRespirationCommand;
class SEPulmonaryFunctionTest;
PULSE_BIND_DECL(RespiratorySystemData)

/**
* @brief The %Respiratory System class handles the analysis and storage of
* data related the respiratory physiology and pathology.
* @details The goal of the Respiratory System class is to handle the analysis
* and storage of data representing the ventilation and gas exchange of the human
* respiratory system. The class handles the analysis of changes in the lungs volume
* and pressure data during the flow and transfer of gases between the lungs, the
* blood and the outside environment. The class updates the volume and volume fractions
* of gases in the lungs, and ensures the integration and flow of data between the
* respiratory system and the anesthesia machine during mechanical ventilation.
*/
class PULSE_DECL Respiratory : public SERespiratorySystem, public PulseSystem
{
  friend PulseController;
  friend class PulseEngineTest;
protected:

  Respiratory(PulseController& data);
  PulseController& m_data;

public:
  virtual ~Respiratory();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();

  static void Load(const pulse::RespiratorySystemData& src, Respiratory& dst);
  static pulse::RespiratorySystemData* Unload(const Respiratory& src);
protected:
  static void Serialize(const pulse::RespiratorySystemData& src, Respiratory& dst);
  static void Serialize(const Respiratory& src, pulse::RespiratorySystemData& dst);

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

public:
  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

  bool CalculatePulmonaryFunctionTest(SEPulmonaryFunctionTest& pft);
private:
  //Tuning
  void TuneCircuit();

  //Conditions
  void COPD();
  void ImpairedAlveolarExchange();
  void LobarPneumonia();

  //PreProcess
  void UpdatePleuralCompliance();
  //Actions
  void AirwayObstruction();
  void BronchoConstriction();
  void BronchoDilation();
  void Intubation();
  void Pneumothorax();
  void ConsciousRespiration();
  /**/void ProcessConsciousRespiration(SEConsciousRespirationCommand& cmd);
  void MechanicalVentilation();
  void Apnea();
  // Driver
  void RespiratoryDriver();

  // Shared Utility Methods for Actions/Driver
  // Asthma/COPD
  /**/void UpdateObstructiveResistance();
  /**/void UpdateIERatio();
  // LobarPneumonia/COPD
  /**/void UpdateAlveoliCompliance(double dCompilanceScalingFactor, double dLeftLungFraction, double dRightLungFraction);
  /**/void UpdateGasDiffusionSurfaceArea(double dFractionalArea, double dLeftLungFraction, double dRightLungFraction);
  // COPD
  /**/void UpdatePulmonaryCapillaryResistance(double dResistanceScalingFactor, double dLeftLungFraction, double dRightLungFraction);
  // Pneumothorax
  void DoLeftNeedleDecompression(double dFlowResistance);
  void DoRightNeedleDecompression(double dFlowResistance);
  // Aerosol Deposition and various Effects
  void ProcessAerosolSubstances();
  // Driver/Conscious Breath
  /**/double VolumeToDriverPressure(double TargetVolume);

  //Process
  void CalculateVitalSigns();

  // Serializable member variables (Set in Initialize and in schema)
  //   Initial patient values
  double m_InitialExpiratoryReserveVolume_L;
  double m_InitialFunctionalResidualCapacity_L;
  double m_InitialInspiratoryCapacity_L;
  double m_InitialResidualVolume_L;
  //   CalculateVitalSigns()
  bool   m_bNotBreathing;
  double m_TopBreathTotalVolume_L;
  double m_TopBreathAlveoliVolume_L;
  double m_TopBreathDeadSpaceVolume_L;
  double m_TopBreathPleuralPressure_cmH2O;
  double m_LastCardiacCycleBloodPH;
  double m_PreviousTotalLungVolume_L;
  RunningAverage* m_BloodPHRunningAverage;
  //   Respiratory Driver
  bool   m_BreathingCycle;
  double m_ArterialO2PartialPressure_mmHg;
  double m_ArterialCO2PartialPressure_mmHg;
  double m_BreathingCycleTime_s;
  double m_BreathTimeExhale_min;
  double m_DefaultDrivePressure_cmH2O;
  double m_DriverPressure_cmH2O;
  double m_DriverPressureMin_cmH2O;
  double m_ElapsedBreathingCycleTime_min;
  double m_IEscaleFactor;
  double m_InstantaneousFunctionalResidualCapacity_L;
  double m_MaxDriverPressure_cmH2O;    
  double m_PeakRespiratoryDrivePressure_cmH2O;
  double m_PreviousTargetAlveolarVentilation_L_Per_min;
  double m_VentilationFrequency_Per_min;
  double m_VentilationToTidalVolumeSlope;
  RunningAverage* m_ArterialO2RunningAverage_mmHg;
  RunningAverage* m_ArterialCO2RunningAverage_mmHg;
  //   Conscious Breathing
  bool   m_ConsciousBreathing;
  double m_ConsciousRespirationPeriod_s;
  double m_ConsciousRespirationRemainingPeriod_s;
  double m_ExpiratoryReserveVolumeFraction;
  double m_InspiratoryCapacityFraction;
  double m_ConsciousStartPressure_cmH2O;
  double m_ConsciousEndPressure_cmH2O;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  double m_dt_min;
  // Configuration parameters
  double m_CentralControlGainConstant;
  double m_dDefaultOpenResistance_cmH2O_s_Per_L;
  double m_dDefaultClosedResistance_cmH2O_s_Per_L;
  double m_PeripheralControlGainConstant;
  double m_PleuralComplianceSensitivity_Per_L;
  double m_dRespOpenResistance_cmH2O_s_Per_L;
  double m_dRespClosedResistance_cmH2O_s_Per_L;
  double m_VentilationTidalVolumeIntercept;
  double m_VentilatoryOcclusionPressure_cmH2O;
  // State between functions (i.e. shared between methods in preprocess, set to a default value at the start of preprocess)
  double m_AverageLocalTissueBronchodilationEffects;

  // Patient
  SEPatient*                 m_Patient;
  SEPatientActionCollection* m_PatientActions;
  //Compartments
  SEGasCompartment*          m_Environment;
  SELiquidCompartment*       m_AerosolMouth;
  SELiquidCompartment*       m_AerosolCarina;
  SELiquidCompartment*       m_AerosolLeftDeadSpace;
  SELiquidCompartment*       m_AerosolLeftAlveoli;
  SELiquidCompartment*       m_AerosolRightDeadSpace;
  SELiquidCompartment*       m_AerosolRightAlveoli;
  SELiquidCompartment*       m_LeftLungExtravascular;
  SELiquidCompartment*       m_RightLungExtravascular;
  SEGasCompartment*          m_Lungs;
  SEGasCompartment*          m_Carina;
  SEGasSubstanceQuantity*    m_CarinaO2;
  SELiquidSubstanceQuantity* m_AortaO2;
  SELiquidSubstanceQuantity* m_AortaCO2;
  SEGasSubstanceQuantity*    m_LeftAlveoliO2;
  SEGasSubstanceQuantity*    m_RightAlveoliO2;
  std::vector<SELiquidCompartment*> m_AerosolEffects;
  SEGasCompartment*          m_MechanicalVentilatorConnection;
  //Circuits
  SEFluidCircuit*            m_RespiratoryCircuit;
  //Nodes
  SEFluidCircuitNode*        m_Mouth;
  SEFluidCircuitNode*        m_LeftAlveoli;
  SEFluidCircuitNode*        m_LeftDeadSpace;
  SEFluidCircuitNode*        m_LeftPleural;
  SEFluidCircuitNode*        m_RespiratoryMuscle;
  SEFluidCircuitNode*        m_RightAlveoli;
  SEFluidCircuitNode*        m_RightDeadSpace;
  SEFluidCircuitNode*        m_RightPleural;
  SEFluidCircuitNode*        m_Ambient;
  SEFluidCircuitNode*        m_Stomach;
  //Paths
  SEFluidCircuitPath*        m_CarinaToLeftAnatomicDeadSpace;
  SEFluidCircuitPath*        m_CarinaToRightAnatomicDeadSpace;
  SEFluidCircuitPath*        m_LeftAnatomicDeadSpaceToLeftAlveoli;
  SEFluidCircuitPath*        m_RightAnatomicDeadSpaceToRightAlveoli;
  SEFluidCircuitPath*        m_RightPleuralToRespiratoryMuscle;
  SEFluidCircuitPath*        m_LeftPleuralToRespiratoryMuscle;
  SEFluidCircuitPath*        m_DriverPressurePath;
  SEFluidCircuitPath*        m_LeftDriverPressurePath;
  SEFluidCircuitPath*        m_MouthToCarina;
  SEFluidCircuitPath*        m_MouthToStomach;
  SEFluidCircuitPath*        m_EnvironmentToLeftChestLeak;
  SEFluidCircuitPath*        m_EnvironmentToRightChestLeak;
  SEFluidCircuitPath*        m_LeftAlveoliLeakToLeftPleural;
  SEFluidCircuitPath*        m_RightAlveoliLeakToRightPleural;
  SEFluidCircuitPath*        m_LeftPleuralToEnvironment;
  SEFluidCircuitPath*        m_RightPleuralToEnvironment;
  SEFluidCircuitPath*        m_RightAlveoliToRightPleuralConnection;
  SEFluidCircuitPath*        m_LeftAlveoliToLeftPleuralConnection;
  SEFluidCircuitPath*        m_RightAnatomicDeadSpaceToRightPleuralConnection;
  SEFluidCircuitPath*        m_LeftAnatomicDeadSpaceToLeftPleuralConnection;
  SEFluidCircuitPath*        m_RightPulmonaryCapillary;
  SEFluidCircuitPath*        m_LeftPulmonaryCapillary;
  SEFluidCircuitPath*        m_ConnectionToMouth;
  SEFluidCircuitPath*        m_GroundToConnection;

  SEFluidCircuitCalculator*  m_Calculator;
  SEGasTransporter*          m_GasTransporter;
  SELiquidTransporter*       m_AerosolTransporter;
};
