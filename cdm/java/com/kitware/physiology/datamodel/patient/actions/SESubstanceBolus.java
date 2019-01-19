/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.SubstanceBolusData;
import com.kitware.physiology.cdm.PatientActionEnums.eSubstanceAdministration.Route;

import com.kitware.physiology.datamodel.properties.SEScalarMassPerVolume;
import com.kitware.physiology.datamodel.properties.SEScalarVolume;
import com.kitware.physiology.datamodel.substance.SESubstance;

public class SESubstanceBolus extends SEPatientAction
{
  protected Route adminRoute;
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolume dose;
  protected SESubstance substance;
  
  public SESubstanceBolus(SESubstance substance)
  {
    this.adminRoute = null;
    this.dose = null;
    this.concentration = null;
    this.substance = substance;
  }
  
  public void copy(SESubstanceBolus other)
  {
    if(this==other)
      return;
    super.copy(other);
    adminRoute = other.adminRoute;
    substance = other.substance;
    
    if (other.dose != null)
      getDose().set(other.getDose());
    else if (dose != null)
      dose.invalidate();
    
    if (other.concentration != null)
      getConcentration().set(other.getConcentration());
    else if (concentration != null)
      concentration.invalidate();
  }
  
  public void reset()
  {
    super.reset();
    adminRoute = null;
    if (dose != null)
      dose.invalidate();
    if (concentration != null)
      concentration.invalidate();
  }
  
  public boolean isValid()
  {
    return hasAdministrationRoute() && hasDose() && hasConcentration() && hasSubstance();
  }
  
  public static void load(SubstanceBolusData src, SESubstanceBolus dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasDose())
      SEScalarVolume.load(src.getDose(),dst.getDose());
    if(src.hasConcentration())
      SEScalarMassPerVolume.load(src.getConcentration(),dst.getConcentration());
    if(src.getAdministrationRoute()!=Route.UNRECOGNIZED)
    	dst.adminRoute = src.getAdministrationRoute();
  }
  
  public static SubstanceBolusData unload(SESubstanceBolus src)
  {
    SubstanceBolusData.Builder dst = SubstanceBolusData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstanceBolus src, SubstanceBolusData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasDose())
      dst.setDose(SEScalarVolume.unload(src.dose));
    if (src.hasConcentration())
      dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration));
    if (src.hasAdministrationRoute())
      dst.setAdministrationRoute(src.adminRoute);
    if (src.hasSubstance())
      dst.setSubstance(src.substance.getName());
  }

  public Route getAdministrationRoute()
  {
    return adminRoute;
  }
  public void setAdministrationRoute(Route adminRoute)
  {
    this.adminRoute = adminRoute;
  }
  public boolean hasAdministrationRoute()
  {
    return adminRoute == null ? false : true;
  }
  
  public boolean hasConcentration()
  {
    return concentration == null ? false : concentration.isValid();
  }
  public SEScalarMassPerVolume getConcentration()
  {
    if (concentration == null)
      concentration = new SEScalarMassPerVolume();
    return concentration;
  }
  
  public boolean hasDose()
  {
    return dose == null ? false : dose.isValid();
  }
  
  public SEScalarVolume getDose()
  {
    if (dose == null)
      dose = new SEScalarVolume();
    return dose;
  }
  
  public SESubstance getSubstance()
  {
    return substance;
  }
  public boolean hasSubstance() { return substance != null; }
  
  public String toString()
  {
    if (dose != null || concentration != null)
      return "Substance Bolus" 
          + "\n\tDose: " + getDose()
          + "\n\tConcentration: " + getConcentration()
          + "\n\tSubstance: " + getSubstance().getName() 
          + "\n\tAdministration Route: " + getAdministrationRoute();
    else
      return "Action not specified properly";
  }
}
