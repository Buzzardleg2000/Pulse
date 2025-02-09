/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SELiquidSubstanceQuantity;

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
  /**
   * @brief @copydoc Physiology_EndocrineSystemData
   * @details
   * The Pulse� endocrine system is a rudimentary system with only one stimulus (increased carbon dioxide partial pressure in the blood stream)
   * and two hormones (epinephrine and norepinephrine). The release of the hormones in response to the stimuli to represent the response of the sympathetic nervous system.
   * In the future, additional stimuli and additional hormones will be added.
   */
  class PULSE_DECL EndocrineModel : public EndocrineSystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    EndocrineModel(Data& data);
    virtual ~EndocrineModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member variables common to both homeostatic initialization and loading a state
    void SetUp() override;

    void AtSteadyState() override;
    void PreProcess() override {}
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

  protected:
    void ComputeExposedModelParameters() override;

    void SynthesizeInsulin();
    void ReleaseEpinephrineAndNorepinephrine();

    // Serializable member variables (Set in Initialize and in schema)

    // Stateless member variable (Set in SetUp())
    double                     m_insulinMolarMass_g_Per_mol;
    SELiquidSubstanceQuantity* m_aortaNorepinephrine = nullptr;
    SELiquidSubstanceQuantity* m_rKidneyEpinephrine = nullptr;
    SELiquidSubstanceQuantity* m_lKidneyEpinephrine = nullptr;
    SELiquidSubstanceQuantity* m_splanchnicInsulin = nullptr;
  };
END_NAMESPACE
