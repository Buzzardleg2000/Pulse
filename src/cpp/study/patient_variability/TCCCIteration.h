/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "ScenarioIteration.h"

#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/patient/actions/SEAirwayObstruction.h"
#include "cdm/patient/actions/SEChestOcclusiveDressing.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/patient/actions/SENeedleDecompression.h"
#include "cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include "cdm/patient/actions/SETensionPneumothorax.h"

namespace pulse::study::patient_variability
{
  class TCCCIteration : public ScenarioIteration
  {
  public:
    TCCCIteration(Logger& logger);
    virtual ~TCCCIteration();

    void Clear() override;

    bool PerformInterventions() const { return m_PerformInterventions; }
    void PerformInterventions(bool p) { m_PerformInterventions = p; }

    ParameterIteration& GetAirwayObstructionSeverity() { return m_AirwayObstructionSeverity; }
    const ParameterIteration& GetAirwayObstructionSeverity() const { return m_AirwayObstructionSeverity; }

    ParameterIteration& GetHemorrhageSeverity() { return m_HemorrhageSeverity; }
    const ParameterIteration& GetHemorrhageSeverity() const { return m_HemorrhageSeverity; }

    ParameterIteration& GetTensionPneumothoraxSeverity() { return m_TensionPneumothoraxSeverity; }
    const ParameterIteration& GetTensionPneumothoraxSeverity() const { return m_TensionPneumothoraxSeverity; }

    ParameterIteration& GetInsultDuration_s() { return m_InsultDuration_s; }
    const ParameterIteration& GetInsultDuration_s() const { return m_InsultDuration_s; }

    ParameterIteration& GetSalineAvailable() { return m_SalineAvailable; }
    const ParameterIteration& GetSalineAvailable() const { return m_SalineAvailable; }

    ParameterIteration& GetNeedleAvailable() { return m_NeedleAvailable; }
    const ParameterIteration& GetNeedleAvailable() const { return m_NeedleAvailable; }

    ParameterIteration& GetChestWrapAvailable() { return m_ChestWrapAvailable; }
    const ParameterIteration& GetChestWrapAvailable() const { return m_ChestWrapAvailable; }

  protected:
    void FixUp() override;
    void GenerateSlicedActionSets(std::pair<std::string, std::string>) override;
    void GenerateCombinationActionSets(std::pair<std::string, std::string>) override;
    void GenerateScenario(double AirwayObstructionSeverity,
                          double HemorrhageSeverity,
                          double TensionPneumothoraxSeverity,
                          double InsultDuration_s,
                          const std::string& PatientName);

    // Statefull
    bool                  m_PerformInterventions;
    ParameterIteration    m_AirwayObstructionSeverity;
    ParameterIteration    m_HemorrhageSeverity;
    ParameterIteration    m_TensionPneumothoraxSeverity;
    ParameterIteration    m_InsultDuration_s;
    ParameterIteration    m_SalineAvailable;
    ParameterIteration    m_NeedleAvailable;
    ParameterIteration    m_ChestWrapAvailable;
    // Stateless
    SEAdvanceTime         m_Adv2Insult;
    SEAirwayObstruction   m_AirwayObstruction;
    SEHemorrhage          m_Hemorrhage;
    SETensionPneumothorax m_TensionPneumothorax;
    SEAdvanceTime         m_Adv2Intervention;
    SESerializeState      m_Serialize;
    SEAdvanceTime         m_Adv2End;
  };
}
