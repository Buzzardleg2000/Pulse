/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

PUSH_EIGEN_WARNINGS
#include "Eigen/Core"
POP_EIGEN_WARNINGS

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
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
    
    //Action methods
    /**/void InitializeSepsisModel();
    /**/void AdvanceSepsisModel();
    /**/double g(double x, double C_A);
    /**/double g_s(double x);
    /**/double dPdt(double P, double N_star, double C_A);
    /**/double dN_stardt(double P, double N_star, double D, double C_A);
    /**/double dDdt(double N_star, double D, double C_A);
    /**/double dC_Adt(double N_star, double D, double C_A);
    /**/Eigen::Vector4d dFdt(Eigen::Vector4d y);

    // Serializable member variables (Set in Initialize and in schema)
    double                     m_PathogenGrowthRate;
    double                     m_PathogenCount;
    double                     m_ActivatedPhagocytes;
    double                     m_TissueDamage;
    double                     m_AntiInflammatoryMediators;

    // Constant sepsis model parameters (Set in Initialize)
    double                     m_h;
    double                     m_k_pm;
    double                     m_k_mp;
    double                     m_s_m;
    double                     m_miu_m;
    double                     m_P_inf;
    double                     m_k_pn;
    double                     m_k_np;
    double                     m_k_nn;
    double                     m_s_nr;
    double                     m_miu_nr;
    double                     m_miu_n;
    double                     m_k_nd;
    double                     m_k_dn;
    double                     m_x_dn;
    double                     m_miu_d;
    double                     m_C_inf;
    double                     m_s_c;
    double                     m_k_cn;
    double                     m_k_cnd;
    double                     m_miu_c;

    // Stateless member variable (Set in SetUp())
    double                     m_CurrentInfectionSeverity;
    double                     m_CurrentProgressionSeverity;

  };
END_NAMESPACE