/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eEvent : int
  {
    Antidiuresis = 0,
    Bradycardia = 1,
    Bradypnea = 2,
    BrainOxygenDeficit = 3,
    CardiacArrest = 4,
    CardiogenicShock = 5,
    CardiovascularCollapse = 6,
    CriticalBrainOxygenDeficit = 7,
    Diuresis = 8,
    Fasciculation = 9,
    Fatigue = 10,
    FunctionalIncontinence = 11,
    Hypercapnia = 12,
    Hyperglycemia = 13,
    Hyperthermia = 14,
    Hypoglycemia = 15,
    Hypothermia = 16,
    Hypoxia = 17,
    HypovolemicShock = 18,
    IntracranialHypertension = 19,
    IntracranialHypotension = 20,
    IrreversibleState = 21,
    Ketoacidosis = 22,
    LacticAcidosis = 23,
    MassiveHemothorax = 24,
    MaximumPulmonaryVentilationRate = 25,
    MediumHemothorax = 26,
    MetabolicAcidosis = 27,
    MetabolicAlkalosis = 28,
    MildDehydration = 29,
    MinimalHemothorax = 30,
    ModerateDehydration = 31,
    ModerateHyperoxemia = 32,
    ModerateHypocapnia = 33,
    MyocardiumOxygenDeficit = 34,
    Natriuresis = 35,
    NutritionDepleted = 36,
    RenalHypoperfusion = 37,
    RespiratoryAcidosis = 38,
    RespiratoryAlkalosis = 39,
    SevereDehydration = 40,
    SevereHyperoxemia = 41,
    SevereHypocapnia = 42,
    Stabilization = 43,
    StartOfCardiacCycle = 44,
    StartOfExhale = 45,
    StartOfInhale = 46,
    Tachycardia = 47,
    Tachypnea = 48,

    // Equipment
    AnesthesiaMachineOxygenBottleOneExhausted = 1000,
    AnesthesiaMachineOxygenBottleTwoExhausted = 1001,
    AnesthesiaMachineReliefValveActive = 1002,
    MechanicalVentilatorReliefValveActive = 1003,
    SupplementalOxygenBottleExhausted = 1004,
    NonRebreatherMaskOxygenBagEmpty = 1005
  }

  public interface IEventHandler
  {
    void HandleEvent(SEEventChange change);
  }

  public class SEActiveEvent
  {
    public eEvent Event { get; }
    public SEScalarTime Duration { get; }

    public SEActiveEvent(eEvent e, SEScalarTime duration)
    {
      Event = e;
      Duration = new SEScalarTime(duration);
    }

    public new string ToString()
    {
      return Event + " has been active for " + Duration.ToString();
    }
  }

  public class SEEventChange
  {
    public eEvent Event { get; }
    public bool Active { get; }
    public SEScalarTime SimTime { get; }

    public SEEventChange(eEvent e, bool active, SEScalarTime simTime)
    {
      Event = e;
      Active = active;
      SimTime = new SEScalarTime(simTime);
    }

    public new string ToString()
    {
      string str;
      str = Event + " was " + Active;
      if (SimTime.IsValid())
        str += " @ " + SimTime.ToString();
      return str;
    }
  }
}
