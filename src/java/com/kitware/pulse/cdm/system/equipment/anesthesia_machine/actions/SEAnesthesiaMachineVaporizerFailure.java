/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;

import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineVaporizerFailureData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineVaporizerFailure extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = -7291568468320301722L;
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineVaporizerFailure()
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
  
  @Override
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AnesthesiaMachineVaporizerFailureData src, SEAnesthesiaMachineVaporizerFailure dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineVaporizerFailureData unload(SEAnesthesiaMachineVaporizerFailure src)
  {
    AnesthesiaMachineVaporizerFailureData.Builder dst = AnesthesiaMachineVaporizerFailureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineVaporizerFailure src, AnesthesiaMachineVaporizerFailureData.Builder dst)
  {
    SEAnesthesiaMachineAction.unload(src, dst.getAnesthesiaMachineActionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  /*
   * Severity
   */
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
      return "Vaoprizer Failure"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
