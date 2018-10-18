/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.properties;

import com.kitware.physiology.cdm.Properties.ScalarFlowComplianceData;

import com.kitware.physiology.datamodel.properties.CommonUnits.FlowComplianceUnit;

/**
 * @author abray
 * FlowCompliance Scalar Class, will enforce that units are proper to FlowCompliances
 */

public class SEScalarFlowCompliance extends SEScalar
{
  public SEScalarFlowCompliance()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarFlowCompliance(double value, String unit)
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
  public SEScalarFlowCompliance(double value, FlowComplianceUnit unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  public static void load(ScalarFlowComplianceData src, SEScalarFlowCompliance dst)
  {
    SEScalar.load(src.getScalarFlowCompliance(),dst);
  }
  public static ScalarFlowComplianceData unload(SEScalarFlowCompliance src)
  {
    if(!src.isValid())
      return null;
    ScalarFlowComplianceData.Builder dst = ScalarFlowComplianceData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScalarFlowCompliance src, ScalarFlowComplianceData.Builder dst)
  {
    SEScalar.unload(src,dst.getScalarFlowComplianceBuilder());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, FlowComplianceUnit unit)
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
  public double getValue(FlowComplianceUnit unit)
  {
    return this.getValue(unit.toString());
  }
  

  public boolean validUnit(String unit)
  {
    if(FlowComplianceUnit.validUnit(unit))
      return true;
    return false;
  }
}
