/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package mil.tatrc.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.BrainInjuryData;
import com.kitware.physiology.cdm.PatientActionEnums.eBrainInjury.Type;

import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEBrainInjury extends SEPatientAction
{
  protected SEScalar0To1  severity;
  protected Type          type;
  
  public SEBrainInjury()
  {
    severity = null;
    type     = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
    this.type = null;
  }
  
  public void copy(SEBrainInjury other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
    this.type = other.type;
  }
  
  public boolean isValid()
  {
    return hasSeverity() && hasType();
  }
  
  public static void load(BrainInjuryData src, SEBrainInjury dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=Type.UNRECOGNIZED)
    	dst.setType(src.getType());
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static  BrainInjuryData unload(SEBrainInjury src)
  {
    BrainInjuryData.Builder dst = BrainInjuryData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEBrainInjury src, BrainInjuryData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if(src.hasType())
      dst.setType(src.type);
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  public Type getType()
  {
    return type;
  }
  public void setType(Type t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
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
      return "Brain Injury" 
          + "\n\tType: " + getType()
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
