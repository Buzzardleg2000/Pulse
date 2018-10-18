/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.properties;

import com.kitware.physiology.cdm.Properties.ScalarMassData;

import com.kitware.physiology.datamodel.properties.CommonUnits.MassUnit;

/**
 * @author abray
 * Mass Scalar Class, will enforce that units are proper to Masss
 */

public class SEScalarMass extends SEScalar
{
  public SEScalarMass()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarMass(double value, String unit)
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
  public SEScalarMass(double value, MassUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarMassData src, SEScalarMass dst)
  {
    SEScalar.load(src.getScalarMass(),dst);
  }
  public static ScalarMassData unload(SEScalarMass src)
  {
    if(!src.isValid())
      return null;
    ScalarMassData.Builder dst = ScalarMassData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarMass src, ScalarMassData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarMassBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, MassUnit unit)
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
  public double getValue(MassUnit unit)
  {
    return this.getValue(unit.toString());
  }

  public boolean validUnit(String unit)
  {
    if(MassUnit.validUnit(unit))
      return true;
    return false;
  }
}
