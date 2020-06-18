/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.testing;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import pulse.utilities.FileUtils;
import pulse.utilities.Log;
import pulse.utilities.RunConfiguration;
import pulse.utilities.jniBridge;

public class Rebase
{
  public static void main(String[] args)
  {
    String hash = null;
    if(args.length != 0)
      hash = args[0];
    jniBridge.initialize();
    rebase(hash);
    jniBridge.deinitialize();
  }
  
  public static void rebase(String hash)
  {
    RunConfiguration cfg = new RunConfiguration();
    String toDir = "./test_results/rebase/";
    List<String> configFiles = new ArrayList<String>();
    try
    {
      File file = new File(cfg.getTestConfigDirectory()+"/Rebase.config");
      FileReader fileReader = new FileReader(file);
      BufferedReader bufferedReader = new BufferedReader(fileReader);
      String line;
      while ((line = bufferedReader.readLine()) != null) 
      {
        line = line.trim();
        if(line.indexOf("#")>-1)
          continue;
        if(line.indexOf(".config")==-1)
            continue;
        configFiles.add(line);
      }
      fileReader.close();
    }
    catch(IOException ex)
    {
      Log.error("Unable to open Rebase.config, check your run.config file test_config_dir");
      return;
    }
    
    // Replace back slashes with forward slashes
    toDir = toDir.replaceAll("\\\\", "/");
    toDir += new SimpleDateFormat("yyyy-MM-dd").format(new Date());
    if(hash != null && !hash.isEmpty())
      toDir += " - "+hash;
    toDir += "/";

    Log.info("Rebasing to "+toDir);

    // Make sure from/to dir ends with a slash
    if(!toDir.endsWith("/"))
      toDir = toDir + "/";
    try
    {
      FileUtils.delete(toDir);
      FileUtils.createDirectory(toDir);
    }
    catch(Exception ex)
    {
      Log.error("Could not create to Directory : "+toDir,ex);
      return;
    }

    String log_file;
    String result_path;
    List<String> result_files = new ArrayList<String>();

    for(String configFile : configFiles)
    {
      Log.info("Rebasing config "+configFile);
      SETestConfiguration config = new SETestConfiguration();
      config.processConfigFile(new File(cfg.getTestConfigDirectory()+"/"+configFile), cfg);

      for(SETestJob job : config.jobs)
      {
        if(job.computedFiles.size()<=0)
        {
          Log.error("A job has no expected result files!?!?");
          continue;
        }
        if(!job.PlottableResults && !job.isAssessment)
          continue;// Nothing to plot/compare, nothing to zip  
        // We assume all json will have something to zip...
        
        log_file = "";
        result_files.clear();
        result_path = job.computedFiles.get(0);
        if(!new File(result_path).exists())
        {
          Log.error("Unable to find file to rebase for "+job.name+" at path "+result_path);
          continue;
        }
        
        // If there is no baselineDirectory, then this must be an assessment...
        if(job.name.endsWith(".json") && !job.isAssessment)
        {
          if(job.computedFiles.size()>1)
          {
            Log.error("A scenario file has more than one result file!?!?");
            continue;
          }
          result_files.add(result_path);
          log_file = result_files.get(0).replaceAll("Results"+SETestConfiguration.ext, ".log");   
          result_files.add(log_file);
          Log.info("Creating zip for "+result_files.get(0));
        }      
        else
        {
          // Its a unit test, only zip up the results
          result_files.addAll(job.computedFiles);
        }

        String prepend = result_path.substring(result_path.indexOf("test_results/")+13,result_path.lastIndexOf("."));
        int ridx = prepend.indexOf("Results");
        if(ridx>-1)
          prepend = prepend.substring(0,ridx);
        String newFile = toDir + prepend + ".zip";

        Log.info("Creating new file "+newFile);
        String rPath = "";
        for(String rDir : newFile.split("/"))
        {
          if(rDir.endsWith(".zip"))
          {
            break;
          }
          rPath += rDir + "/";
        }
        try
        {
          Log.info("Creating "+rPath);
          FileUtils.createDirectory(rPath);
        } 
        catch (IOException ex)
        {
          Log.error("Could not create directory : "+rPath, ex);
        }
        File oldZip = new File(newFile);
        if(oldZip.exists())
          oldZip.delete();
        String[] files = new String[result_files.size()];
        files = result_files.toArray(files);
        FileUtils.zipFiles(files, newFile,prepend+".zip");
      }
    }
  }

}
