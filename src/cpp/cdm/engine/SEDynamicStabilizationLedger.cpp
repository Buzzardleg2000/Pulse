/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDynamicStabilizationLedger.h"
#include "cdm/engine/SEDynamicStabilizationEngineConvergence.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDynamicStabilizationPropertyConvergence.h"
#include "cdm/properties/SEScalarTime.h"

SEDynamicStabilizationLedger::SEDynamicStabilizationLedger(double timeStep_s, const SEDynamicStabilizationEngineConvergence& criteria, Logger* logger) : Loggable(logger), m_properties(criteria.GetPropertyConvergence())
{
  m_dT_s = timeStep_s;
  m_totTime_s = 0;
  m_passTime_s = 0;
  m_optsPassTime_s = 0;
  m_covTime_s = criteria.GetConvergenceTime(TimeUnit::s);
  m_minTime_s = criteria.GetMinimumReactionTime(TimeUnit::s);
  m_maxTime_s = criteria.GetMaximumAllowedStabilizationTime(TimeUnit::s) + m_minTime_s;
  // We will run for at least minTime, THEN to the max time, so maxTime_s is the sum of both

  m_converged = false;
  m_convergedOptional = false;
  m_exceededTime = false;
  m_hasOptionalProperties = false;
  for (SEDynamicStabilizationPropertyConvergence* pc : m_properties)
  {
    if (pc->IsOptional())
    {
      m_hasOptionalProperties = true;
      break;
    }
  }
  if (!m_hasOptionalProperties)
    m_convergedOptional = true;
}

void SEDynamicStabilizationLedger::TestConvergence()
{
  bool   passedTimeStep = false;
  bool   passedTimeStepOptions = false;

  m_totTime_s += m_dT_s;
  if (m_totTime_s < m_minTime_s)
    return;// Wait for it

  std::stringstream ss;
  passedTimeStep = true;
  passedTimeStepOptions = true;
  for (SEDynamicStabilizationPropertyConvergence* pc : m_properties)
  {
    bool b = pc->Test(m_totTime_s);
    if (pc->IsOptional())
    {
      passedTimeStepOptions &= b;
    }
    else
    {
      passedTimeStep &= b;
    }
  }

  if (passedTimeStep)
    m_passTime_s += m_dT_s;
  else
    m_passTime_s = 0;
  if (passedTimeStepOptions)
    m_optsPassTime_s += m_dT_s;
  else
    m_optsPassTime_s = 0;

  if (!m_converged && m_passTime_s >= m_covTime_s)
  {// We have converged
    m_converged = true;
    if (m_hasOptionalProperties)
      Info("We have passed required convergence criteria, attempting to converge optional properties from multiple conditions.");
  }
  if (m_converged && m_optsPassTime_s >= m_covTime_s)
  {// We have converged optional props
    m_convergedOptional = true;
    Info("We have passed required and optional convergence criteria.");
  }

  if (m_totTime_s > m_maxTime_s)
  {
    m_converged = false;
    m_convergedOptional = false;
    m_exceededTime = true;
  }
}