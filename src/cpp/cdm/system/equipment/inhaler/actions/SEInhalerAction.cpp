/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/inhaler/actions/SEInhalerAction.h"
#include "substance/SESubstanceManager.h"

SEInhalerAction::SEInhalerAction() : SEEquipmentAction()
{

}

SEInhalerAction::~SEInhalerAction()
{
  Clear();
}

void SEInhalerAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEInhalerAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
