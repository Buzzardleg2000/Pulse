/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.HemorrhageData;
import com.kitware.physiology.cdm.PatientActionEnums.eHemorrhage;
import com.kitware.physiology.cdm.PatientActionEnums.eIntubation.Type;
import com.kitware.physiology.datamodel.properties.SEScalarVolumePerTime;

public class SEHemorrhage extends SEPatientAction
{
  protected String compartment;
  protected eHemorrhage.Type type;
  protected SEScalarVolumePerTime rate;
  
  public SEHemorrhage()
  {
    compartment = null;
    rate = null;
    type = eHemorrhage.Type.External;
  }
  
  public void copy(SEHemorrhage other)
  {
    if(this==other)
      return;
    super.copy(other);
    compartment = other.compartment;
    if (other.rate != null)
      getRate().set(other.getRate());
    else if (rate != null)
      rate.invalidate();
    type = other.type;
  }
  
  public void reset()
  {
    super.reset();
    compartment = null;
    if (rate != null)
      rate.invalidate();
    type = eHemorrhage.Type.External;
  }
  
  public boolean isValid()
  {
    return hasRate() && hasCompartment();
  }
  
  public static void load(HemorrhageData src, SEHemorrhage dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.compartment = src.getCompartment();
    if(src.hasRate())
      SEScalarVolumePerTime.load(src.getRate(),dst.getRate());
    dst.type = src.getType();
  }
  
  public static HemorrhageData unload(SEHemorrhage src)
  {
    HemorrhageData.Builder dst = HemorrhageData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEHemorrhage src, HemorrhageData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasCompartment())
      dst.setCompartment(src.compartment);
    if (src.hasRate())
      dst.setRate(SEScalarVolumePerTime.unload(src.rate));
    dst.setType(src.type);
  }
  
  public String getCompartment()
  {
    return compartment;
  }
  public void setCompartment(Enum<?> compartment)
  {
    this.compartment = compartment.name();
  }
  public void setCompartment(String compartment)
  {
    this.compartment = compartment;
  }
  public boolean hasCompartment()
  {
    return compartment == null ? false : !compartment.isEmpty();
  }
  
  public boolean hasRate()
  {
    return rate == null ? false : rate.isValid();
  }
  public SEScalarVolumePerTime getRate()
  {
    if (rate == null)
      rate = new SEScalarVolumePerTime();
    return rate;
  }
  
  public eHemorrhage.Type getType()
  {
    return type;
  }
  public void setType(eHemorrhage.Type t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type != null;
  }
  
  public String toString()
  {
    if (rate != null)
      return "Hemorrhage" 
          + "\n\tRate: " + getRate() 
          + "\n\tCompartment: " + getCompartment()
          + "\n\tType: " + getType();
    else
      return "Action not specified properly";
  }
}



