/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarTimeData;
import pulse.cdm.properties.CommonUnits.TimeUnit;

/**
 * @author abray
 * Time Scalar Class, will enforce that units are proper to Times
 */

public class SEScalarTime extends SEScalar
{
  public SEScalarTime()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarTime(double value, String unit)
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
  public SEScalarTime(double value, TimeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarTimeData src, SEScalarTime dst)
  {
    SEScalar.load(src.getScalarTime(),dst);
  }
  public static ScalarTimeData unload(SEScalarTime src)
  {
    if(!src.isValid())
      return null;
    ScalarTimeData.Builder dst = ScalarTimeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarTime src, ScalarTimeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarTimeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, TimeUnit unit)
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
  public double getValue(TimeUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(TimeUnit.validUnit(unit))
      return true;
    return false;
  }
}
