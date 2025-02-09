/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ScalarPressureTimePerVolumeAreaData;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureTimePerVolumeAreaUnit;

/**
 * @author abray
 * Pressure Scalar Class, will enforce that units are proper to Pressures
 */

public class SEScalarPressureTimePerVolumeArea extends SEScalar
{
  public SEScalarPressureTimePerVolumeArea()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarPressureTimePerVolumeArea(double value, String unit)
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
  public SEScalarPressureTimePerVolumeArea(double value, PressureTimePerVolumeAreaUnit unit)
  {
    this();
    this.setValue(value,unit);
  }

  public static void load(ScalarPressureTimePerVolumeAreaData src, SEScalarPressureTimePerVolumeArea dst)
  {
    SEScalar.load(src.getScalarPressureTimePerVolumeArea(),dst);
  }
  public static ScalarPressureTimePerVolumeAreaData unload(SEScalarPressureTimePerVolumeArea src)
  {
    if(!src.isValid())
      return null;
    ScalarPressureTimePerVolumeAreaData.Builder dst = ScalarPressureTimePerVolumeAreaData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarPressureTimePerVolumeArea src, ScalarPressureTimePerVolumeAreaData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarPressureTimePerVolumeAreaBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, PressureTimePerVolumeAreaUnit unit)
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
  public double getValue(PressureTimePerVolumeAreaUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(PressureTimePerVolumeAreaUnit.validUnit(unit))
      return true;
    return false;
  }
}
