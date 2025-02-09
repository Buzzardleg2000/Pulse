/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.AcuteStressData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAcuteStress extends SEPatientAction
{
  private static final long serialVersionUID = 1086982565287954007L;
  
  protected SEScalar0To1 severity;
  
  public SEAcuteStress()
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
  
  public void copy(SEAcuteStress other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AcuteStressData src, SEAcuteStress dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static AcuteStressData unload(SEAcuteStress src)
  {
    AcuteStressData.Builder dst = AcuteStressData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEAcuteStress src, AcuteStressData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
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
    if (severity != null)
      return "Acute Stress" 
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
