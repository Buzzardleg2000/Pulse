/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import com.kitware.pulse.utilities.DoubleUtils;
import com.kitware.pulse.utilities.Log;

/**
 * @author abray
 *
 */
public class CSVContents
{
  // The file
  protected String          resultsFile;
  protected List<String>    headers = new ArrayList<>();
  List<Double>              rowDoubles = new ArrayList<>();

  protected FileInputStream fStream = null;
  protected DataInputStream in      = null;
  protected BufferedReader  buff    = null;
  
  protected boolean         contentMismatchErrorReported = false;
  
  public int abreviateContents = 0;
  
  public static void main(String[] args) throws IOException
  {
    CSVContents regular = new CSVContents("./scenarios/showcase/CombatMultitraumaResults.csv");
    List<Double> regularData = new ArrayList<>();
    regular.readHeader("Lead3ElectricPotential(mV)", regularData);
    double regularMax = DoubleUtils.getMax(regularData);
    double regularMin = DoubleUtils.getMin(regularData);
    System.out.println("Regular : Max : "+regularMax+" Min : "+regularMin);
    
    CSVContents nonInterp = new CSVContents("./scenarios/showcase/CombatMultitraumaResults-NoInterp.csv");
    List<Double> nonInterpData = new ArrayList<>();
    nonInterp.readHeader("Lead3ElectricPotential(mV)", nonInterpData);
    double nonInterpMax = DoubleUtils.getMax(nonInterpData);
    double nonInterpMin = DoubleUtils.getMin(nonInterpData);
    System.out.println("NonInterp : Max : "+nonInterpMax+" Min : "+nonInterpMin);
    
  }

  
  public CSVContents(String resultsFile) throws IOException
  {
    this.resultsFile = resultsFile;
    if(! new File(resultsFile).exists())
      throw new IOException("Results file not found : "+resultsFile);
  }
  
  @Override
  protected void finalize() throws IOException
  {
    if(in!=null)
      in.close();
  }

  public List<String> getHeaders() 
  {
    if(in == null)
    {
      if(!getBuffer(resultsFile))
      {
        Log.error("Unable to get buffer from file "+resultsFile);
        return null;
      }
      if(!readHeaders())
      {
        Log.error("Unable to get headers from file "+resultsFile);
        return null;
      }
    }
    return Collections.unmodifiableList(this.headers);
  }

  protected boolean getBuffer(String resultsFile)
  {
    try
    {
      if(in!=null)
        in.close();
      fStream = new FileInputStream(resultsFile);
      in = new DataInputStream(fStream);
      buff = new BufferedReader(new InputStreamReader(in));
      if(!headers.isEmpty())
        buff.readLine();
    }
    catch(Exception ex)
    {
      Log.error("Unable to get buffer for file "+resultsFile);
      Log.error(ex.getMessage());
      return false;
    }        
    return buff!=null;
  }

  protected boolean readHeaders()
  {
    if(!headers.isEmpty())
      return true;    
    try
    {
      String[] headers=readNextLine(buff);
      for(int i=0; i<headers.length; i++)
      {
        this.headers.add(unitUnderscoreToSpace(headers[i]));
      }
    }
    catch(Exception ex)
    {
      Log.error("Error reading next line");
      Log.error(ex.getMessage());
      return false;
    }
    return true;
  }

  /**
   * Reads data in and only stores the header column into memory
   */
  public boolean readHeader(String header, List<Double> data)
  {
    if(!getBuffer(resultsFile))
    {
      Log.error("Unable to get buffer from file "+resultsFile);
      return false;
    }
    if(!readHeaders())
    {
      Log.error("Unable to get headers from file "+resultsFile);
      return false;
    }

    int idx=this.headers.indexOf(header);
    if(idx==-1)
    {
      Log.error(header+" is not a valid header from results file");
      return false;
    }

    double d;
    boolean noZero=false;
    if(header.indexOf("PlasmaConcentration")>-1)
      noZero=true;
    data.clear();   
    while(parseLine(buff))
    {      
      d=this.rowDoubles.get(idx);
      if(noZero&&d<=0)
        d=Double.NaN;
      data.add(d);
      if(abreviateContents>0)
      {
        try
        {
          boolean eof=false;
          for(int i=0; i<abreviateContents; i++)
          {
            if(buff.readLine()==null)
            {
              eof=true;          
              break;
            }
          }
          if(eof)
            break;
        }
        catch(Exception ex)
        {
          Log.error("Error reading next line");
          Log.error(ex.getMessage());
          return false;
        }
      }
    }
    this.rowDoubles.clear();    
    return true;
  }


  /**
   * This will load the entire contents into memory
   * @param filePath
   * @throws IOException 
   * @throws FileNotFoundException
   */
  public boolean readAll(Map<String,List<Double>> results)
  {
    if(!getBuffer(resultsFile))
    {
      Log.error("Unable to get buffer from file "+resultsFile);
      return false;
    }
    if(!readHeaders())
    {
      Log.error("Unable to get headers from file "+resultsFile);
      return false;
    }
    
    results.clear();
    List<Double> list=null;

    int h;
    double d;   
    while(parseLine(buff))
    {      
      h=0;
      for(String header : this.headers)
      {
        list=results.get(header);
        if(list==null)
        {
          list=new ArrayList<>();          
          results.put(header, list);
        }
        d=this.rowDoubles.get(h++);
        if(d<=0&&header.indexOf("PlasmaConcentration")>-1)
          d=Double.NaN;
        list.add(d);
      }
      if(abreviateContents>0)
      {
        try
        {
          boolean eof=false;
          for(int i=0; i<abreviateContents; i++)
          {
            if(buff.readLine()==null)
            {
              eof=true;          
              break;
            }
          }
          if(eof)
            break;
        }
        catch(Exception ex)
        {
          Log.error("Error reading next line");
          Log.error(ex.getMessage());
          return false;
        }
      }
    }
    this.rowDoubles.clear();   
    return true;
  }
  
  public List<Double> readNextLine()
  {
    if(this.buff != null)
      parseLine(this.buff);    
    return Collections.unmodifiableList(this.rowDoubles);
  }

  protected String[] readNextLine(BufferedReader buff) throws IOException
  {
    String[] row;
    String aLine=null;
    aLine = buff.readLine();
    if(aLine==null)
      return null;
    String aLine2 = aLine.replaceAll("\\s+", "");
    row=aLine2.split(",");
    if(!this.headers.isEmpty()&&this.headers.size()!=row.length&&!this.contentMismatchErrorReported)
    {
      this.contentMismatchErrorReported = true; // Don't repeat this error endlessly
      Log.error(this.resultsFile+" did not find the expected number of results on line, there are "+this.headers.size()+" headers and "+row.length+" data columns on this row");
    }  
    return row;
  }

  protected boolean parseLine(BufferedReader buff)
  {
    this.rowDoubles.clear();
    if(this.buff == null)
      return false;
    double data;
    String[] rowData;

    try
    {
      rowData=readNextLine(buff);
    }
    catch(Exception ex)
    {
      Log.error("Error reading next line");
      Log.error(ex.getMessage());
      return false;
    }
    if(rowData==null)
      return false;
    for(int r=0; r<rowData.length; r++)
    {
      try
      {
        data = Double.parseDouble(rowData[r]);
      }
      catch(NumberFormatException ex)
      {
        if(rowData[r].indexOf("#")>-1||// C++ rounds the # in QNAN to an $ when precision is 0 
            rowData[r].indexOf("$")>-1||
            rowData[r].indexOf("IND")>-1||
            rowData[r].indexOf("ind")>-1||
            rowData[r].indexOf("INF")>-1||
            rowData[r].indexOf("inf")>-1||
            rowData[r].indexOf("NAN")>-1||
            rowData[r].indexOf("nan")>-1)
        {
          data = Double.NaN;          
        }
        else
        {
          Log.error("Error reading input on line " + this.headers.get(r));
          throw new RuntimeException("Error reading in data: non doubles detected");
        }
      }        
      this.rowDoubles.add(data);        
    }
    return true;
  }
  
  //Replace any underscore in units with a space
  // Units are in parens
  public String unitUnderscoreToSpace(String header)
  {
    int firstParen = header.indexOf("(");
    if(firstParen > -1)
    {
      int lastParen = header.indexOf(")",firstParen);
      String unit = header.substring(firstParen,lastParen+1);
      String property = header.substring(0,firstParen);
      header = property+unit.replaceAll("_", " ");
    }
    
    return header;
  }

}
