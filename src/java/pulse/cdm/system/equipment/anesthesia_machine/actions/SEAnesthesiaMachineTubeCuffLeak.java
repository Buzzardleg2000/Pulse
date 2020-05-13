/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.anesthesia_machine.actions;

import pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineTubeCuffLeakData;

import pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineTubeCuffLeak extends SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineTubeCuffLeak()
  {
    severity = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AnesthesiaMachineTubeCuffLeakData src, SEAnesthesiaMachineTubeCuffLeak dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineTubeCuffLeakData unload(SEAnesthesiaMachineTubeCuffLeak src)
  {
    AnesthesiaMachineTubeCuffLeakData.Builder dst = AnesthesiaMachineTubeCuffLeakData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineTubeCuffLeak src, AnesthesiaMachineTubeCuffLeakData.Builder dst)
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
  
  public String toString()
  {
    if (severity != null)
      return "Tube Cuff Leak"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
