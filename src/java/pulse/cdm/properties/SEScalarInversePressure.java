/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarInversePressureData;
import pulse.cdm.properties.CommonUnits.InversePressureUnit;

/**
 * @author abray
 * InversePressure Scalar Class, will enforce that units are proper to InversePressures
 */

public class SEScalarInversePressure extends SEScalar
{
  public SEScalarInversePressure()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarInversePressure(double value, String unit)
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
  public SEScalarInversePressure(double value, InversePressureUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarInversePressureData src, SEScalarInversePressure dst)
  {
    SEScalar.load(src.getScalarInversePressure(),dst);
  }
  public static ScalarInversePressureData unload(SEScalarInversePressure src)
  {
    if(!src.isValid())
      return null;
    ScalarInversePressureData.Builder dst = ScalarInversePressureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarInversePressure src, ScalarInversePressureData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarInversePressureBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, InversePressureUnit unit)
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
  public double getValue(InversePressureUnit unit)
  {
    return this.getValue(unit.toString());
  }

  @Override
  public boolean validUnit(String unit)
  {
    if(InversePressureUnit.validUnit(unit))
      return true;
    return false;
  }
}
