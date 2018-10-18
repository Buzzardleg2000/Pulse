/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.substance.quantity;

import com.kitware.physiology.cdm.SubstanceQuantity.GasSubstanceQuantityData;

import com.kitware.physiology.datamodel.properties.SEScalar0To1;
import com.kitware.physiology.datamodel.properties.SEScalarPressure;
import com.kitware.physiology.datamodel.properties.SEScalarVolume;
import com.kitware.physiology.datamodel.substance.SESubstance;

public class SEGasSubstanceQuantity extends SESubstanceQuantity
{
  protected SEScalarPressure      partialPressure;
  protected SEScalarVolume        volume;
  protected SEScalar0To1          volumeFraction;

  public SEGasSubstanceQuantity(SESubstance s)
  {
    super(s);
    partialPressure = null;
    volume = null;
    volumeFraction = null;
  }

  public void reset()
  {
    super.reset();
   if (partialPressure != null)
      partialPressure.invalidate();
    if (volume != null)
      volume.invalidate();  
    if (volumeFraction != null)
      volumeFraction.invalidate();  
  }

  public static void load(GasSubstanceQuantityData src, SEGasSubstanceQuantity dst)
  {
    SESubstanceQuantity.load(src.getSubstanceQuantity(),dst);
    if (src.hasPartialPressure()) 
      SEScalarPressure.load(src.getPartialPressure(),dst.getPartialPressure()); 
    if (src.hasVolume()) 
      SEScalarVolume.load(src.getVolume(),dst.getVolume()); 
    if (src.hasVolumeFraction()) 
      SEScalar0To1.load(src.getVolumeFraction(),dst.getVolumeFraction()); 
  }
  public static GasSubstanceQuantityData unload(SEGasSubstanceQuantity src)
  {
    GasSubstanceQuantityData.Builder dst = GasSubstanceQuantityData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEGasSubstanceQuantity src, GasSubstanceQuantityData.Builder dst)
  {
    SESubstanceQuantity.unload(src,dst.getSubstanceQuantityBuilder());
    if (src.hasPartialPressure())
      dst.setPartialPressure(SEScalarPressure.unload(src.partialPressure));
    if (src.hasVolume())
      dst.setVolume(SEScalarVolume.unload(src.volume));
    if (src.hasVolumeFraction())
      dst.setVolumeFraction(SEScalar0To1.unload(src.volumeFraction));    
  }  

  public boolean hasPartialPressure()
  {
    return partialPressure == null ? false : partialPressure.isValid();
  }
  public SEScalarPressure getPartialPressure()
  {
    if (partialPressure == null)
      partialPressure = new SEScalarPressure();
    return partialPressure;
  }

  public boolean hasVolume()
  {
    return volume == null ? false : volume.isValid();
  }
  public SEScalarVolume getVolume()
  {
    if (volume == null)
      volume = new SEScalarVolume();
    return volume;
  }
  
  public boolean hasVolumeFraction()
  {
    return volumeFraction == null ? false : volumeFraction.isValid();
  }
  public SEScalar0To1 getVolumeFraction()
  {
    if (volumeFraction == null)
      volumeFraction = new SEScalar0To1();
    return volumeFraction;
  }
}
