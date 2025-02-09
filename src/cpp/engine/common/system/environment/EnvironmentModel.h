/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEPatient;
class SEGasCompartment;
class SELiquidCompartment;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;

namespace pulse
{
  class PBEnvironment;
  class EngineTest;
  /**
   * @brief The %Environment class characterizes the environment and manages interactions between the body its surroundings.
   */
  class PULSE_DECL EnvironmentModel : public EnvironmentSystem, public Model
  {
    friend PBEnvironment;//friend the serialization class
    friend EngineTest;
  public:
    EnvironmentModel(Data& data);
    virtual ~EnvironmentModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    void AtSteadyState() override;
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

    void StateChange() override;

  protected:
    void ComputeExposedModelParameters() override;

    void ProcessActions();
    void CalculateSupplementalValues();
    /**/double AntoineEquation(double dTemperature_C);
    void CalculateRadiation();
    void CalculateConvection();
    void CalculateEvaporation();
    void CalculateRespiration();

    // Serializable member variables (Set in Initialize and in schema)
    double m_PatientEquivalentDiameter_m;// For Convection calculations

    // Stateless member variable (Calculated in Supplemental Method and used in other methods)
    double m_dLewisRelation;
    double m_dHeatOfVaporizationOfWater_J_Per_kg;
    double m_dWaterVaporPressureInAmbientAir_Pa;
    double m_dWaterVaporPressureAtSkin_Pa;
    //Prandtl number calculation
    double m_WaterSpecificHeat_J_Per_kg_K;
    double m_WaterViscosity_N_s_Per_m2;
    double m_WaterThermalConductivity_W_Per_m_K;
    //Grashof number calculation
    double m_ThermalExpansion_Per_K;

    // Stateless member variable (Set in SetUp())
    // Compartments
    SEGasCompartment* m_AmbientGases;
    SELiquidCompartment* m_AmbientAerosols;
    //Circuits                     
    SEThermalCircuit* m_EnvironmentCircuit;
    //Nodes                        
    SEThermalCircuitNode* m_ThermalEnvironment;
    SEThermalCircuitNode* m_SkinNode;
    SEThermalCircuitNode* m_ClothingNode;
    SEThermalCircuitNode* m_EnclosureNode;
    //Paths                        
    SEThermalCircuitPath* m_SkinToClothing;
    SEThermalCircuitPath* m_ActiveHeatTransferRatePath;
    SEThermalCircuitPath* m_ActiveTemperaturePath;
    SEThermalCircuitPath* m_ActiveSwitchPath;
    SEThermalCircuitPath* m_ClothingToEnclosurePath;
    SEThermalCircuitPath* m_GroundToEnclosurePath;
    SEThermalCircuitPath* m_ClothingToEnvironmentPath;
    SEThermalCircuitPath* m_GroundToEnvironmentPath;
    SEThermalCircuitPath* m_EnvironmentSkinToGroundPath;
    SEThermalCircuitPath* m_EnvironmentCoreToGroundPath;
  };
}
