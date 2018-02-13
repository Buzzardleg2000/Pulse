/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEEnvironmentActionCollection.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/actions/SEChangeEnvironmentConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
PROTO_PUSH
#include "bind/cdm/Scenario.pb.h"
PROTO_POP
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarHeatConductancePerArea.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"

SEEnvironmentActionCollection::SEEnvironmentActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_Change = nullptr;
  m_ThermalApplication = nullptr;
}

SEEnvironmentActionCollection::~SEEnvironmentActionCollection()
{
  Clear();
}

void SEEnvironmentActionCollection::Clear()
{
  RemoveChange();
  RemoveThermalApplication();
}

void SEEnvironmentActionCollection::Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst)
{
  if (src.HasChange())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_Change));
  if (src.HasThermalApplication())
    dst.mutable_anyaction()->AddAllocated(SEAction::Unload(*src.m_ThermalApplication));
}

bool SEEnvironmentActionCollection::ProcessAction(const SEEnvironmentAction& action, cdm::AnyEnvironmentActionData& any)
{
  const SEChangeEnvironmentConditions* conditions = dynamic_cast<const SEChangeEnvironmentConditions*>(&action);
  if (conditions != nullptr)
  {
    if (m_Change == nullptr)
      m_Change = new SEChangeEnvironmentConditions(m_Substances);
    any.set_allocated_conditions(SEChangeEnvironmentConditions::Unload(*conditions));
    SEChangeEnvironmentConditions::Load(any.conditions(), *m_Change);
    if (!m_Change->IsActive())
      RemoveChange();
    return true;
  }

  const SEThermalApplication *thermal = dynamic_cast<const SEThermalApplication*>(&action);
  if (thermal != nullptr)
  {    
    if (m_ThermalApplication == nullptr)
      m_ThermalApplication = new SEThermalApplication();
    any.set_allocated_thermalapplication(SEThermalApplication::Unload(*thermal));
    SEThermalApplication::Load(any.thermalapplication(), *m_ThermalApplication);
    if (!m_ThermalApplication->IsActive())
      RemoveThermalApplication();     
    return true;
  }

  /// \error Unsupported Action
  Error("Unsupported Environment Action");
  return false;
}

bool SEEnvironmentActionCollection::HasChange() const
{
  return m_Change == nullptr ? false : true;
}
SEChangeEnvironmentConditions* SEEnvironmentActionCollection::GetChange() const
{
  return m_Change;
}
void SEEnvironmentActionCollection::RemoveChange()
{
  SAFE_DELETE(m_Change);
}

bool SEEnvironmentActionCollection::HasThermalApplication() const
{
  return m_ThermalApplication != nullptr;
}
SEThermalApplication* SEEnvironmentActionCollection::GetThermalApplication() const
{
  return m_ThermalApplication;
}
void SEEnvironmentActionCollection::RemoveThermalApplication()
{
  SAFE_DELETE(m_ThermalApplication);
}