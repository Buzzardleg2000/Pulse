/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.properties;

import com.kitware.physiology.cdm.Properties.ScalarAmountPerTimeData;

import com.kitware.physiology.datamodel.properties.CommonUnits.AmountPerTimeUnit;

/**
 * @author abray
 * AmountPerTime Scalar Class, will enforce that units are proper to AmountPerTimes
 */

public class SEScalarAmountPerTime extends SEScalar
{
  public SEScalarAmountPerTime()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarAmountPerTime(double value, String unit)
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
  public SEScalarAmountPerTime(double value, AmountPerTimeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarAmountPerTimeData src, SEScalarAmountPerTime dst)
  {
    SEScalar.load(src.getScalarAmountPerTime(),dst);
  }
  public static ScalarAmountPerTimeData unload(SEScalarAmountPerTime src)
  {
    if(!src.isValid())
      return null;
    ScalarAmountPerTimeData.Builder dst = ScalarAmountPerTimeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarAmountPerTime src, ScalarAmountPerTimeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarAmountPerTimeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, AmountPerTimeUnit unit)
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
  public double getValue(AmountPerTimeUnit unit)
  {
    return this.getValue(unit.toString());
  }
  

  public boolean validUnit(String unit)
  {
    if(AmountPerTimeUnit.validUnit(unit))
      return true;
    return false;
  }
}
