/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.SepsisExacerbationData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SESepsisExacerbation extends SEPatientAction
{
  private static final long serialVersionUID = 8862611857864194588L;
  protected SEScalar0To1 infectionSeverity;
  protected SEScalar0To1 progressionSeverity;
  
  public SESepsisExacerbation()
  {
    infectionSeverity = null;
    progressionSeverity = null;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (infectionSeverity != null)
      infectionSeverity.invalidate();
    if (progressionSeverity != null)
      progressionSeverity.invalidate();
  }
  
  public void copy(SESepsisExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.infectionSeverity != null)
      getInfectionSeverity().set(other.getInfectionSeverity());
    else if (infectionSeverity != null)
      infectionSeverity.invalidate();

    if (other.progressionSeverity != null)
      getProgressionSeverity().set(other.getProgressionSeverity());
    else if (progressionSeverity != null)
      progressionSeverity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasInfectionSeverity() || hasProgressionSeverity();
  }
  
  public static void load(SepsisExacerbationData src, SESepsisExacerbation dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasInfectionSeverity())
      SEScalar0To1.load(src.getInfectionSeverity(),dst.getInfectionSeverity());
    if(src.hasProgressionSeverity())
      SEScalar0To1.load(src.getProgressionSeverity(),dst.getProgressionSeverity());
  }
  
  public static SepsisExacerbationData unload(SESepsisExacerbation src) 
  {
    SepsisExacerbationData.Builder dst = SepsisExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESepsisExacerbation src, SepsisExacerbationData.Builder dst)
  {
    SEPatientAction.unload(src, dst.getPatientActionBuilder());
    if (src.hasInfectionSeverity())
      dst.setInfectionSeverity(SEScalar0To1.unload(src.infectionSeverity));
    if (src.hasProgressionSeverity())
      dst.setProgressionSeverity(SEScalar0To1.unload(src.progressionSeverity));
  }
  
  public boolean hasInfectionSeverity()
  {
    return infectionSeverity == null ? false : infectionSeverity.isValid();
  }
  public SEScalar0To1 getInfectionSeverity()
  {
    if (infectionSeverity == null)
      infectionSeverity = new SEScalar0To1();
    return infectionSeverity;
  }
 
  public boolean hasProgressionSeverity()
  {
    return progressionSeverity == null ? false : progressionSeverity.isValid();
  }
  public SEScalar0To1 getProgressionSeverity()
  {
    if (progressionSeverity == null)
      progressionSeverity = new SEScalar0To1();
    return progressionSeverity;
  }
  
  @Override
  public String toString()
  {
    return "Sepsis Exacerbation" 
        + "\n\tInfectionSeverity: " + getInfectionSeverity()
        + "\n\tProgressionSeverity: " + getProgressionSeverity();
  }
}
