/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import com.kitware.pulse.cdm.bind.PatientConditions.PulmonaryShuntData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEPulmonaryShunt extends SEPatientCondition
{

  private static final long serialVersionUID = 8862611857864194588L;
  protected SEScalar0To1 severity;
  
  public SEPulmonaryShunt()
  {
    severity = null;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (severity != null)
      severity.invalidate();
  }
  
  public void copy(SEPulmonaryShunt other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  public static void load(PulmonaryShuntData src, SEPulmonaryShunt dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static PulmonaryShuntData unload(SEPulmonaryShunt src) 
  {
    PulmonaryShuntData.Builder dst = PulmonaryShuntData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEPulmonaryShunt src, PulmonaryShuntData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }
  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }
  
  @Override
  public String toString()
  {
    return "Pulmonary Shunt" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}
