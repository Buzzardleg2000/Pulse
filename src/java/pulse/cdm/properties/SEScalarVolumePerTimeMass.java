/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarVolumePerTimeMassData;

import pulse.cdm.properties.CommonUnits.VolumePerTimeMassUnit;

/**
 * @author abray
 * VolumePerTimeMass Scalar Class, will enVolumePerTimeMass that units are proper to VolumePerTimeMasss
 */

public class SEScalarVolumePerTimeMass extends SEScalar
{
  public SEScalarVolumePerTimeMass()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarVolumePerTimeMass(double value, String unit)
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
  public SEScalarVolumePerTimeMass(double value, VolumePerTimeMassUnit unit)
  {
    this();
    this.setValue(value,unit);
  }

  public static void load(ScalarVolumePerTimeMassData src, SEScalarVolumePerTimeMass dst)
  {
    SEScalar.load(src.getScalarVolumePerTimeMass(),dst);
  }
  public static ScalarVolumePerTimeMassData unload(SEScalarVolumePerTimeMass src)
  {
    if(!src.isValid())
      return null;
    ScalarVolumePerTimeMassData.Builder dst = ScalarVolumePerTimeMassData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarVolumePerTimeMass src, ScalarVolumePerTimeMassData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarVolumePerTimeMassBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, VolumePerTimeMassUnit unit)
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
  public double getValue(VolumePerTimeMassUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(VolumePerTimeMassUnit.validUnit(unit))
      return true;
    return false;
  }
}
