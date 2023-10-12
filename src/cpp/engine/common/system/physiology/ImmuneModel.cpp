/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/system/physiology/ImmuneModel.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/patient/actions/SESepsisExacerbation.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/utils/DataTrack.h"

PUSH_EIGEN_WARNINGS
#include "Eigen/Core"
POP_EIGEN_WARNINGS

namespace pulse
{
  // Implementation of the Reynolds reduced four-variable model
  class ReynoldsModel : public ImmuneModel::SepsisModel
  {
  public:
    ReynoldsModel(double& pc, double& pgr, double& ap, double& td, double& aim) :
      m_pc(pc), m_pgr(pgr), m_ap(ap), m_td(td), m_aim(aim) {};
    virtual ~ReynoldsModel() = default;

    void AdvanceModelTime(double dt_s) override
    {
      m_h = dt_s / 3600;
      Eigen::Vector4d y;
      y[0] = m_pc;
      y[1] = m_ap;
      y[2] = m_td;
      y[3] = m_aim;

      //    // simple linear integrator
      //    auto dydt = dFdt(y);
      //    m_PathogenCount += m_h * dydt[0];
      //    m_ActivatedPhagocytes += m_h * dydt[1];
      //    m_TissueDamage += m_h * dydt[2];
      //    m_AntiInflammatoryMediators += m_h * dydt[3];

          // RK4 integrator
      auto k1 = dFdt(y);
      auto y1 = y + 0.5 * m_h * k1;
      auto k2 = dFdt(y1);
      auto y2 = y + 0.5 * m_h * k2;
      auto k3 = dFdt(y2);
      auto y3 = y + m_h * k3;
      auto k4 = dFdt(y3);
      auto y4 = m_h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);

      m_pc  += y4[0];
      m_ap  += y4[1];
      m_td  += y4[2];
      m_aim += y4[3];
    }

  protected:
    double g(double x, double C_A)
    {
      return x / (1 + (C_A / m_C_inf) * (C_A / m_C_inf));
    }
    double g_s(double x)
    {
      double x_6 = std::pow(x, 6);
      double  x_dn_6 = std::pow(m_x_dn, 6);
      return x_6 / (x_dn_6 + x_6);
    }
    double dPdt(double P, double N_star, double C_A)
    {
      return m_pgr * P * (1 - (P / m_P_inf)) - m_k_pm * m_s_m * P / (m_miu_m + m_k_mp * P) - m_k_pn * g(N_star, C_A) * P;
    }
    double dN_stardt(double P, double N_star, double D, double C_A)
    {
      double R = g(m_k_nn * N_star + m_k_np * P + m_k_nd * D, C_A);
      return m_s_nr * R / (m_miu_nr + R) - m_miu_n * N_star;
    }
    double dDdt(double N_star, double D, double C_A)
    {
      return m_k_dn * g_s(g(N_star, C_A)) - m_miu_d * D;
    }
    double dC_Adt(double N_star, double D, double C_A)
    {
      double f = g(N_star + m_k_cnd * D, C_A);
      return m_s_c + m_k_cn * f / (1 + f) - m_miu_c * C_A;
    }
    Eigen::Vector4d dFdt(Eigen::Vector4d y)
    {
      double P = y[0];
      double N_star = y[1];
      double D = y[2];
      double C_A = y[3];

      Eigen::Vector4d dydt;
      dydt[0] = dPdt(P, N_star, C_A);
      dydt[1] = dN_stardt(P, N_star, D, C_A);
      dydt[2] = dDdt(N_star, D, C_A);
      dydt[3] = dC_Adt(N_star, D, C_A);
      // Todo: remove progression acceleration.
      return 60 * dydt;
    }

    double  m_h=0;
    double& m_pgr;
    double& m_pc;
    double& m_ap;
    double& m_td;
    double& m_aim;
    // Constant sepsis model parameters
    const double m_k_pm = 0.6;
    const double m_k_mp = 0.01;
    const double m_s_m = 0.005;
    const double m_miu_m = 0.002;
    const double m_P_inf = 2e7;
    const double m_k_pn = 1.8;
    const double m_k_np = 0.1;
    const double m_k_nn = 0.01;
    const double m_s_nr = 0.08;
    const double m_miu_nr = 0.12;
    const double m_miu_n = 0.05;
    const double m_k_nd = 0.02;
    const double m_k_dn = 0.35;
    const double m_x_dn = 0.06;
    const double m_miu_d = 0.02;
    const double m_C_inf = 0.28;
    const double m_s_c = 0.0125;
    const double m_k_cn = 0.04;
    const double m_k_cnd = 48.0;
    const double m_miu_c = 0.1;
  };

  ImmuneModel::ImmuneModel(Data& data) : ImmuneSystem(data.GetLogger()), Model(data)
  {
    Clear();
    m_SepsisModel = new ReynoldsModel(m_PathogenCount,
                                      m_PathogenGrowthRate, m_ActivatedPhagocytes,
                                      m_TissueDamage, m_AntiInflammatoryMediators);
  }

  ImmuneModel::~ImmuneModel()
  {
    Clear();
  }

  void ImmuneModel::Clear()
  {
    ImmuneSystem::Clear();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  ///
  /// \details
  /// For stabilization only!
  /// Called AFTER Setup when stabilizing a new patient
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::Initialize()
  {
    Model::Initialize();

    m_PathogenCount = 0;
    m_PathogenGrowthRate = 0;
    m_ActivatedPhagocytes = 0;
    m_TissueDamage = 0;
    m_AntiInflammatoryMediators = 0.125;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes parameters for the Immune Class
  ///
  /// \details
  /// Called during both State loading and Patient Stabilization
  /// Pull and setup up our data (can be from other systems)
  /// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::SetUp()
  {
    m_InitialPathogenCount = -1;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determines if the engine is stable.
  ///
  /// \details
  /// When the engine is stable, the CDM makes this call to update the member variable.
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::AtSteadyState()
  {

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Preprocess function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::PreProcess()
  {
    Sepsis();
  }

  void ImmuneModel::Sepsis()
  {
    if (m_data.GetActions().GetPatientActions().HasSepsisExacerbation())
    {
      double PathogenGrowthRate = 0;
      SESepsisExacerbation& sepsis = m_data.GetActions().GetPatientActions().GetSepsisExacerbation();
      if (sepsis.HasProgressionSeverity())
      {
        // TODO Do we need to do some maths here to compute a rate from our 0 to 1 scale
        PathogenGrowthRate = sepsis.GetProgressionSeverity().GetValue();
      }
      else
      {
        Error("Sepsis requires a ProgressionSeverity");
        return;
      }

      if (sepsis.HasInfectionSeverity())
      {
        if (m_InitialPathogenCount < 0 || m_InitialPathogenCount != sepsis.GetInfectionSeverity().GetValue())
        {
          if (m_InitialPathogenCount != sepsis.GetInfectionSeverity().GetValue() && m_InitialPathogenCount >= 0)
            Warning("Changing sepsis infection severity can produce unreliable results.");

          m_InitialPathogenCount = sepsis.GetInfectionSeverity().GetValue();
          // TODO Do we need to do some maths here to compute a count from our 0 to 1 scale?
          m_PathogenCount = m_InitialPathogenCount;
        }

        // I don't think we need to initialize m_ActivatedPhagocytes or m_TissueDamage
        // They will start out as 0 or what ever they last were computed to be in Advance
        // Maybe AntiInflammatoryMediator could be computed to be something here?
      }
      // Currently these variable are the same, but in the future
      // we could calculate a new growth rate based on drug values
      m_PathogenGrowthRate = PathogenGrowthRate;
      m_SepsisModel->AdvanceModelTime(m_data.GetTimeStep_s());

      // TODO when do we remove sepsis? if at all
      // When the pathogen count reaches 0?
    }

    m_data.GetDataTrack().Probe("PathogenCount", m_PathogenCount);
    m_data.GetDataTrack().Probe("ActivatedPhagocytes", m_ActivatedPhagocytes);
    m_data.GetDataTrack().Probe("TissueDamage", m_TissueDamage);
    m_data.GetDataTrack().Probe("AntiInflammatoryMediators", m_AntiInflammatoryMediators);
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Process function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::Process(bool /*solve_and_transport*/)
  {
    ComputeExposedModelParameters();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// PostProcess function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::PostProcess(bool /*solve_and_transport*/)
  {

  }

  void ImmuneModel::ComputeExposedModelParameters()
  {

  }
END_NAMESPACE
