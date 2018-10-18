/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.properties;

import com.kitware.physiology.cdm.Properties.ScalarVolumePerPressureData;

import com.kitware.physiology.datamodel.properties.CommonUnits.VolumePerPressureUnit;

/**
 * @author abray
 * VolumePerPressure Scalar Class, will enforce that units are proper to VolumePerPressures
 */

public class SEScalarVolumePerPressure extends SEScalar
{
  public SEScalarVolumePerPressure()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarVolumePerPressure(double value, String unit)
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
  public SEScalarVolumePerPressure(double value, VolumePerPressureUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarVolumePerPressureData src, SEScalarVolumePerPressure dst)
  {
    SEScalar.load(src.getScalarVolumePerPressure(),dst);
  }
  public static ScalarVolumePerPressureData unload(SEScalarVolumePerPressure src)
  {
    if(!src.isValid())
      return null;
    ScalarVolumePerPressureData.Builder dst = ScalarVolumePerPressureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarVolumePerPressure src, ScalarVolumePerPressureData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarVolumePerPressureBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, VolumePerPressureUnit unit)
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
  public double getValue(VolumePerPressureUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(VolumePerPressureUnit.validUnit(unit))
      return true;
    return false;
  }
}
