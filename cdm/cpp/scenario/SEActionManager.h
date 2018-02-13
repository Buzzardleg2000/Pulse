/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEPatientActionCollection;
class SEAnesthesiaMachineActionCollection;
class SEEnvironmentActionCollection;
class SEInhalerActionCollection;
class SESubstanceManager;
CDM_BIND_DECL(ActionListData)

class CDM_DECL SEActionManager : public Loggable
{
public:

  SEActionManager(SESubstanceManager&);
  ~SEActionManager();

  void Clear();

  static void Load(const cdm::ActionListData& src, SEActionManager& dst);
  static cdm::ActionListData* Unload(const SEActionManager& src);
protected:
  static void Serialize(const cdm::ActionListData& src, SEActionManager& dst);
  static void Serialize(const SEActionManager& src, cdm::ActionListData& dst);

public:

  bool ProcessAction(const SEAction& action);// Will make a copy

  SEPatientActionCollection&           GetPatientActions()           { return m_PatientActions; }
  SEEnvironmentActionCollection&       GetEnvironmentActions()       { return m_EnvironmentActions; }
  SEAnesthesiaMachineActionCollection& GetAnesthesiaMachineActions() { return m_AnesthesiaMachineActions; }
  SEInhalerActionCollection&           GetInhalerActions()           { return m_InhalerActions; }

  // This is here in case you want to take all the actions from an engine and write them out so you can reproduce the same engine state later
  const cdm::ActionListData& GetActionList() { return m_ProcessedActions; }// I don't really have anything that does that yet...

protected:

  SESubstanceManager&                 m_Substances;
  SEPatientActionCollection           m_PatientActions;
  SEEnvironmentActionCollection       m_EnvironmentActions;
  SEAnesthesiaMachineActionCollection m_AnesthesiaMachineActions;
  SEInhalerActionCollection           m_InhalerActions;

  cdm::ActionListData                 m_ProcessedActions;
  
  std::stringstream m_ss;
};