/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ScalarAmountData;
import com.kitware.pulse.cdm.properties.CommonUnits.AmountUnit;

/**
 * @author abray
 * Amount Scalar Class, will enforce that units are proper to Amounts
 */

public class SEScalarAmount extends SEScalar
{
  public SEScalarAmount()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarAmount(double value, String unit)
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
  public SEScalarAmount(double value, AmountUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarAmountData src, SEScalarAmount dst)
  {
    SEScalar.load(src.getScalarAmount(),dst);
  }
  public static ScalarAmountData unload(SEScalarAmount src)
  {
    if(!src.isValid())
      return null;
    ScalarAmountData.Builder dst = ScalarAmountData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarAmount src, ScalarAmountData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarAmountBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, AmountUnit unit)
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
  public double getValue(AmountUnit unit)
  {
    return this.getValue(unit.toString());
  }
  
  @Override
  public boolean validUnit(String unit)
  {
    if(AmountUnit.validUnit(unit))
      return true;
    return false;
  }
}
