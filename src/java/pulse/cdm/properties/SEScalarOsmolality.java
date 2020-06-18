/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarOsmolalityData;
import pulse.cdm.properties.CommonUnits.OsmolalityUnit;

/**
 * @author abray
 * Osmolality Scalar Class, will enforce that units are proper to Osmolality
 */

public class SEScalarOsmolality extends SEScalar
{
  public SEScalarOsmolality()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarOsmolality(double value, String unit)
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
  public SEScalarOsmolality(double value, OsmolalityUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarOsmolalityData src, SEScalarOsmolality dst)
  {
    SEScalar.load(src.getScalarOsmolality(),dst);
  }
  public static ScalarOsmolalityData unload(SEScalarOsmolality src)
  {
    if(!src.isValid())
      return null;
    ScalarOsmolalityData.Builder dst = ScalarOsmolalityData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarOsmolality src, ScalarOsmolalityData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarOsmolalityBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, OsmolalityUnit unit)
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
  public double getValue(OsmolalityUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(OsmolalityUnit.validUnit(unit))
      return true;
    return false;
  }
}
