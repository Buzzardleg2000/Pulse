/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/system/physiology/ImmuneModel.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/patient/actions/SESepsisExacerbation.h"
#include "cdm/patient/conditions/SESepsis.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarAmount.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/GeneralMath.h"

PUSH_EIGEN_WARNINGS
#include "Eigen/Core"
POP_EIGEN_WARNINGS

namespace pulse
{
  // Implementation of the Reynolds reduced four-variable model
  class ReynoldsSepsisModelImpl : public ImmuneModel::ReynoldsSepsisModel
  {
  public:
    ReynoldsSepsisModelImpl() :
      m_pc(PathogenCount),m_pgr(PathogenGrowthRate),
      m_ap(ActivatedPhagocytes), m_td(TissueDamage), m_aim(AntiInflammatoryMediators) {};
    virtual ~ReynoldsSepsisModelImpl() = default;

    bool Active() const override
    {
      // TODO can we stop calculating the model?
      if (PathogenCount > 0)
        return true;
      if (PathogenGrowthRate > 0)
        return true;
      if (ActivatedPhagocytes > 0)
        return true;
      if (TissueDamage > 0)
        return true;
      // AntiInflammatoryMediators?

      return false;
    }

    double InfectionSeverityToPathogenCount(double InfectionSeverity) override
    {
      std::vector<std::pair<double, double>> pts;
      pts.push_back(std::make_pair(0.0, 0.0));
      pts.push_back(std::make_pair(0.3, 1.0));
      pts.push_back(std::make_pair(0.6, 1.25));
      pts.push_back(std::make_pair(0.9, 1.4));
      pts.push_back(std::make_pair(1.0, 1.5));
      return GeneralMath::PiecewiseLinearInterpolator(pts, InfectionSeverity);
    }

    double ProgressionSeverityToPathogenGrowthRate(double ProgressionSeverity) override
    {
      std::vector<std::pair<double, double>> pts;
      pts.push_back(std::make_pair(0.0, 0.0));
      pts.push_back(std::make_pair(0.3, 0.25));
      pts.push_back(std::make_pair(0.6, 0.33));
      pts.push_back(std::make_pair(0.9, 0.6));
      pts.push_back(std::make_pair(1.0, 0.65));
      return GeneralMath::PiecewiseLinearInterpolator(pts, ProgressionSeverity);
    }

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
      return dydt;
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
    const double m_P_inf = 20;
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
    m_SepsisModel = new ReynoldsSepsisModelImpl();
  }

  ImmuneModel::~ImmuneModel()
  {
    Clear();
    delete m_SepsisModel;
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

    m_ActiveSepsis = false;
    m_InitialSepsisInfectionSeverity = -1;
    m_SepsisModel->PathogenCount = 0; // max 20e6
    m_SepsisModel->PathogenGrowthRate = 0;
    m_SepsisModel->ActivatedPhagocytes = 0;
    m_SepsisModel->TissueDamage = 0;
    m_SepsisModel->AntiInflammatoryMediators = 0.125;
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
    if (m_data.GetState() == EngineState::AtInitialStableState)
    {
      if (m_data.GetConditions().HasSepsis() && m_data.GetConditions().GetSepsis().HasProgressionDuration())
      {
        SESepsis& sepsis = m_data.GetConditions().GetSepsis();
        m_InitialSepsisInfectionSeverity = sepsis.GetInfectionSeverity().GetValue();
        m_SepsisModel->PathogenCount = m_SepsisModel->InfectionSeverityToPathogenCount(m_InitialSepsisInfectionSeverity);
        m_SepsisModel->PathogenGrowthRate = m_SepsisModel->ProgressionSeverityToPathogenGrowthRate(sepsis.GetProgressionSeverity().GetValue());

        // Advance sepsis model the duration
        double duration_s = 0;
        if (m_data.GetConditions().GetSepsis().HasProgressionDuration())
        {
          duration_s = m_data.GetConditions().GetSepsis().GetProgressionDuration(TimeUnit::s);
          if (duration_s > 0)
          {
            size_t steps = std::ceil(duration_s / m_data.GetTimeStep_s());
            Info("Simulating " + m_data.GetConditions().GetSepsis().GetProgressionDuration().ToString() + " of Sepsis");
            Info("  -That is " + std::to_string(steps) + " time steps.");

            for (size_t i = 0; i < steps; i++)
              m_SepsisModel->AdvanceModelTime(m_data.GetTimeStep_s());
          }
        }

        // NOTE: Don't active the sepsis model until after we have stabilized to the values we computed in the sepsis model
      }
    }
    else if (m_data.GetState() == EngineState::Active)
    {
      if (m_data.GetConditions().HasSepsis())
        m_ActiveSepsis = true;
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Preprocess function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void ImmuneModel::PreProcess()
  {
    if (m_data.GetActions().GetPatientActions().HasSepsisExacerbation())
    {
      m_ActiveSepsis = true;
      SESepsisExacerbation& sepsis = m_data.GetActions().GetPatientActions().GetSepsisExacerbation();

      if (sepsis.HasInfectionSeverity())
      {
        // You really should not change the initial infection severity in the middle of a run
        if (m_InitialSepsisInfectionSeverity < 0 || m_InitialSepsisInfectionSeverity != sepsis.GetInfectionSeverity().GetValue())
        {
          if (m_InitialSepsisInfectionSeverity != sepsis.GetInfectionSeverity().GetValue() && m_InitialSepsisInfectionSeverity >= 0)
            Warning("Changing sepsis infection severity can produce unreliable results.");

          m_InitialSepsisInfectionSeverity = sepsis.GetInfectionSeverity().GetValue();
          m_SepsisModel->PathogenCount = m_SepsisModel->InfectionSeverityToPathogenCount(m_InitialSepsisInfectionSeverity);
        }

        m_SepsisModel->PathogenGrowthRate = m_SepsisModel->ProgressionSeverityToPathogenGrowthRate(sepsis.GetProgressionSeverity().GetValue());
      }
    }
    else
      m_ActiveSepsis = m_SepsisModel->Active();

    if(m_ActiveSepsis)
      Sepsis();

    m_data.GetDataTrack().Probe("PathogenCount", m_SepsisModel->PathogenCount);
    m_data.GetDataTrack().Probe("ActivatedPhagocytes", m_SepsisModel->ActivatedPhagocytes);
    m_data.GetDataTrack().Probe("TissueDamage", m_SepsisModel->TissueDamage);
    m_data.GetDataTrack().Probe("AntiInflammatoryMediators", m_SepsisModel->AntiInflammatoryMediators);

    // TODO Translate the sepsis model parameters to the CDM
    /*
    GetActivatedPhagocytes().SetValue(m_ActivatedPhagocytes, AmountUnit::ct);
    GetAntiInflammatoryMediators().SetValue(m_AntiInflammatoryMediators, AmountUnit::ct);
    GetPathogenCount().SetValue(m_PathogenCount, AmountUnit::ct);
    GetTissueDamage().SetValue(m_TissueDamage);
    */
  }

  void ImmuneModel::Sepsis()
  {
    // TODO calculate a new growth rate based on drug values

    // TODO when do we remove sepsis? if at all
    // When the pathogen count reaches 0?

    m_SepsisModel->AdvanceModelTime(m_data.GetTimeStep_s());
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
