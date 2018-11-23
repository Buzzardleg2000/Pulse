/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.testing.validation;

import java.util.Arrays;

import com.kitware.physiology.utilities.jniBridge;
import com.kitware.physiology.utilities.Log;

public class SystemValidation extends ValidationTool
{
  public SystemValidation()
  {
    DEST_DIRECTORY = "./test_results/tables/";
    DEFAULT_DIRECTORY = cfg.getValidationDirectory();
    DEFAULT_FILE = "SystemValidationData.xlsx";
    TABLE_TYPE = "System";
    VALIDATION_FOLDER = "systems";
  }
  public static void main(String[] args)
  {
    jniBridge.initialize();
    Log.setFileName("ValidationTool.log");
    Log.info("Running with agrs : "+Arrays.toString(args));
    SystemValidation me = new SystemValidation();
    if(args.length!=3)
    {
      me.loadData("Local","MSVC","x64");
    }
    else
    {      
      me.loadData(args[0], args[1], args[2]);
    }
    jniBridge.deinitialize();
  }

}
