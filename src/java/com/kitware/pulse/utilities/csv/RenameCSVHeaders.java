/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;

public class RenameCSVHeaders
{
  public static void main(String[] args) throws IOException
  {
    Map<String,String> replaceWith = new HashMap<>();
    //replaceWith.put("ECGWaveform", "Lead3ElectricPotential");
    boolean convertToCSV = true;
    
    
    List<String> baselines = FileUtils.findFiles("..\\verification\\UnitTests\\", ".csv", true);
    for(String filename : baselines)
    {
      Log.info("Looking at file: " + filename);
      File fIn = new File(filename);
      if(!fIn.exists())
      {
        Log.error("Say what!?");
        continue;
      }
      FileInputStream fStream = new FileInputStream(filename);
      DataInputStream in = new DataInputStream(fStream);
      BufferedReader bIn = new BufferedReader(new InputStreamReader(in));
      
      boolean rewriteFile=false;
      String origHeader = bIn.readLine(); 
      String newHeader = origHeader;      
      String headers = origHeader.replaceAll("\\s+", " ");
      for(String header : headers.split(" "))
      {
        for(String replace : replaceWith.keySet())
        {
          if(header.indexOf(replace)>-1)
          {
            rewriteFile = true;
            newHeader=origHeader.replaceAll(replace, replaceWith.get(replace));
          }
        }
      }
      if(rewriteFile || convertToCSV)
      {
        if(rewriteFile)
        {
          Log.info("Replacing header for file: " + filename);
          Log.info("Old Header : " + origHeader);
          Log.info("New Header : " + newHeader);
        }
        if(convertToCSV)
        {
          Log.info("Converting file from tab delimited to comma delimited");
          newHeader=newHeader.replaceAll("\t", ",");
          if(newHeader.endsWith(","))
            newHeader=newHeader.substring(0, newHeader.length()-1);
        }
        File fOut = new File(filename+"new");
        FileOutputStream oStream = new FileOutputStream(fOut);
        BufferedWriter bOut = new BufferedWriter(new OutputStreamWriter(oStream));
        bOut.write(newHeader);
        bOut.newLine();
        String aLine;
        while ((aLine=bIn.readLine()) != null) 
        {
          if(convertToCSV)
          {
            aLine=aLine.replaceAll("\t", ",");
            if(aLine.endsWith(","))
              aLine=aLine.substring(0, aLine.length()-1);
          }
          bOut.write(aLine);
          bOut.newLine();
        }       
        bIn.close();
        bOut.close();
        fIn.delete();
        if(!fOut.renameTo(fIn))
          Log.error("Could not rename new header file to: "+filename);
      }      
      bIn.close();
    }       
  }
}
