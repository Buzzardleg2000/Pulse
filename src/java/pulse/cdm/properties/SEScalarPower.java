/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarPowerData;

import pulse.cdm.properties.CommonUnits.PowerUnit;

/**
 * @author abray
 * Power Scalar Class, will enforce that units are proper to Powers
 */

public class SEScalarPower extends SEScalar
{
  public SEScalarPower()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarPower(double value, String unit)
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
  public SEScalarPower(double value, PowerUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarPowerData src, SEScalarPower dst)
  {
    SEScalar.load(src.getScalarPower(),dst);
  }
  public static ScalarPowerData unload(SEScalarPower src)
  {
    if(!src.isValid())
      return null;
    ScalarPowerData.Builder dst = ScalarPowerData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarPower src, ScalarPowerData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarPowerBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, PowerUnit unit)
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
  public double getValue(PowerUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(PowerUnit.validUnit(unit))
      return true;
    return false;
  }
}
