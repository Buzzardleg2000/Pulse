/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/blackbox/SEBlackBox.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"
#include "cdm/compartment/thermal/SEThermalCompartment.h"
#include "cdm/compartment/thermal/SEThermalCompartmentLink.h"

class CDM_DECL SEThermalBlackBox : public SEBlackBox<THERMAL_BLACK_BOX>
{
  friend class SEBlackBoxManager;
  friend class CommonDataModelTest;
protected:
  SEThermalBlackBox(const std::string& name, Logger* logger);
public:
  virtual ~SEThermalBlackBox();

protected:
  // Graph Elements
  virtual bool HasCompartment() const { return m_Compartment != nullptr; }
  virtual SEThermalCompartment* GetCompartment() const { return m_Compartment; }
  virtual void SetCompartment(SEThermalCompartment* c) { m_Compartment = c; }

  virtual bool HasSourceCompartment() const { return m_srcCompartment != nullptr; }
  virtual SEThermalCompartment* GetSourceCompartment() const { return m_srcCompartment; }
  virtual void SetSourceCompartment(SEThermalCompartment* c) { m_srcCompartment = c; }

  virtual bool HasTargetCompartment() const { return m_tgtCompartment != nullptr; }
  virtual SEThermalCompartment* GetTargetCompartment() const { return m_tgtCompartment; }
  virtual void SetTargetCompartment(SEThermalCompartment* c) { m_tgtCompartment = c; }

  virtual bool HasSourceLink() const { return m_srcLink != nullptr; }
  virtual SEThermalCompartmentLink* GetSourceLink() const { return m_srcLink; }
  virtual void SetSourceLink(SEThermalCompartmentLink* l) { m_srcLink = l; }

  virtual bool HasTargetLink() const { return m_tgtLink != nullptr; }
  virtual SEThermalCompartmentLink* GetTargetLink() const { return m_tgtLink; }
  virtual void SetTargetLink(SEThermalCompartmentLink* l) { m_tgtLink = l; }

protected:
  // Graph Elements
  SEThermalCompartment*     m_Compartment = nullptr;
  SEThermalCompartment*     m_srcCompartment = nullptr;
  SEThermalCompartment*     m_tgtCompartment = nullptr;
  SEThermalCompartmentLink* m_srcLink = nullptr;
  SEThermalCompartmentLink* m_tgtLink = nullptr;
};
