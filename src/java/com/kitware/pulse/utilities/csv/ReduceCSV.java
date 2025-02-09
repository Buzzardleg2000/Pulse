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

import com.kitware.pulse.utilities.Log;

public class ReduceCSV
{
  public static void main(String[] args) throws IOException
  {
    String filename = "BasicStandard12hrResults.csv";

    Log.info("Looking at file: " + filename);
    File fIn = new File(filename);
    if(!fIn.exists())
    {
      Log.error("Say what!?, I don't see that file "+filename);
      return;
    }
    FileInputStream fStream = new FileInputStream(filename);
    DataInputStream in = new DataInputStream(fStream);
    BufferedReader bIn = new BufferedReader(new InputStreamReader(in));

    File fOut = new File("Reduced"+fIn.getName());
    FileOutputStream oStream = new FileOutputStream(fOut);
    BufferedWriter bOut = new BufferedWriter(new OutputStreamWriter(oStream));

    String line = bIn.readLine(); 
    bOut.write(line);
    bOut.newLine();

    int i=0;
    String aLine;
    while ((aLine=bIn.readLine()) != null) 
    {       
      if(i++ %90 == 0)
      {        
        bOut.write(aLine);
        bOut.newLine();
      }
    }       
    bIn.close();
    bOut.close();
  }
}
