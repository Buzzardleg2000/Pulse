/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "ActionIteration.h"

#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/patient/actions/SEAirwayObstruction.h"
#include "cdm/patient/actions/SEChestOcclusiveDressing.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/patient/actions/SENeedleDecompression.h"
#include "cdm/patient/actions/SEAcuteStress.h"
#include "cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include "cdm/patient/actions/SEBrainInjury.h"
#include "cdm/patient/actions/SETensionPneumothorax.h"

namespace pulse::study::patient_variability
{
  enum class eHemorrhageWound
  {
    LeftLegLaceration = 0,
    RightArmLaceration,
    InternalLiver,
    _COUNT
  };

  enum class eTensionPneumothoraxWound
  {
    LeftClosed = 0,
    LeftOpen,
    RightClosed,
    RightOpen,
    _COUNT
  };

  class TCCCIteration : public ActionIteration
  {
  public:
    TCCCIteration(Logger& logger);
    virtual ~TCCCIteration();

    void Clear() override;

    bool PerformInterventions() const { return m_PerformInterventions; }
    void PerformInterventions(bool p) { m_PerformInterventions = p; }

    ParameterIteration<double>& GetAirwayObstructionSeverity() { return m_AirwayObstructionSeverity; }
    const ParameterIteration<double>& GetAirwayObstructionSeverity() const { return m_AirwayObstructionSeverity; }

    ParameterIteration<double>& GetHemorrhageSeverity() { return m_HemorrhageSeverity; }
    const ParameterIteration<double>& GetHemorrhageSeverity() const { return m_HemorrhageSeverity; }

    ParameterIteration<size_t>& GetHemorrhageWound() { return m_HemorrhageWound; }
    const ParameterIteration<size_t>& GetHemorrhageWound() const { return m_HemorrhageWound; }

    ParameterIteration<double>& GetStressSeverity() { return m_StressSeverity; }
    const ParameterIteration<double>& GetStressSeverity() const { return m_StressSeverity; }

    ParameterIteration<double>& GetTBISeverity() { return m_TBISeverity; }
    const ParameterIteration<double>& GetTBISeverity() const { return m_TBISeverity; }

    ParameterIteration<double>& GetTensionPneumothoraxSeverity() { return m_TensionPneumothoraxSeverity; }
    const ParameterIteration<double>& GetTensionPneumothoraxSeverity() const { return m_TensionPneumothoraxSeverity; }

    ParameterIteration<size_t>& GetTensionPneumothoraxWound() { return m_TensionPneumothoraxWound; }
    const ParameterIteration<size_t>& GetTensionPneumothoraxWound() const { return m_TensionPneumothoraxWound; }

    ParameterIteration<double>& GetInsultDuration_s() { return m_InsultDuration_s; }
    const ParameterIteration<double>& GetInsultDuration_s() const { return m_InsultDuration_s; }

    ParameterIteration<double>& GetSalineAvailable() { return m_SalineAvailable; }
    const ParameterIteration<double>& GetSalineAvailable() const { return m_SalineAvailable; }

    ParameterIteration<double>& GetNeedleAvailable() { return m_NeedleAvailable; }
    const ParameterIteration<double>& GetNeedleAvailable() const { return m_NeedleAvailable; }

    ParameterIteration<double>& GetChestWrapAvailable() { return m_ChestWrapAvailable; }
    const ParameterIteration<double>& GetChestWrapAvailable() const { return m_ChestWrapAvailable; }

  protected:
    void FixUp() override;
    void GenerateSlicedActionSets(std::pair<std::string, std::string>) override;
    void GenerateCombinationActionSets(std::pair<std::string, std::string>) override;
    void GenerateScenario(double AirwayObstructionSeverity,
                          double HemorrhageSeverity,
                          size_t HemorrhageWound,
                          double StressSeverity,
                          double TBISeverity,
                          double TensionPneumothoraxSeverity,
                          size_t TensionPneumothoraxWound,
                          double InsultDuration_s,
                          const std::string& PatientName);

    // Stateful
    // Injuries
    ParameterIteration<double>    m_AirwayObstructionSeverity;
    ParameterIteration<double>    m_HemorrhageSeverity;
    ParameterIteration<size_t>    m_HemorrhageWound;
    ParameterIteration<double>    m_StressSeverity;
    ParameterIteration<double>    m_TBISeverity;
    ParameterIteration<double>    m_TensionPneumothoraxSeverity;
    ParameterIteration<size_t>    m_TensionPneumothoraxWound;
    ParameterIteration<double>    m_InsultDuration_s;
    // Interventions
    bool                          m_PerformInterventions;
    ParameterIteration<double>    m_SalineAvailable;
    ParameterIteration<double>    m_NeedleAvailable;
    ParameterIteration<double>    m_ChestWrapAvailable;

    // Stateless
    SEAdvanceTime         m_Adv2Insult;
    SEAirwayObstruction   m_AirwayObstruction;
    SEBrainInjury         m_BrainInjury;
    SEHemorrhage          m_Hemorrhage;
    SEAcuteStress         m_Stress;
    SETensionPneumothorax m_TensionPneumothorax;
    SEAdvanceTime         m_Adv2Intervention;
    SEAdvanceTime         m_Adv2End;
  };
}
