/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;

import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineVentilatorPressureLossData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineVentilatorPressureLoss extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = -7214143013719559191L;
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineVentilatorPressureLoss()
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
  
  public static void load(AnesthesiaMachineVentilatorPressureLossData src, SEAnesthesiaMachineVentilatorPressureLoss dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineVentilatorPressureLossData unload(SEAnesthesiaMachineVentilatorPressureLoss src)
  {
    AnesthesiaMachineVentilatorPressureLossData.Builder dst = AnesthesiaMachineVentilatorPressureLossData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineVentilatorPressureLoss src, AnesthesiaMachineVentilatorPressureLossData.Builder dst)
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
      return "Ventilator Pressure Loss"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
