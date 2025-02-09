/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.physiology;

import com.kitware.pulse.cdm.bind.Physiology.GastrointestinalSystemData;
import com.kitware.pulse.cdm.patient.nutrition.SENutrition;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;
import com.kitware.pulse.cdm.system.SESystem;

public class SEGastrointestinalSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarVolumePerTime  waterAbsorptionRate;
  protected SENutrition            stomachContents;

  public SEGastrointestinalSystem()
  {
    waterAbsorptionRate = null;
    stomachContents = null;
  }

  @Override
  public void clear()
  {    
    if (waterAbsorptionRate != null)
      waterAbsorptionRate.invalidate();
    if (stomachContents != null)
      stomachContents.clear();
  }

  public static void load(GastrointestinalSystemData src, SEGastrointestinalSystem dst)
  {    
    if (src.hasWaterAbsorptionRate())
      SEScalarVolumePerTime.load(src.getWaterAbsorptionRate(),dst.getWaterAbsorptionRate());
    if (src.hasStomachContents())
      SENutrition.load(src.getStomachContents(),dst.getStomachContents());
  }

  public static GastrointestinalSystemData unload(SEGastrointestinalSystem src)
  {
    GastrointestinalSystemData.Builder dst = GastrointestinalSystemData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SEGastrointestinalSystem src, GastrointestinalSystemData.Builder dst)
  {    
    if (src.hasWaterAbsorptionRate())
      dst.setWaterAbsorptionRate(SEScalarVolumePerTime.unload(src.getWaterAbsorptionRate()));
    if (src.hasStomachContents())
      dst.setStomachContents(SENutrition.unload(src.getStomachContents()));
  }

  public boolean hasWaterAbsorptionRate()
  {
    return waterAbsorptionRate == null ? false : waterAbsorptionRate.isValid();
  }
  public SEScalarVolumePerTime getWaterAbsorptionRate()
  {
    if (waterAbsorptionRate == null)
      waterAbsorptionRate = new SEScalarVolumePerTime();
    return waterAbsorptionRate;
  }

  public boolean hasStomachContents()
  {
    return stomachContents == null ? false : true;
  }
  public SENutrition getStomachContents()
  {
    if (stomachContents == null)
      stomachContents = new SENutrition();
    return stomachContents;
  }
}
