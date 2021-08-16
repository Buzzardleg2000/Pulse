/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEEquipmentActionCollection.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineChamber.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
#include "cdm/system/equipment/inhaler/SEInhaler.h"
#include "cdm/system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorHold.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

SEEquipmentActionCollection::SEEquipmentActionCollection(SESubstanceManager& subMgr) : m_SubMgr(subMgr), Loggable(subMgr.GetLogger())
{
  m_AnesthesiaMachineConfiguration = nullptr;
  m_BagValveMaskConfiguration = nullptr;
  m_InhalerConfiguration = nullptr;
  m_MechanicalVentilatorConfiguration = nullptr;

  m_AnesthesiaMachineOxygenTankPressureLoss = nullptr;
  m_AnesthesiaMachineOxygenWallPortPressureLoss = nullptr;

  m_AnesthesiaMachineExpiratoryValveLeak = nullptr;
  m_AnesthesiaMachineExpiratoryValveObstruction = nullptr;
  m_AnesthesiaMachineInspiratoryValveLeak = nullptr;
  m_AnesthesiaMachineInspiratoryValveObstruction = nullptr;
  m_AnesthesiaMachineMaskLeak = nullptr;
  m_AnesthesiaMachineSodaLimeFailure = nullptr;
  m_AnesthesiaMachineTubeCuffLeak = nullptr;
  m_AnesthesiaMachineVaporizerFailure = nullptr;
  m_AnesthesiaMachineVentilatorPressureLoss = nullptr;
  m_AnesthesiaMachineYPieceDisconnect = nullptr;

  m_BagValveMaskAutomated = nullptr;
  m_BagValveMaskInstantaneous = nullptr;
  m_BagValveMaskSqueeze = nullptr;
  m_MechanicalVentilatorHold = nullptr;
}

SEEquipmentActionCollection::~SEEquipmentActionCollection()
{
  Clear();
}

void SEEquipmentActionCollection::Clear()
{
  // State
  RemoveAnesthesiaMachineConfiguration();
  RemoveInhalerConfiguration();
  RemoveMechanicalVentilatorConfiguration();
  RemoveBagValveMaskConfiguration();
  // Incidents
  RemoveAnesthesiaMachineOxygenTankPressureLoss();
  RemoveAnesthesiaMachineOxygenWallPortPressureLoss();
  // Failures
  RemoveAnesthesiaMachineExpiratoryValveLeak();
  RemoveAnesthesiaMachineExpiratoryValveObstruction();
  RemoveAnesthesiaMachineInspiratoryValveLeak();
  RemoveAnesthesiaMachineInspiratoryValveObstruction();
  RemoveAnesthesiaMachineMaskLeak();
  RemoveAnesthesiaMachineSodaLimeFailure();
  RemoveAnesthesiaMachineTubeCuffLeak();
  RemoveAnesthesiaMachineVaporizerFailure();
  RemoveAnesthesiaMachineVentilatorPressureLoss();
  RemoveAnesthesiaMachineYPieceDisconnect();
  // Actions
  RemoveBagValveMaskAutomated();
  RemoveBagValveMaskInstantaneous();
  RemoveBagValveMaskSqueeze();
  RemoveMechanicalVentilatorHold();
}

bool SEEquipmentActionCollection::ProcessAction(const SEEquipmentAction& action)
{
  const SEMechanicalVentilatorAction* mva = dynamic_cast<const SEMechanicalVentilatorAction*>(&action);
  if (mva != nullptr)
  {
    const SEMechanicalVentilatorConfiguration* config = dynamic_cast<const SEMechanicalVentilatorConfiguration*>(&action);
    if (config != nullptr)
    {
      GetMechanicalVentilatorConfiguration().Copy(*config, m_SubMgr, true);
      m_MechanicalVentilatorConfiguration->Activate();
      if (!m_MechanicalVentilatorConfiguration->IsActive())
        RemoveMechanicalVentilatorConfiguration();
      return true;
    }

    const SEMechanicalVentilatorHold* hold = dynamic_cast<const SEMechanicalVentilatorHold*>(&action);
    if (hold != nullptr)
    {
      GetMechanicalVentilatorHold().Copy(*hold, true);
      m_MechanicalVentilatorHold->Activate();
      if (!m_MechanicalVentilatorHold->IsActive())
        RemoveMechanicalVentilatorHold();
      return true;
    }
  }

  const SEBagValveMaskAction* bvm = dynamic_cast<const SEBagValveMaskAction*>(&action);
  if (bvm != nullptr)
  {
    const SEBagValveMaskConfiguration* config = dynamic_cast<const SEBagValveMaskConfiguration*>(&action);
    if (config != nullptr)
    {
      GetBagValveMaskConfiguration().Copy(*config, m_SubMgr, true);
      m_BagValveMaskConfiguration->Activate();
      if (!m_BagValveMaskConfiguration->IsActive())
        RemoveBagValveMaskConfiguration();
      return true;
    }

    const SEBagValveMaskAutomated* automated = dynamic_cast<const SEBagValveMaskAutomated*>(&action);
    if (automated != nullptr)
    {
      GetBagValveMaskAutomated().Copy(*automated);
      m_BagValveMaskAutomated->Activate();
      if (!m_BagValveMaskAutomated->IsActive())
        RemoveBagValveMaskAutomated();
      else
      {
        RemoveBagValveMaskInstantaneous();
        RemoveBagValveMaskSqueeze();
      }
      return true;
    }

    const SEBagValveMaskInstantaneous* inst = dynamic_cast<const SEBagValveMaskInstantaneous*>(&action);
    if (inst != nullptr)
    {
      GetBagValveMaskInstantaneous().Copy(*inst);
      m_BagValveMaskInstantaneous->Activate();
      if (!m_BagValveMaskInstantaneous->IsActive())
        RemoveBagValveMaskInstantaneous();
      else
      {
        RemoveBagValveMaskAutomated();
        RemoveBagValveMaskSqueeze();
      }
      return true;
    }

    const SEBagValveMaskSqueeze* squeeze = dynamic_cast<const SEBagValveMaskSqueeze*>(&action);
    if (squeeze != nullptr)
    {
      GetBagValveMaskSqueeze().Copy(*squeeze);
      m_BagValveMaskSqueeze->Activate();
      if (!m_BagValveMaskSqueeze->IsActive())
        RemoveBagValveMaskSqueeze();
      else
      {
        RemoveBagValveMaskAutomated();
        RemoveBagValveMaskInstantaneous();
      }
      return true;
    }
  }

  const SEAnesthesiaMachineAction* ama = dynamic_cast<const SEAnesthesiaMachineAction*>(&action);
  if (ama != nullptr)
  {
    const SEAnesthesiaMachineConfiguration* config = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action);
    if (config != nullptr)
    {
      GetAnesthesiaMachineConfiguration().Copy(*config, m_SubMgr, true);
      m_AnesthesiaMachineConfiguration->Activate();
      if (!m_AnesthesiaMachineConfiguration->IsActive())
        RemoveAnesthesiaMachineConfiguration();
      return true;
    }

    const SEAnesthesiaMachineOxygenTankPressureLoss* O2Tank = dynamic_cast<const SEAnesthesiaMachineOxygenTankPressureLoss*>(&action);
    if (O2Tank != nullptr)
    {
      GetAnesthesiaMachineOxygenTankPressureLoss().Copy(*O2Tank, true);
      m_AnesthesiaMachineOxygenTankPressureLoss->Activate();
      if (!m_AnesthesiaMachineOxygenTankPressureLoss->IsActive())
        RemoveAnesthesiaMachineOxygenTankPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineOxygenWallPortPressureLoss* O2Wall = dynamic_cast<const SEAnesthesiaMachineOxygenWallPortPressureLoss*>(&action);
    if (O2Wall != nullptr)
    {
      GetAnesthesiaMachineOxygenWallPortPressureLoss().Copy(*O2Wall, true);
      m_AnesthesiaMachineOxygenWallPortPressureLoss->Activate();
      if (!m_AnesthesiaMachineOxygenWallPortPressureLoss->IsActive())
        RemoveAnesthesiaMachineOxygenWallPortPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineExpiratoryValveLeak* eLeak = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveLeak*>(&action);
    if (eLeak != nullptr)
    {
      GetAnesthesiaMachineExpiratoryValveLeak().Copy(*eLeak, true);
      m_AnesthesiaMachineExpiratoryValveLeak->Activate();
      if (!m_AnesthesiaMachineExpiratoryValveLeak->IsActive())
        RemoveAnesthesiaMachineExpiratoryValveLeak();
      return true;
    }

    const SEAnesthesiaMachineExpiratoryValveObstruction* eOb = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveObstruction*>(&action);
    if (eOb != nullptr)
    {
      GetAnesthesiaMachineExpiratoryValveObstruction().Copy(*eOb, true);
      m_AnesthesiaMachineExpiratoryValveObstruction->Activate();
      if (!m_AnesthesiaMachineExpiratoryValveObstruction->IsActive())
        RemoveAnesthesiaMachineExpiratoryValveObstruction();
      return true;
    }

    const SEAnesthesiaMachineInspiratoryValveLeak* iLeak = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveLeak*>(&action);
    if (iLeak != nullptr)
    {
      GetAnesthesiaMachineInspiratoryValveLeak().Copy(*iLeak, true);
      m_AnesthesiaMachineInspiratoryValveLeak->Activate();
      if (!m_AnesthesiaMachineInspiratoryValveLeak->IsActive())
        RemoveAnesthesiaMachineInspiratoryValveLeak();
      return true;
    }

    const SEAnesthesiaMachineInspiratoryValveObstruction* iOb = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveObstruction*>(&action);
    if (iOb != nullptr)
    {
      GetAnesthesiaMachineInspiratoryValveObstruction().Copy(*iOb, true);
      m_AnesthesiaMachineInspiratoryValveObstruction->Activate();
      if (!m_AnesthesiaMachineInspiratoryValveObstruction->IsActive())
        RemoveAnesthesiaMachineInspiratoryValveObstruction();
      return true;
    }

    const SEAnesthesiaMachineMaskLeak* mask = dynamic_cast<const SEAnesthesiaMachineMaskLeak*>(&action);
    if (mask != nullptr)
    {
      GetAnesthesiaMachineMaskLeak().Copy(*mask, true);
      m_AnesthesiaMachineMaskLeak->Activate();
      if (!m_AnesthesiaMachineMaskLeak->IsActive())
        RemoveAnesthesiaMachineMaskLeak();
      return true;
    }

    const SEAnesthesiaMachineSodaLimeFailure* soda = dynamic_cast<const SEAnesthesiaMachineSodaLimeFailure*>(&action);
    if (soda != nullptr)
    {
      GetAnesthesiaMachineSodaLimeFailure().Copy(*soda, true);
      m_AnesthesiaMachineSodaLimeFailure->Activate();
      if (!m_AnesthesiaMachineSodaLimeFailure->IsActive())
        RemoveAnesthesiaMachineSodaLimeFailure();
      return true;
    }

    const SEAnesthesiaMachineTubeCuffLeak* tube = dynamic_cast<const SEAnesthesiaMachineTubeCuffLeak*>(&action);
    if (tube != nullptr)
    {
      GetAnesthesiaMachineTubeCuffLeak().Copy(*tube, true);
      m_AnesthesiaMachineTubeCuffLeak->Activate();
      if (!m_AnesthesiaMachineTubeCuffLeak->IsActive())
        RemoveAnesthesiaMachineTubeCuffLeak();
      return true;
    }

    const SEAnesthesiaMachineVaporizerFailure* vFail = dynamic_cast<const SEAnesthesiaMachineVaporizerFailure*>(&action);
    if (vFail != nullptr)
    {
      GetAnesthesiaMachineVaporizerFailure().Copy(*vFail, true);
      m_AnesthesiaMachineVaporizerFailure->Activate();
      if (!m_AnesthesiaMachineVaporizerFailure->IsActive())
        RemoveAnesthesiaMachineVaporizerFailure();
      return true;
    }

    const SEAnesthesiaMachineVentilatorPressureLoss* vLoss = dynamic_cast<const SEAnesthesiaMachineVentilatorPressureLoss*>(&action);
    if (vLoss != nullptr)
    {
      GetAnesthesiaMachineVentilatorPressureLoss().Copy(*vLoss, true);
      m_AnesthesiaMachineVentilatorPressureLoss->Activate();
      if (!m_AnesthesiaMachineVentilatorPressureLoss->IsActive())
        RemoveAnesthesiaMachineVentilatorPressureLoss();
      return true;
    }

    const SEAnesthesiaMachineYPieceDisconnect* Y = dynamic_cast<const SEAnesthesiaMachineYPieceDisconnect*>(&action);
    if (Y != nullptr)
    {
      GetAnesthesiaMachineYPieceDisconnect().Copy(*Y, true);
      m_AnesthesiaMachineYPieceDisconnect->Activate();
      if (!m_AnesthesiaMachineYPieceDisconnect->IsActive())
        RemoveAnesthesiaMachineYPieceDisconnect();
      return true;
    }
  }

  const SEInhalerAction* ia = dynamic_cast<const SEInhalerAction*>(&action);
  if (ia != nullptr)
  {
    const SEInhalerConfiguration* config = dynamic_cast<const SEInhalerConfiguration*>(&action);
    if (config != nullptr)
    {
      GetInhalerConfiguration().Copy(*config, m_SubMgr, true);
      m_InhalerConfiguration->Activate();
      if (!m_InhalerConfiguration->IsActive())
        RemoveInhalerConfiguration();
      return true;
    }
  }

  /// \error Unsupported Action
  Error("Unsupported Action");
  return false;
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineConfiguration() const
{
  return m_AnesthesiaMachineConfiguration == nullptr ? false : m_AnesthesiaMachineConfiguration->IsActive();
}
SEAnesthesiaMachineConfiguration& SEEquipmentActionCollection::GetAnesthesiaMachineConfiguration()
{
  if (m_AnesthesiaMachineConfiguration == nullptr)
    m_AnesthesiaMachineConfiguration = new SEAnesthesiaMachineConfiguration();
  return *m_AnesthesiaMachineConfiguration;
}
const SEAnesthesiaMachineConfiguration* SEEquipmentActionCollection::GetAnesthesiaMachineConfiguration() const
{
  return m_AnesthesiaMachineConfiguration;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineConfiguration()
{
  if (m_AnesthesiaMachineConfiguration)
    m_AnesthesiaMachineConfiguration->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineOxygenTankPressureLoss() const
{
  return m_AnesthesiaMachineOxygenTankPressureLoss == nullptr ? false : m_AnesthesiaMachineOxygenTankPressureLoss->IsActive();
}
SEAnesthesiaMachineOxygenTankPressureLoss& SEEquipmentActionCollection::GetAnesthesiaMachineOxygenTankPressureLoss()
{
  if (m_AnesthesiaMachineOxygenTankPressureLoss == nullptr)
    m_AnesthesiaMachineOxygenTankPressureLoss = new SEAnesthesiaMachineOxygenTankPressureLoss();
  return *m_AnesthesiaMachineOxygenTankPressureLoss;
}
const SEAnesthesiaMachineOxygenTankPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenTankPressureLoss() const
{
  return m_AnesthesiaMachineOxygenTankPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineOxygenTankPressureLoss()
{
  if (m_AnesthesiaMachineOxygenTankPressureLoss)
    m_AnesthesiaMachineOxygenTankPressureLoss->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineOxygenWallPortPressureLoss() const
{
  return m_AnesthesiaMachineOxygenWallPortPressureLoss == nullptr ? false : m_AnesthesiaMachineOxygenWallPortPressureLoss->IsActive();
}
SEAnesthesiaMachineOxygenWallPortPressureLoss& SEEquipmentActionCollection::GetAnesthesiaMachineOxygenWallPortPressureLoss()
{
  if (m_AnesthesiaMachineOxygenWallPortPressureLoss == nullptr)
    m_AnesthesiaMachineOxygenWallPortPressureLoss = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
  return *m_AnesthesiaMachineOxygenWallPortPressureLoss;
}
const SEAnesthesiaMachineOxygenWallPortPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineOxygenWallPortPressureLoss() const
{
  return m_AnesthesiaMachineOxygenWallPortPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineOxygenWallPortPressureLoss()
{
  if (m_AnesthesiaMachineOxygenWallPortPressureLoss)
    m_AnesthesiaMachineOxygenWallPortPressureLoss->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineExpiratoryValveLeak() const
{
  return m_AnesthesiaMachineExpiratoryValveLeak == nullptr ? false : m_AnesthesiaMachineExpiratoryValveLeak->IsActive();
}
SEAnesthesiaMachineExpiratoryValveLeak& SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveLeak()
{
  if (m_AnesthesiaMachineExpiratoryValveLeak == nullptr)
    m_AnesthesiaMachineExpiratoryValveLeak = new SEAnesthesiaMachineExpiratoryValveLeak();
  return *m_AnesthesiaMachineExpiratoryValveLeak;
}
const SEAnesthesiaMachineExpiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveLeak() const
{
  return m_AnesthesiaMachineExpiratoryValveLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineExpiratoryValveLeak()
{
  if (m_AnesthesiaMachineExpiratoryValveLeak)
    m_AnesthesiaMachineExpiratoryValveLeak->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineExpiratoryValveObstruction() const
{
  return m_AnesthesiaMachineExpiratoryValveObstruction == nullptr ? false : m_AnesthesiaMachineExpiratoryValveObstruction->IsActive();
}
SEAnesthesiaMachineExpiratoryValveObstruction& SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveObstruction()
{
  if (m_AnesthesiaMachineExpiratoryValveObstruction == nullptr)
    m_AnesthesiaMachineExpiratoryValveObstruction = new SEAnesthesiaMachineExpiratoryValveObstruction();
  return *m_AnesthesiaMachineExpiratoryValveObstruction;
}
const SEAnesthesiaMachineExpiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineExpiratoryValveObstruction() const
{
  return m_AnesthesiaMachineExpiratoryValveObstruction;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineExpiratoryValveObstruction()
{
  if (m_AnesthesiaMachineExpiratoryValveObstruction)
    m_AnesthesiaMachineExpiratoryValveObstruction->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineInspiratoryValveLeak() const
{
  return m_AnesthesiaMachineInspiratoryValveLeak == nullptr ? false : m_AnesthesiaMachineInspiratoryValveLeak->IsActive();
}
SEAnesthesiaMachineInspiratoryValveLeak& SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveLeak()
{
  if (m_AnesthesiaMachineInspiratoryValveLeak == nullptr)
    m_AnesthesiaMachineInspiratoryValveLeak = new SEAnesthesiaMachineInspiratoryValveLeak();
  return *m_AnesthesiaMachineInspiratoryValveLeak;
}
const SEAnesthesiaMachineInspiratoryValveLeak* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveLeak() const
{
  return m_AnesthesiaMachineInspiratoryValveLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineInspiratoryValveLeak()
{
  if (m_AnesthesiaMachineInspiratoryValveLeak)
    m_AnesthesiaMachineInspiratoryValveLeak->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineInspiratoryValveObstruction() const
{
  return m_AnesthesiaMachineInspiratoryValveObstruction == nullptr ? false : m_AnesthesiaMachineInspiratoryValveObstruction->IsActive();
}
SEAnesthesiaMachineInspiratoryValveObstruction& SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveObstruction()
{
  if (m_AnesthesiaMachineInspiratoryValveObstruction == nullptr)
    m_AnesthesiaMachineInspiratoryValveObstruction = new SEAnesthesiaMachineInspiratoryValveObstruction();
  return *m_AnesthesiaMachineInspiratoryValveObstruction;
}
const SEAnesthesiaMachineInspiratoryValveObstruction* SEEquipmentActionCollection::GetAnesthesiaMachineInspiratoryValveObstruction() const
{
  return m_AnesthesiaMachineInspiratoryValveObstruction;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineInspiratoryValveObstruction()
{
  if (m_AnesthesiaMachineInspiratoryValveObstruction)
    m_AnesthesiaMachineInspiratoryValveObstruction->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineMaskLeak() const
{
  return m_AnesthesiaMachineMaskLeak == nullptr ? false : m_AnesthesiaMachineMaskLeak->IsActive();
}
SEAnesthesiaMachineMaskLeak& SEEquipmentActionCollection::GetAnesthesiaMachineMaskLeak()
{
  if (m_AnesthesiaMachineMaskLeak == nullptr)
    m_AnesthesiaMachineMaskLeak = new SEAnesthesiaMachineMaskLeak();
  return *m_AnesthesiaMachineMaskLeak;
}
const SEAnesthesiaMachineMaskLeak* SEEquipmentActionCollection::GetAnesthesiaMachineMaskLeak() const
{
  return m_AnesthesiaMachineMaskLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineMaskLeak()
{
  if (m_AnesthesiaMachineMaskLeak)
    m_AnesthesiaMachineMaskLeak->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineSodaLimeFailure() const
{
  return m_AnesthesiaMachineSodaLimeFailure == nullptr ? false : m_AnesthesiaMachineSodaLimeFailure->IsActive();
}
SEAnesthesiaMachineSodaLimeFailure& SEEquipmentActionCollection::GetAnesthesiaMachineSodaLimeFailure()
{
  if (m_AnesthesiaMachineSodaLimeFailure == nullptr)
    m_AnesthesiaMachineSodaLimeFailure = new SEAnesthesiaMachineSodaLimeFailure();
  return *m_AnesthesiaMachineSodaLimeFailure;
}
const SEAnesthesiaMachineSodaLimeFailure* SEEquipmentActionCollection::GetAnesthesiaMachineSodaLimeFailure() const
{
  return m_AnesthesiaMachineSodaLimeFailure;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineSodaLimeFailure()
{
  if (m_AnesthesiaMachineSodaLimeFailure)
    m_AnesthesiaMachineSodaLimeFailure->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineTubeCuffLeak() const
{
  return m_AnesthesiaMachineTubeCuffLeak == nullptr ? false : m_AnesthesiaMachineTubeCuffLeak->IsActive();
}
SEAnesthesiaMachineTubeCuffLeak& SEEquipmentActionCollection::GetAnesthesiaMachineTubeCuffLeak()
{
  if (m_AnesthesiaMachineTubeCuffLeak == nullptr)
    m_AnesthesiaMachineTubeCuffLeak = new SEAnesthesiaMachineTubeCuffLeak();
  return *m_AnesthesiaMachineTubeCuffLeak;
}
const SEAnesthesiaMachineTubeCuffLeak* SEEquipmentActionCollection::GetAnesthesiaMachineTubeCuffLeak() const
{
  return m_AnesthesiaMachineTubeCuffLeak;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineTubeCuffLeak()
{
  if (m_AnesthesiaMachineTubeCuffLeak)
    m_AnesthesiaMachineTubeCuffLeak->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineVaporizerFailure() const
{
  return m_AnesthesiaMachineVaporizerFailure == nullptr ? false : m_AnesthesiaMachineVaporizerFailure->IsActive();
}
SEAnesthesiaMachineVaporizerFailure& SEEquipmentActionCollection::GetAnesthesiaMachineVaporizerFailure()
{
  if (m_AnesthesiaMachineVaporizerFailure == nullptr)
    m_AnesthesiaMachineVaporizerFailure = new SEAnesthesiaMachineVaporizerFailure();
  return *m_AnesthesiaMachineVaporizerFailure;
}
const SEAnesthesiaMachineVaporizerFailure* SEEquipmentActionCollection::GetAnesthesiaMachineVaporizerFailure() const
{
  return m_AnesthesiaMachineVaporizerFailure;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineVaporizerFailure()
{
  if (m_AnesthesiaMachineVaporizerFailure)
    m_AnesthesiaMachineVaporizerFailure->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineVentilatorPressureLoss() const
{
  return m_AnesthesiaMachineVentilatorPressureLoss == nullptr ? false : m_AnesthesiaMachineVentilatorPressureLoss->IsActive();
}
SEAnesthesiaMachineVentilatorPressureLoss& SEEquipmentActionCollection::GetAnesthesiaMachineVentilatorPressureLoss()
{
  if (m_AnesthesiaMachineVentilatorPressureLoss == nullptr)
    m_AnesthesiaMachineVentilatorPressureLoss = new SEAnesthesiaMachineVentilatorPressureLoss();
  return *m_AnesthesiaMachineVentilatorPressureLoss;
}
const SEAnesthesiaMachineVentilatorPressureLoss* SEEquipmentActionCollection::GetAnesthesiaMachineVentilatorPressureLoss() const
{
  return m_AnesthesiaMachineVentilatorPressureLoss;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineVentilatorPressureLoss()
{
  if (m_AnesthesiaMachineVentilatorPressureLoss)
    m_AnesthesiaMachineVentilatorPressureLoss->Deactivate();
}

bool SEEquipmentActionCollection::HasAnesthesiaMachineYPieceDisconnect() const
{
  return m_AnesthesiaMachineYPieceDisconnect == nullptr ? false : m_AnesthesiaMachineYPieceDisconnect->IsActive();
}
SEAnesthesiaMachineYPieceDisconnect& SEEquipmentActionCollection::GetAnesthesiaMachineYPieceDisconnect()
{
  if (m_AnesthesiaMachineYPieceDisconnect == nullptr)
    m_AnesthesiaMachineYPieceDisconnect = new SEAnesthesiaMachineYPieceDisconnect();
  return *m_AnesthesiaMachineYPieceDisconnect;
}
const SEAnesthesiaMachineYPieceDisconnect* SEEquipmentActionCollection::GetAnesthesiaMachineYPieceDisconnect() const
{
  return m_AnesthesiaMachineYPieceDisconnect;
}
void SEEquipmentActionCollection::RemoveAnesthesiaMachineYPieceDisconnect()
{
  if (m_AnesthesiaMachineYPieceDisconnect)
    m_AnesthesiaMachineYPieceDisconnect->Deactivate();
}

bool SEEquipmentActionCollection::HasBagValveMaskConfiguration() const
{
  return m_BagValveMaskConfiguration == nullptr ? false : m_BagValveMaskConfiguration->IsActive();
}
SEBagValveMaskConfiguration& SEEquipmentActionCollection::GetBagValveMaskConfiguration()
{
  if (m_BagValveMaskConfiguration == nullptr)
    m_BagValveMaskConfiguration = new SEBagValveMaskConfiguration();
  return *m_BagValveMaskConfiguration;
}
const SEBagValveMaskConfiguration* SEEquipmentActionCollection::GetBagValveMaskConfiguration() const
{
  return m_BagValveMaskConfiguration;
}
void SEEquipmentActionCollection::RemoveBagValveMaskConfiguration()
{
  if (m_BagValveMaskConfiguration)
    m_BagValveMaskConfiguration->Deactivate();
}

bool SEEquipmentActionCollection::HasBagValveMaskAutomated() const
{
  return m_BagValveMaskAutomated == nullptr ? false : m_BagValveMaskAutomated->IsActive();
}
SEBagValveMaskAutomated& SEEquipmentActionCollection::GetBagValveMaskAutomated()
{
  if (m_BagValveMaskAutomated == nullptr)
    m_BagValveMaskAutomated = new SEBagValveMaskAutomated();
  return *m_BagValveMaskAutomated;
}
const SEBagValveMaskAutomated* SEEquipmentActionCollection::GetBagValveMaskAutomated() const
{
  return m_BagValveMaskAutomated;
}
void SEEquipmentActionCollection::RemoveBagValveMaskAutomated()
{
  if (m_BagValveMaskAutomated)
    m_BagValveMaskAutomated->Deactivate();
}

bool SEEquipmentActionCollection::HasBagValveMaskInstantaneous() const
{
  return m_BagValveMaskInstantaneous == nullptr ? false : m_BagValveMaskInstantaneous->IsActive();
}
SEBagValveMaskInstantaneous& SEEquipmentActionCollection::GetBagValveMaskInstantaneous()
{
  if (m_BagValveMaskInstantaneous == nullptr)
    m_BagValveMaskInstantaneous = new SEBagValveMaskInstantaneous();
  return *m_BagValveMaskInstantaneous;
}
const SEBagValveMaskInstantaneous* SEEquipmentActionCollection::GetBagValveMaskInstantaneous() const
{
  return m_BagValveMaskInstantaneous;
}
void SEEquipmentActionCollection::RemoveBagValveMaskInstantaneous()
{
  if (m_BagValveMaskInstantaneous)
    m_BagValveMaskInstantaneous->Deactivate();
}

bool SEEquipmentActionCollection::HasBagValveMaskSqueeze() const
{
  return m_BagValveMaskSqueeze == nullptr ? false : m_BagValveMaskSqueeze->IsActive();
}
SEBagValveMaskSqueeze& SEEquipmentActionCollection::GetBagValveMaskSqueeze()
{
  if (m_BagValveMaskSqueeze == nullptr)
    m_BagValveMaskSqueeze = new SEBagValveMaskSqueeze();
  return *m_BagValveMaskSqueeze;
}
const SEBagValveMaskSqueeze* SEEquipmentActionCollection::GetBagValveMaskSqueeze() const
{
  return m_BagValveMaskSqueeze;
}
void SEEquipmentActionCollection::RemoveBagValveMaskSqueeze()
{
  if (m_BagValveMaskSqueeze)
    m_BagValveMaskSqueeze->Deactivate();
}

bool SEEquipmentActionCollection::HasInhalerConfiguration() const
{
  return m_InhalerConfiguration == nullptr ? false : m_InhalerConfiguration->IsActive();
}
SEInhalerConfiguration& SEEquipmentActionCollection::GetInhalerConfiguration()
{
  if (m_InhalerConfiguration == nullptr)
    m_InhalerConfiguration = new SEInhalerConfiguration();
  return *m_InhalerConfiguration;
}
const SEInhalerConfiguration* SEEquipmentActionCollection::GetInhalerConfiguration() const
{
  return m_InhalerConfiguration;
}
void SEEquipmentActionCollection::RemoveInhalerConfiguration()
{
  if (m_InhalerConfiguration)
    m_InhalerConfiguration->Deactivate();
}

bool SEEquipmentActionCollection::HasMechanicalVentilatorConfiguration() const
{
  return m_MechanicalVentilatorConfiguration == nullptr ? false : m_MechanicalVentilatorConfiguration->IsActive();
}
SEMechanicalVentilatorConfiguration& SEEquipmentActionCollection::GetMechanicalVentilatorConfiguration()
{
  if (m_MechanicalVentilatorConfiguration == nullptr)
    m_MechanicalVentilatorConfiguration = new SEMechanicalVentilatorConfiguration();
  return *m_MechanicalVentilatorConfiguration;
}
const SEMechanicalVentilatorConfiguration* SEEquipmentActionCollection::GetMechanicalVentilatorConfiguration() const
{
  return m_MechanicalVentilatorConfiguration;
}
void SEEquipmentActionCollection::RemoveMechanicalVentilatorConfiguration()
{
  if (m_MechanicalVentilatorConfiguration)
    m_MechanicalVentilatorConfiguration->Deactivate();
}

bool SEEquipmentActionCollection::HasMechanicalVentilatorHold() const
{
  return m_MechanicalVentilatorHold == nullptr ? false : m_MechanicalVentilatorHold->IsActive();
}
SEMechanicalVentilatorHold& SEEquipmentActionCollection::GetMechanicalVentilatorHold()
{
  if (m_MechanicalVentilatorHold == nullptr)
    m_MechanicalVentilatorHold = new SEMechanicalVentilatorHold();
  return *m_MechanicalVentilatorHold;
}
const SEMechanicalVentilatorHold* SEEquipmentActionCollection::GetMechanicalVentilatorHold() const
{
  return m_MechanicalVentilatorHold;
}
void SEEquipmentActionCollection::RemoveMechanicalVentilatorHold()
{
  if (m_MechanicalVentilatorHold)
    m_MechanicalVentilatorHold->Deactivate();
}

void SEEquipmentActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasAnesthesiaMachineConfiguration())
    actions.push_back(GetAnesthesiaMachineConfiguration());
  if (HasAnesthesiaMachineOxygenTankPressureLoss())
    actions.push_back(GetAnesthesiaMachineOxygenTankPressureLoss());
  if (HasAnesthesiaMachineOxygenWallPortPressureLoss())
    actions.push_back(GetAnesthesiaMachineOxygenWallPortPressureLoss());
  if (HasAnesthesiaMachineExpiratoryValveLeak())
    actions.push_back(GetAnesthesiaMachineExpiratoryValveLeak());
  if (HasAnesthesiaMachineExpiratoryValveObstruction())
    actions.push_back(GetAnesthesiaMachineExpiratoryValveObstruction());
  if (HasAnesthesiaMachineInspiratoryValveLeak())
    actions.push_back(GetAnesthesiaMachineInspiratoryValveLeak());
  if (HasAnesthesiaMachineInspiratoryValveObstruction())
    actions.push_back(GetAnesthesiaMachineInspiratoryValveObstruction());
  if (HasAnesthesiaMachineMaskLeak())
    actions.push_back(GetAnesthesiaMachineMaskLeak());
  if (HasAnesthesiaMachineSodaLimeFailure())
    actions.push_back(GetAnesthesiaMachineSodaLimeFailure());
  if (HasAnesthesiaMachineTubeCuffLeak())
    actions.push_back(GetAnesthesiaMachineTubeCuffLeak());
  if (HasAnesthesiaMachineVaporizerFailure())
    actions.push_back(GetAnesthesiaMachineVaporizerFailure());
  if (HasAnesthesiaMachineVentilatorPressureLoss())
    actions.push_back(GetAnesthesiaMachineVentilatorPressureLoss());
  if (HasAnesthesiaMachineYPieceDisconnect())
    actions.push_back(GetAnesthesiaMachineYPieceDisconnect());

  if (HasBagValveMaskConfiguration())
    actions.push_back(GetBagValveMaskConfiguration());
  if (HasBagValveMaskAutomated())
    actions.push_back(GetBagValveMaskAutomated());
  if (HasBagValveMaskInstantaneous())
    actions.push_back(GetBagValveMaskInstantaneous());
  if (HasBagValveMaskSqueeze())
    actions.push_back(GetBagValveMaskSqueeze());

  if(HasInhalerConfiguration())
    actions.push_back(GetInhalerConfiguration());

  if(HasMechanicalVentilatorConfiguration())
    actions.push_back(GetMechanicalVentilatorConfiguration());
  if (HasMechanicalVentilatorHold())
    actions.push_back(GetMechanicalVentilatorHold());
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
const SEScalar* SEEquipmentActionCollection::GetScalar(const std::string& actionName, const std::string& cmptName, const std::string& substance, const std::string& property)
{
  if (actionName == "AnesthesiaMachineConfiguration")
    return GetAnesthesiaMachineConfiguration().GetScalar(property);
  if (actionName == "AnesthesiaMachineOxygenTankPressureLoss")
    return GetAnesthesiaMachineOxygenTankPressureLoss().GetScalar(property);
  if (actionName == "AnesthesiaMachineOxygenWallPortPressureLoss")
    return GetAnesthesiaMachineOxygenWallPortPressureLoss().GetScalar(property);
  if (actionName == "AnesthesiaMachineExpiratoryValveLeak")
    return GetAnesthesiaMachineExpiratoryValveLeak().GetScalar(property);
  if (actionName == "AnesthesiaMachineExpiratoryValveObstruction")
    return GetAnesthesiaMachineExpiratoryValveObstruction().GetScalar(property);
  if (actionName == "AnesthesiaMachineInspiratoryValveLeak")
    return GetAnesthesiaMachineInspiratoryValveLeak().GetScalar(property);
  if (actionName == "AnesthesiaMachineInspiratoryValveObstruction")
    return GetAnesthesiaMachineInspiratoryValveObstruction().GetScalar(property);
  if (actionName == "AnesthesiaMachineMaskLeak")
    return GetAnesthesiaMachineMaskLeak().GetScalar(property);
  if (actionName == "AnesthesiaMachineSodaLimeFailure")
    return GetAnesthesiaMachineSodaLimeFailure().GetScalar(property);
  if (actionName == "AnesthesiaMachineTubeCuffLeak")
    return GetAnesthesiaMachineTubeCuffLeak().GetScalar(property);
  if (actionName == "AnesthesiaMachineVaporizerFailure")
    return GetAnesthesiaMachineVaporizerFailure().GetScalar(property);
  if (actionName == "AnesthesiaMachineVentilatorPressureLoss")
    return GetAnesthesiaMachineVentilatorPressureLoss().GetScalar(property);
  if (actionName == "AnesthesiaMachineYPieceDisconnect")
    return GetAnesthesiaMachineYPieceDisconnect().GetScalar(property);

  if (actionName == "BagValveMaskConfiguration")
    return GetBagValveMaskConfiguration().GetScalar(property);
  if (actionName == "BagValveMaskAutomated")
    return GetBagValveMaskAutomated().GetScalar(property);
  if (actionName == "BagValveMaskInstantaneous")
    return GetBagValveMaskInstantaneous().GetScalar(property);
  if (actionName == "BagValveMaskSqueeze")
    return GetBagValveMaskSqueeze().GetScalar(property);

  if (actionName == "InhalerConfiguration")
    return GetInhalerConfiguration().GetScalar(property);

  if (actionName == "MechanicalVentilatorConfiguration")
    return GetMechanicalVentilatorConfiguration().GetScalar(property);
  if (actionName == "MechanicalVentilatorHold")
    return GetMechanicalVentilatorHold().GetScalar(property);

  return nullptr;
}
DISABLE_WARNING_POP
