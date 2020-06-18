/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarPressureTimePerAreaData;
import pulse.cdm.properties.CommonUnits.PressureTimePerAreaUnit;

/**
 * @author abray
 * Pressure Scalar Class, will enforce that units are proper to Pressures
 */

public class SEScalarPressureTimePerArea extends SEScalar
{
  public SEScalarPressureTimePerArea()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarPressureTimePerArea(double value, String unit)
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
  public SEScalarPressureTimePerArea(double value, PressureTimePerAreaUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarPressureTimePerAreaData src, SEScalarPressureTimePerArea dst)
  {
    SEScalar.load(src.getScalarPressureTimePerArea(),dst);
  }
  public static ScalarPressureTimePerAreaData unload(SEScalarPressureTimePerArea src)
  {
    if(!src.isValid())
      return null;
    ScalarPressureTimePerAreaData.Builder dst = ScalarPressureTimePerAreaData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarPressureTimePerArea src, ScalarPressureTimePerAreaData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarPressureTimePerAreaBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, PressureTimePerAreaUnit unit)
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
  public double getValue(PressureTimePerAreaUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(PressureTimePerAreaUnit.validUnit(unit))
      return true;
    return false;
  }
}
