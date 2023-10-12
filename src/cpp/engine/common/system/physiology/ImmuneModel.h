/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

namespace pulse
{
  class EngineTest;
  class PBPhysiology;

  /**
   * @copydoc Physiology_ImmuneystemData
   */
  class PULSE_DECL ImmuneModel : public ImmuneSystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    ImmuneModel(Data& data);
    virtual ~ImmuneModel();

    class SepsisModel
    {
    public:
      virtual void AdvanceModelTime(double dt_s) = 0;
    };

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    void AtSteadyState() override;
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

  protected:
    void ComputeExposedModelParameters() override;
    
    //Action methods/models
    void Sepsis();
    SepsisModel* m_SepsisModel;

    // Serializable member variables (Set in Initialize and in schema)
    double                     m_InitialPathogenCount;
    double                     m_PathogenCount;
    double                     m_PathogenGrowthRate;
    double                     m_ActivatedPhagocytes;
    double                     m_TissueDamage;
    double                     m_AntiInflammatoryMediators;

    // Stateless member variable (Set in SetUp())

  };
END_NAMESPACE
