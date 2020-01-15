/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEAnesthesiaMachineInspiratoryValveLeak : SEAnesthesiaMachineAction
  {
    protected SEScalar0To1 severity;

    public SEAnesthesiaMachineInspiratoryValveLeak()
    {
      severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (severity != null)
        severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasSeverity();
    }

    public bool HasSeverity()
    {
      return severity == null ? false : severity.IsValid();
    }
    public SEScalar0To1 GetSeverity()
    {
      if (severity == null)
        severity = new SEScalar0To1();
      return severity;
    }

    public override string ToString()
    {
      if (severity != null)
        return "Inspiratory Valve Leak"
            + "\n\tSeverity: " + GetSeverity();
      else
        return "Inspiratory Valve Leak not specified properly";
    }
  }
}
