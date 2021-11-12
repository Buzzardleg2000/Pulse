/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulseEngine.h"
#include "PulseConfiguration.h"
#include "cdm/substance/SESubstanceManager.h"

#include "SEValidationTarget.h"

namespace pulse::study::circuit_optimization
{
  class CircuitOptimizer : public Loggable, protected SEEventHandler
  {
  public:
    CircuitOptimizer(Logger* logger=nullptr);
    virtual ~CircuitOptimizer();

    bool ConvergeToHemodynamicsTargets(size_t maxLoops, std::vector<SEValidationTarget*>& targets);

    virtual void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr) override;

  protected:
    bool GenerateHemodynamicsData(PulseConfiguration& cfg, std::vector<SEValidationTarget*>& targets);
    bool ComputeNewModifiers(PulseConfiguration& cfg, std::vector<SEValidationTarget*>& targets);
    void WriteModifiers(const PulseConfiguration& cfg, const std::string& filename);

    unsigned int       m_SimulationNum = 0;
    unsigned int       m_StopAtCycle = 5;
    unsigned int       m_CardiacCycle = 0;
    bool               m_StartOfCardiacCycle = false;
    SESubstanceManager m_SubMgr;
  };
}
