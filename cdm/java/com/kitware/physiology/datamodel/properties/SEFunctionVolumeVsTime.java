/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.properties;

import org.jfree.util.Log;

import com.kitware.physiology.cdm.Properties.DoubleArrayData;
import com.kitware.physiology.cdm.Properties.FunctionVolumeVsTimeData;

import com.kitware.physiology.datamodel.exceptions.InvalidUnitException;
import com.kitware.physiology.datamodel.properties.CommonUnits.TimeUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.VolumeUnit;


/**
 * @author abray
 */
public class SEFunctionVolumeVsTime extends SEFunction
{
  public SEFunctionVolumeVsTime()
  {
    super();
  }
    
  public static void load(FunctionVolumeVsTimeData src, SEFunctionVolumeVsTime dst)
  {
    SEFunction.load(src.getFunctionVolumeVsTime(),dst);
  }
  public static FunctionVolumeVsTimeData unload(SEFunctionVolumeVsTime src)
  {
    if(!src.isValid())
      return null;
    FunctionVolumeVsTimeData.Builder dst = FunctionVolumeVsTimeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEFunctionVolumeVsTime src, FunctionVolumeVsTimeData.Builder dst)
  {
    SEFunction.unload(src,dst.getFunctionVolumeVsTimeBuilder());
  }

  public boolean isValidDependentUnit(String unit)
  {    
    return VolumeUnit.validUnit(unit);
  }
  
  public boolean isValidIndependentUnit(String unit)
  {
    return TimeUnit.validUnit(unit);
  }
  
  public String getVolumeUnit()
  {
    return this.dependentUnit;
  }
  
  public String getTimeUnit()
  {
    return this.independentUnit;
  }

  public double[] getVolume(String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "Volume");
    return getDependent(unit);
  }
  
  public String getVolumeString(String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "Volume");
    return getDependentString(unit);
  }
  
  public void setVolume(double[] data, String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "Volume");
    setDependent(data, unit);
  }
  
  public void setVolume(String data, String unit)
  {
    if(!isValidDependentUnit(unit))
      throw new InvalidUnitException(unit, "Volume");
    setDependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getVolume(VolumeUnit unit)
  {
    return getDependent(unit.toString());
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getVolumeString(VolumeUnit unit)
  {
    return getDependentString(unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setVolume(double[] data, VolumeUnit unit)
  {
    setDependent(data, unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setVolume(String data, VolumeUnit unit)
  {
    setDependent(data, unit.toString());
  }
  
  public double[] getTime(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    return getIndependent(unit);
  }
  
  public String getTimeString(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    return getIndependentString(unit);
  }
  
  public void setTime(double[] data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    setIndependent(data, unit);
  }
  
  public void setTime(String data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Time");
    setIndependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getTime(TimeUnit unit)
  {
    return getIndependent(unit.toString());
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getTimeString(TimeUnit unit)
  {
    return getIndependentString(unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setTime(double[] data, TimeUnit unit)
  {
    setIndependent(data, unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setTime(String data, TimeUnit unit)
  {
    setIndependent(data, unit.toString());
  }
}
