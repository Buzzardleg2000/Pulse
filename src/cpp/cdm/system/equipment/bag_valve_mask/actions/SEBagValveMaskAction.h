/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/SEEquipmentAction.h"

class CDM_DECL SEBagValveMaskAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskAction(Logger* logger);
  virtual ~SEBagValveMaskAction();

  static constexpr char const* ActionType = "Bag Valve Mask";
  virtual std::string GetActionType() const { return ActionType; }

  virtual void Clear();

  virtual bool IsValid() const;
};  