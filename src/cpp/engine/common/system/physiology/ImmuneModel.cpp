/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/system/physiology/ImmuneModel.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/patient/actions/SESepsisExacerbation.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/utils/DataTrack.h"

namespace pulse
{
  ImmuneModel::ImmuneModel(Data& data) : ImmuneSystem(data.GetLogger()), Model(data)
  {
    Clear();
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

    m_PathogenGrowthRate = 0;
    m_PathogenCount = 0;
    m_ActivatedPhagocytes = 0;
    m_TissueDamage = 0;
    m_AntiInflammatoryMediators = 0;
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
    m_CurrentInfectionSeverity = -1;
    m_CurrentProgressionSeverity = 0;
    if (m_data.GetActions().GetPatientActions().HasSepsisExacerbation())
    {
      SESepsisExacerbation& sepsis = m_data.GetActions().GetPatientActions().GetSepsisExacerbation();
      if (sepsis.HasInfectionSeverity())
        m_CurrentInfectionSeverity = sepsis.GetInfectionSeverity().GetValue();
      if (sepsis.HasProgressionSeverity())
        m_CurrentProgressionSeverity = sepsis.GetProgressionSeverity().GetValue();
    }
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
    if (m_data.GetActions().GetPatientActions().HasSepsisExacerbation())
    {
      SESepsisExacerbation& sepsis = m_data.GetActions().GetPatientActions().GetSepsisExacerbation();
      if (sepsis.HasProgressionSeverity())
        m_CurrentProgressionSeverity = sepsis.GetProgressionSeverity().GetValue();

      if (sepsis.HasInfectionSeverity())
      {
        if (m_CurrentInfectionSeverity >= 0)
          Warning("Changing sepsis infection severity can produce unreliable results.");
        if (m_CurrentInfectionSeverity != sepsis.GetInfectionSeverity().GetValue())
        {
          m_CurrentInfectionSeverity = sepsis.GetInfectionSeverity().GetValue();
          InitializeSepsisModel();
        }
      }
      AdvanceSepsisModel();
    }

    m_data.GetDataTrack().Probe("PathogenGrowthRate", m_PathogenGrowthRate);
    m_data.GetDataTrack().Probe("PathogenCount", m_PathogenCount);
    m_data.GetDataTrack().Probe("ActivatedPhagocytes", m_ActivatedPhagocytes);
    m_data.GetDataTrack().Probe("TissueDamage", m_TissueDamage);
    m_data.GetDataTrack().Probe("AntiInflammatoryMediators", m_AntiInflammatoryMediators);
  }

  void ImmuneModel::InitializeSepsisModel()
  {
    // I am assuming initialization is only a function of the infection severity

    // Set our sepsis count based on the infection severity
    m_PathogenCount = m_CurrentInfectionSeverity;
    // I don't think we need to initialize m_ActivatedPhagocytes or m_TissueDamage
    // They will start out as 0 or what ever they last were computed to be in AdvanceSepsisModel

    // Maybe AntiInflammatoryMediator could be computed to be something here?
  }

  void ImmuneModel::AdvanceSepsisModel()
  {

    m_PathogenGrowthRate = m_CurrentProgressionSeverity*24;
    // Implementation of the Reynolds reduced four-variable model
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
