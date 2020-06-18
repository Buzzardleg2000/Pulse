/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.TensionPneumothoraxData;
import pulse.cdm.bind.Enums.eGate;
import pulse.cdm.bind.Enums.eSide;

import pulse.cdm.properties.SEScalar0To1;

public class SETensionPneumothorax extends SEPatientAction
{
  private static final long serialVersionUID = 4051212107004337111L;
  
  protected eGate type;
  protected eSide side;
  protected SEScalar0To1 severity;
  
  public SETensionPneumothorax()
  {
    type = null;
    side = null;
    severity = null;
  }
  
  public void copy(SETensionPneumothorax other)
  {
    if(this==other)
      return;
    super.copy(other);
    type = other.type;
    side = other.side;
    if (other.severity != null)
      getSeverity().set(other.severity);
    else if (severity != null)
      severity.invalidate();
  }
  
  public void reset()
  {
    super.reset();
    type = null;
    side = null;
    if (severity != null)
      severity.invalidate();
  }
  
  public boolean isValid()
  {
    return hasType() && hasSide() && hasSeverity();
  }
  
  public static void load(TensionPneumothoraxData src, SETensionPneumothorax dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
    if(src.getType()!=eGate.UNRECOGNIZED)
    	dst.type = src.getType();
    if(src.getSide()!=eSide.UNRECOGNIZED)
    	dst.side = src.getSide();
  }
  
  public static TensionPneumothoraxData unload(SETensionPneumothorax src)
  {
    TensionPneumothoraxData.Builder dst = TensionPneumothoraxData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SETensionPneumothorax src, TensionPneumothoraxData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
    if (src.hasType())
      dst.setType(src.type);
    if (src.hasSide())
      dst.setSide(src.side);
  }
  
  public eGate getType()
  {
    return type;
  }
  public void setType(eGate type)
  {
    this.type = type;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  public eSide getSide()
  {
    return side;
  }
  public void setSide(eSide side)
  {
    this.side = side;
  }
  public boolean hasSide()
  {
    return side == null ? false : true;
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
  
  public String toString() 
  {
    if (severity != null)
      return "Pneumothorax" 
          + "\n\tSeverity: " + getSeverity()
          + "\n\tType : " + getType() 
          + "\n\tSide : " + getSide();
    else
      return "Action not specified properly";
  }

}
