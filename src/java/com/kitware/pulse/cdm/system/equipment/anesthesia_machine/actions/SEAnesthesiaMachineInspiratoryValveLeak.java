/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;

import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineInspiratoryValveLeakData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineInspiratoryValveLeak extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = -5073406968309010540L;
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineInspiratoryValveLeak()
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
  
  public static void load(AnesthesiaMachineInspiratoryValveLeakData src, SEAnesthesiaMachineInspiratoryValveLeak dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineInspiratoryValveLeakData unload(SEAnesthesiaMachineInspiratoryValveLeak src)
  {
    AnesthesiaMachineInspiratoryValveLeakData.Builder dst = AnesthesiaMachineInspiratoryValveLeakData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineInspiratoryValveLeak src, AnesthesiaMachineInspiratoryValveLeakData.Builder dst)
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
      return "Inspiratory Valve Leak"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
