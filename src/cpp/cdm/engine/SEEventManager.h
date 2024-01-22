/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalarTime.h"

// Keep enums in sync with appropriate schema/cdm/Event.proto file !!
enum class eEvent
{
  Antidiuresis                    = 0,
  Bradycardia                     = 1,
  Bradypnea                       = 2,
  BrainOxygenDeficit              = 3,
  CardiacArrest                   = 4,
  CardiogenicShock                = 5,
  CardiovascularCollapse          = 6,
  CriticalBrainOxygenDeficit      = 7,
  Dehydration                     = 8,
  Diuresis                        = 9,
  Fasciculation                   = 10,
  Fatigue                         = 11,
  FunctionalIncontinence          = 12,
  Hypercapnia                     = 13,
  Hyperglycemia                   = 14,
  Hyperthermia                    = 15,
  Hypoglycemia                    = 16,
  Hypothermia                     = 17,
  Hypoxia                         = 18,
  HypovolemicShock                = 19,
  IntracranialHypertension        = 20,
  IntracranialHypotension         = 21,
  IrreversibleState               = 22,
  Ketoacidosis                    = 23,
  LacticAcidosis                  = 24,
  MassiveHemothorax               = 25,
  MaximumPulmonaryVentilationRate = 26,
  MediumHemothorax                = 27,
  MetabolicAcidosis               = 28,
  MetabolicAlkalosis              = 29,
  MinimalHemothorax               = 30,
  ModerateHyperoxemia             = 31,
  ModerateHypocapnia              = 32,
  MyocardiumOxygenDeficit         = 33,
  Natriuresis                     = 34,
  NutritionDepleted               = 35,
  RenalHypoperfusion              = 36,
  RespiratoryAcidosis             = 37,
  RespiratoryAlkalosis            = 38,
  SevereHyperoxemia               = 39,
  SevereHypocapnia                = 40,
  Stabilization                   = 41,
  StartOfCardiacCycle             = 42,
  StartOfExhale                   = 43,
  StartOfInhale                   = 44,
  Tachycardia                     = 45,
  Tachypnea                       = 46,

  // Equipment
  AnesthesiaMachineOxygenBottleOneExhausted = 1000,
  AnesthesiaMachineOxygenBottleTwoExhausted = 1001,
  AnesthesiaMachineReliefValveActive = 1002,
  MechanicalVentilatorReliefValveActive = 1003,
  SupplementalOxygenBottleExhausted = 1004,
  NonRebreatherMaskOxygenBagEmpty = 1005
};
extern CDM_DECL const std::string& eEvent_Name(eEvent m);

class CDM_DECL SEEventHandler
{
public:
  SEEventHandler() {};
  virtual ~SEEventHandler() {};

  virtual void HandleEvent(eEvent e, bool active, const SEScalarTime* simTime = nullptr) = 0;
};

class CDM_DECL SEActiveEvent
{
public:
  SEActiveEvent(eEvent e, const SEScalarTime& duration);
  SEActiveEvent(eEvent e, double duration, const TimeUnit& unit);

  static bool SerializeToString(std::vector<const SEActiveEvent*>& active, std::string& output, eSerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEActiveEvent*>& active, eSerializationFormat m, Logger* logger);

  eEvent GetEvent() const { return m_Event; }
  const SEScalarTime& GetDuration() const { return m_Duration; }

protected:
  eEvent       m_Event;
  SEScalarTime m_Duration;
};

class CDM_DECL SEEventChange
{
public:
  SEEventChange(eEvent e, bool active, const SEScalarTime* simTime = nullptr);

  static bool SerializeToString(std::vector<const SEEventChange*>& changes, std::string& output, eSerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEEventChange*>& changes, eSerializationFormat m, Logger* logger);

  eEvent GetEvent() const { return m_Event; }
  bool   GetActive() const { return m_Active; }
  const SEScalarTime& GetSimTime() const { return m_SimTime; }

protected:
  bool         m_Active;
  eEvent       m_Event;
  SEScalarTime m_SimTime;
};

class CDM_DECL SEEventManager : public Loggable
{
public:
  SEEventManager(Logger* logger);
  virtual ~SEEventManager();

  void Clear();
  // Essentially a load, this will set the state to active and set its duration
  virtual void OverrideActiveState(eEvent state, const SEScalarTime& duration);

  virtual const std::map<eEvent, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(eEvent e, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(eEvent e) const;
  virtual double GetEventDuration(eEvent e, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  virtual bool GetActiveEvents(std::vector<const SEActiveEvent*>& active) const;
  /** @name ForwardEvents
   *  @brief - Set a callback class to invoke when any event changes
   *  @details - Note that the handler callback can and will be called in the middle of a time step
   *             So system and compartment objects may not be completely up to date when called.
   *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
   *             data is up to date at the time the callback is invoked
   */
  virtual void ForwardEvents(SEEventHandler* handler) const;
  virtual SEEventHandler* GetEventHandler() { return m_EventHandler; }

protected:

  std::stringstream        m_ss;
  mutable SEEventHandler*  m_EventHandler;
  std::map<eEvent, bool>   m_EventState;
  std::map<eEvent, double> m_EventDuration_s;
};
