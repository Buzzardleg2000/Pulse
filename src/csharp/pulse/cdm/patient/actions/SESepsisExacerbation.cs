/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESepsisExacerbation : SEPatientAction
  {
    protected SEScalar0To1 infection_severity;
    protected SEScalar0To1 progression_severity;

    public SESepsisExacerbation()
    {
      infection_severity = null;
      progression_severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (infection_severity != null)
        infection_severity.Invalidate();
      if (progression_severity != null)
        progression_severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasInfectionSeverity() || HasProgressionSeverity();
    }

    public bool HasInfectionSeverity()
    {
      return infection_severity == null ? false : infection_severity.IsValid();
    }
    public SEScalar0To1 GetInfectionSeverity()
    {
      if (infection_severity == null)
        infection_severity = new SEScalar0To1();
      return infection_severity;
    }

    public bool HasProgressionSeverity()
    {
      return progression_severity == null ? false : progression_severity.IsValid();
    }
    public SEScalar0To1 GetProgressionSeverity()
    {
      if (progression_severity == null)
        progression_severity = new SEScalar0To1();
      return progression_severity;
    }
  }
}
