/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.properties;

import pulse.cdm.bind.Properties.ScalarElectricPotentialData;
import pulse.cdm.properties.CommonUnits.ElectricPotentialUnit;

/**
 * @author abray
 * ElectricPotential Scalar Class, will enforce that units are proper to ElectricPotentials
 */

public class SEScalarElectricPotential extends SEScalar
{
  public SEScalarElectricPotential()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarElectricPotential(double value, String unit)
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
  public SEScalarElectricPotential(double value, ElectricPotentialUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarElectricPotentialData src, SEScalarElectricPotential dst)
  {
    SEScalar.load(src.getScalarElectricPotential(),dst);
  }
  public static ScalarElectricPotentialData unload(SEScalarElectricPotential src)
  {
    if(!src.isValid())
      return null;
    ScalarElectricPotentialData.Builder dst = ScalarElectricPotentialData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarElectricPotential src, ScalarElectricPotentialData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarElectricPotentialBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, ElectricPotentialUnit unit)
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
  public double getValue(ElectricPotentialUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(ElectricPotentialUnit.validUnit(unit))
      return true;
    return false;
  }
}
