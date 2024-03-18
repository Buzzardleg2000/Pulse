/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eEvent : int
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
    Hypernatremia                   = 15,
    Hyperthermia                    = 16,
    Hypoglycemia                    = 17,
    Hyponatremia                    = 18,
    Hypothermia                     = 19,
    Hypoxia                         = 20,
    HypovolemicShock                = 21,
    IntracranialHypertension        = 22,
    IntracranialHypotension         = 23,
    IrreversibleState               = 24,
    Ketoacidosis                    = 25,
    LacticAcidosis                  = 26,
    MassiveHemothorax               = 27,
    MaximumPulmonaryVentilationRate = 28,
    MediumHemothorax                = 29,
    MetabolicAcidosis               = 30,
    MetabolicAlkalosis              = 31,
    MinimalHemothorax               = 32,
    ModerateHyperoxemia             = 33,
    ModerateHypocapnia              = 34,
    MyocardiumOxygenDeficit         = 35,
    Natriuresis                     = 36,
    NutritionDepleted               = 37,
    RenalHypoperfusion              = 38,
    RespiratoryAcidosis             = 39,
    RespiratoryAlkalosis            = 40,
    SevereHyperoxemia               = 41,
    SevereHypocapnia                = 42,
    Stabilizing                     = 43,
    StartOfCardiacCycle             = 44,
    StartOfExhale                   = 45,
    StartOfInhale                   = 46,
    Tachycardia                     = 47,
    Tachypnea                       = 48,

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
