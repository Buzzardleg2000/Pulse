/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package mil.tatrc.physiology.utilities;

import java.io.File;

import java.util.ArrayList;
import java.util.List;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;

/**
 * UnitConverter class that implements Unit Conversion using
 * UnitConversionJavaSupport DLL support
 * 
 * @author abray
 */
public class UnitConverter
{
  /** Loads Unit Conversion DLL */
  static
  {
  	String location = System.getProperty("user.dir");
    List<String>libs = new ArrayList<String>();
    libs.add("PulseJNI");
    if(!FileUtils.loadLibraries(libs,location))
      throw new RuntimeException("Could not find all dependent libraries : " + libs);
  }
  
  /**
   * Optional initialize method
   * @param workingDirectory
   * @return
   */
  public static boolean initialize(String workingDirectory)
  {
    File workingDirectoryFile = new File(workingDirectory).getAbsoluteFile();
    File f = new File(workingDirectoryFile.getAbsolutePath() + "/UCEDefs.txt").getAbsoluteFile();
    
    if(!f.exists())
    {
      //fLog.error("Unable to find "+workingDirectory+"/UCEDefs.txt");
      return false;
    }

    //nativeInitialize(workingDirectory);

    return true;
  }
  private static native void nativeInitialize(String workingDirectory);
  
  /**
   * Converts values from one unit to the other unit
   * 
   * @param value - original value
   * @param fromUnit - original unit
   * @param toUnit - new unit
   * @return - new value
   */
  public static double convert(double value, String fromUnit, String toUnit)// throws InvalidUnitsException
  {
    if(Double.isNaN(value))
    {
      //fLog.error("Conversion Input is NaN");
      return value;
    }
    if(fromUnit.equals(toUnit))
      return value;
    if(fromUnit.isEmpty()||toUnit.isEmpty())
    {
      //fLog.warn("No unit supplied to convertions, returning input");
      return value;
    }
    double d = nativeConvert(value, fromUnit, toUnit);
    //if(Double.isNaN(d))
      //fLog.error("Conversion equals NaN");
    return d;
  }
  /** nativeConvert method */
  private static native double nativeConvert(double value, String fromUnit, String toUnit);
  
  public static synchronized double[] convert(double[] data, String fromUnits, String toUnits)
  {
    if(fromUnits.equals(toUnits))
      return data;// TODO return a copy?

    if(!isCompatibleWithUnit(fromUnits, toUnits))
      throw new InvalidUnitException(fromUnits,toUnits);
    
    double[] tmp=new double[data.length];
    for(int i = 0; i < data.length; i++)
    {
      tmp[i]=convert(data[i], fromUnits, toUnits);
    }
    return tmp;
  }
  
  
  /**
   * Determines if units are compatible with each other 
   * 
   * @param unitTemplates - unit that we want to be like
   * @param unit - the unit in question
   * @return - boolean
   */
  public static boolean isCompatibleWithUnit(String unitTemplate, String unit)
  {
    if(unitTemplate==null)
      return false;
    if(unit.equals(unitTemplate))
      return true;
    return nativeIsCompatibleWithUnit(unitTemplate, unit);
  }
  
  /**
   * Determines if unit are compatible with an array of units 
   * 
   * @param unitTemplates - array of standard units
   * @param unit - unit comparing to
   * @return - boolean
   */
  public static boolean isCompatibleWithUnitList(String[] unitTemplates, String unit)
  {
    if(unitTemplates==null)
      return false;
    for(int i=0;i<unitTemplates.length;i++)
    {
      if(unit.equals(unitTemplates[i]))
        return true;
    }
    return nativeIsCompatibleWithUnitList(unitTemplates, unit);
  }
  /** nativeIsCompatibleWithUnitInList method */
  private static native boolean nativeIsCompatibleWithUnit(String unitTemplate, String unit);
  private static native boolean nativeIsCompatibleWithUnitList(String[] unitTemplates, String unit);
  
  /**
   * Determines if the provided unit is part of the 
   * specified unit quantity type
   * i.e. is MeV an Energy Unit
   * 
   * @param unitQuantity - unit quantity that we want to be like
   * @param unit - the unit in question
   * @return - boolean
   */
  public static boolean isOfQuantityType(String templateUnit, String unit)
  {
    if(templateUnit==null)
      return false;
    if(templateUnit.equals(unit))
      return true;
    return nativeIsOfQuantityType(templateUnit, unit);
  }
  
  /**
   * Determines if the provided unit is part of the 
   * specified unit quantity types
   * i.e. is MeV an Energy Unit
   * 
   * @param unitTemplates - array of standard units
   * @param unit - unit comparing to
   * @return - boolean
   */
  public static boolean isOfQuantityTypes(String[] templateUnits, String unit)
  {
    if(templateUnits==null||templateUnits.length==0)
      return false;
    if(unit==null)
      return false;
    for(String u : templateUnits)
    {
      if(unit.equals(u))
        return true;
    }
    return nativeIsOfQuantityTypes(templateUnits, unit);
  }
  /** nativeIsCompatibleWithUnitInList method */
  private static native boolean nativeIsOfQuantityType(String unitQuantity, String unit);
  private static native boolean nativeIsOfQuantityTypes(String[] unitQuantities, String unit);
  
}
