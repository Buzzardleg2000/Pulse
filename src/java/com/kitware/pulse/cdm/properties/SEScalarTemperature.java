/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ScalarTemperatureData;
import com.kitware.pulse.cdm.properties.CommonUnits.TemperatureUnit;

/**
 * @author abray
 * Temperature Scalar Class, will enforce that units are proper to temperatures
 */

public class SEScalarTemperature extends SEScalar
{
  public SEScalarTemperature()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarTemperature(double value, String unit)
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
  public SEScalarTemperature(double value, TemperatureUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarTemperatureData src, SEScalarTemperature dst)
  {
    SEScalar.load(src.getScalarTemperature(),dst);
  }
  public static ScalarTemperatureData unload(SEScalarTemperature src)
  {
    if(!src.isValid())
      return null;
    ScalarTemperatureData.Builder dst = ScalarTemperatureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarTemperature src, ScalarTemperatureData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarTemperatureBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, TemperatureUnit unit)
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
  public double getValue(TemperatureUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(TemperatureUnit.validUnit(unit))
      return true;
    return false;
  }
}
