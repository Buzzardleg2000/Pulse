/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.properties;

import pulse.cdm.bind.Properties.HistogramData;
import pulse.cdm.bind.Properties.HistogramFractionVsLengthData;

import pulse.cdm.exceptions.InvalidUnitException;
import pulse.cdm.properties.CommonUnits.LengthUnit;


/**
 * @author abray
 */
public class SEHistogramFractionVsLength extends SEHistogram
{
  public SEHistogramFractionVsLength()
  {
    super();
  }
    
  public static void load(HistogramFractionVsLengthData src, SEHistogramFractionVsLength dest)
  {
   SEHistogram.load(src.getHistogramFractionVsLength(), dest);
  }
  public static HistogramFractionVsLengthData unload(SEHistogramFractionVsLength src)
  {
    if(!src.isValid())
      return null;
    HistogramFractionVsLengthData.Builder dst = HistogramFractionVsLengthData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEHistogramFractionVsLength src, HistogramFractionVsLengthData.Builder dst)
  {
    SEHistogram.unload(src,dst.getHistogramFractionVsLengthBuilder());
  }

  public boolean isValidDependentUnit(String unit)
  {
    if(unit == null || unit.isEmpty())
      return true;
    return false;
  }
  
  public boolean isValidIndependentUnit(String unit)
  {
    return LengthUnit.validUnit(unit);
  }
  
  public String getLengthUnit()
  {
    return this.independentUnit;
  }
 
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setFraction(double[] data)
  {
    setDependent(data);
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setFraction(String data)
  {
    setDependent(data);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getFraction()
  {
    return getDependent();
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getFractionString()
  {
    return getDependentString();
  }
  
  // Independent //
  
  public double[] getLength(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    return getIndependent(unit);
  }
  
  public String getLengthString(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    return getIndependentString(unit);
  }
  
  public void setLength(double[] data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    setIndependent(data, unit);
  }
  
  public void setLength(String data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    setIndependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getLength(LengthUnit unit)
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
  public String getLengthString(LengthUnit unit)
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
  public void setLength(double[] data, LengthUnit unit)
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
  public void setLength(String data, LengthUnit unit)
  {
    setIndependent(data, unit.toString());
  }
}
