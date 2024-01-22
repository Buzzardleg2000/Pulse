/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/blackbox/SEBlackBox.h"
#include "cdm/circuit/electrical/SEElectricalCircuitPath.h"
#include "cdm/compartment/electrical/SEElectricalCompartment.h"
#include "cdm/compartment/electrical/SEElectricalCompartmentLink.h"

class CDM_DECL SEElectricalBlackBox : public SEBlackBox<ELECTRICAL_BLACK_BOX>
{
  friend class SEBlackBoxManager;
  friend class CommonDataModelTest;
protected:
  SEElectricalBlackBox(const std::string& name, Logger* logger);
public:
  virtual ~SEElectricalBlackBox();

protected:
  // Graph Elements
  virtual bool HasCompartment() const { return m_Compartment != nullptr; }
  virtual SEElectricalCompartment* GetCompartment() const { return m_Compartment; }
  virtual void SetCompartment(SEElectricalCompartment* c) { m_Compartment = c; }

  virtual bool HasSourceCompartment() const { return m_srcCompartment != nullptr; }
  virtual SEElectricalCompartment* GetSourceCompartment() const { return m_srcCompartment; }
  virtual void SetSourceCompartment(SEElectricalCompartment* c) { m_srcCompartment = c; }

  virtual bool HasTargetCompartment() const { return m_tgtCompartment != nullptr; }
  virtual SEElectricalCompartment* GetTargetCompartment() const { return m_tgtCompartment; }
  virtual void SetTargetCompartment(SEElectricalCompartment* c) { m_tgtCompartment = c; }

  virtual bool HasSourceLink() const { return m_srcLink != nullptr; }
  virtual SEElectricalCompartmentLink* GetSourceLink() const { return m_srcLink; }
  virtual void SetSourceLink(SEElectricalCompartmentLink* l) { m_srcLink = l; }

  virtual bool HasTargetLink() const { return m_tgtLink != nullptr; }
  virtual SEElectricalCompartmentLink* GetTargetLink() const { return m_tgtLink; }
  virtual void SetTargetLink(SEElectricalCompartmentLink* l) { m_tgtLink = l; }

protected:
  // Graph Elements
  SEElectricalCompartment*     m_Compartment = nullptr;
  SEElectricalCompartment*     m_srcCompartment = nullptr;
  SEElectricalCompartment*     m_tgtCompartment = nullptr;
  SEElectricalCompartmentLink* m_srcLink = nullptr;
  SEElectricalCompartmentLink* m_tgtLink = nullptr;
};
