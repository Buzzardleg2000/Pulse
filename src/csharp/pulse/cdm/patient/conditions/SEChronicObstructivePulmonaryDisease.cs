/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  using LungImpairmentMap = Dictionary<eLungCompartment, SEScalar0To1>;
  public class SEChronicObstructivePulmonaryDisease : SEPatientCondition
  {
    protected SEScalar0To1 bronchitis_severity;
    protected LungImpairmentMap emphysema_severities;

    public SEChronicObstructivePulmonaryDisease()
    {
      bronchitis_severity = null;
      emphysema_severities = new LungImpairmentMap();
    }

    public override void Clear()
    {
      base.Clear();
      if (bronchitis_severity != null)
        bronchitis_severity.Invalidate();
      foreach (var itr in emphysema_severities)
        itr.Value.Invalidate();
    }

    public override bool IsValid()
    {
      return HasBronchitisSeverity() && HasEmphysemaSeverity();
    }

    public bool HasBronchitisSeverity()
    {
      return bronchitis_severity == null ? false : bronchitis_severity.IsValid();
    }
    public SEScalar0To1 GetBronchitisSeverity()
    {
      if (bronchitis_severity == null)
        bronchitis_severity = new SEScalar0To1();
      return bronchitis_severity;
    }

    public bool HasEmphysemaSeverity()
    {
      foreach (var itr in emphysema_severities)
        if (itr.Value.IsValid())
          return true;
      return false;
    }
    public bool HasEmphysemaSeverity(eLungCompartment c)
    {
      if (!emphysema_severities.ContainsKey(c))
        return false;
      return emphysema_severities[c].IsValid();
    }
    public SEScalar0To1 GetEmphysemaSeverity(eLungCompartment c)
    {
      SEScalar0To1 s = emphysema_severities[c];
      if (s == null)
      {
        s = new SEScalar0To1();
        emphysema_severities[c] = s;
      }
      return s;
    }
    public LungImpairmentMap GetEmphysemaSeverities()
    {
      return emphysema_severities;
    }
  }
}
