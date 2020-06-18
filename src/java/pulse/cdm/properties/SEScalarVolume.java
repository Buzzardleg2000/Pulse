/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarVolumeData;
import pulse.cdm.properties.CommonUnits.VolumeUnit;

/**
 * @author abray
 * Volume Scalar Class, will enforce that units are proper to Volumes
 */

public class SEScalarVolume extends SEScalar
{
  public SEScalarVolume()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarVolume(double value, String unit)
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
  public SEScalarVolume(double value, VolumeUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarVolumeData src, SEScalarVolume dst)
  {
    SEScalar.load(src.getScalarVolume(),dst);
  }
  public static ScalarVolumeData unload(SEScalarVolume src)
  {
    if(!src.isValid())
      return null;
    ScalarVolumeData.Builder dst = ScalarVolumeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarVolume src, ScalarVolumeData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarVolumeBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, VolumeUnit unit)
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
  public double getValue(VolumeUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(VolumeUnit.validUnit(unit))
      return true;
    return false;
  }
}
