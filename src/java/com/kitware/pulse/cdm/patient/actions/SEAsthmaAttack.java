/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.AsthmaAttackData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAsthmaAttack extends SEPatientAction
{
  private static final long serialVersionUID = -6357431990769735817L;
  
  protected SEScalar0To1 severity;
  
  public SEAsthmaAttack()
  {
    severity = null;
  }
  
  public void copy(SEAsthmaAttack other)
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
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AsthmaAttackData src, SEAsthmaAttack dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static AsthmaAttackData unload(SEAsthmaAttack src)
  {
    AsthmaAttackData.Builder dst = AsthmaAttackData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEAsthmaAttack src, AsthmaAttackData.Builder dst)
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
      return "AsthmaAttack" 
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
