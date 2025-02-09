/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ScalarPressureTimeSquaredPerVolumeData;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureTimeSquaredPerVolumeUnit;

/**
 * @author abray
 * PressureTimeSquaredPerVolume Scalar Class, will enforce that units are proper to PressureTimeSquaredPerVolumes
 */

public class SEScalarPressureTimeSquaredPerVolume extends SEScalar
{
  public SEScalarPressureTimeSquaredPerVolume()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarPressureTimeSquaredPerVolume(double value, String unit)
  {
    this();
    this.setValue(value, unit);
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public SEScalarPressureTimeSquaredPerVolume(double value, PressureTimeSquaredPerVolumeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarPressureTimeSquaredPerVolumeData src, SEScalarPressureTimeSquaredPerVolume dst)
  {
    SEScalar.load(src.getScalarPressureTimeSquaredPerVolume(),dst);
  }
  public static ScalarPressureTimeSquaredPerVolumeData unload(SEScalarPressureTimeSquaredPerVolume src)
  {
    if(!src.isValid())
      return null;
    ScalarPressureTimeSquaredPerVolumeData.Builder dst = ScalarPressureTimeSquaredPerVolumeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarPressureTimeSquaredPerVolume src, ScalarPressureTimeSquaredPerVolumeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarPressureTimeSquaredPerVolumeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, PressureTimeSquaredPerVolumeUnit unit)
  {
    this.setValue(value,unit.toString());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double getValue(PressureTimeSquaredPerVolumeUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(PressureTimeSquaredPerVolumeUnit.validUnit(unit))
      return true;
    return false;
  }
}
