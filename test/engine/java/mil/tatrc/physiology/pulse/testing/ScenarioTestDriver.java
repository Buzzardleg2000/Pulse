/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.pulse.testing;

import java.io.File;

import com.google.protobuf.TextFormat;
import com.kitware.physiology.cdm.Scenario.ScenarioData;
import com.kitware.physiology.cdm.Scenario.ScenarioData.StartTypeCase;
import com.kitware.physiology.pulse.*;

import mil.tatrc.physiology.datamodel.engine.SEAutoSerialization;
import mil.tatrc.physiology.pulse.engine.PulseScenarioExec;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.testing.SETestDriver;
import mil.tatrc.physiology.testing.SETestJob;

public class ScenarioTestDriver implements SETestDriver.Executor
{
  public boolean ExecuteTest(SETestJob job)
  {
    String outputFile = job.computedDirectory+"/"+job.name;
    String log;
    String results;
    String pba = FileUtils.readFile(job.baselineDirectory+"/"+job.name);
    if(pba==null)
    {
      Log.error("Could not read file : "+job.baselineDirectory+"/"+job.name);
      return false;
    }
    ScenarioData.Builder builder;
    Engine.ScenarioData.Builder pBuilder = Engine.ScenarioData.newBuilder();
    try
    {
    	TextFormat.getParser().merge(pba, pBuilder);
    	builder = pBuilder.getScenarioBuilder();
    }
    catch(Exception ex)
    {
	    try
	    {
	    	builder = pBuilder.getScenarioBuilder();
	    	TextFormat.getParser().merge(pba, builder);
	    }
	    catch(Exception ex2)
	    {
	    	Log.error("Unable to read scenario"+job.baselineDirectory+"/"+job.name,ex2);
	    	return false;
	    }
    }
    if(builder.getStartTypeCase()==StartTypeCase.STARTTYPE_NOT_SET)
    {
    	Log.error("Scenario does not have a start type");
    	return false;
    }
    
    if(job.patientFile==null)
    {
      log = outputFile.replaceAll("pba", "log");
      results = outputFile.replaceAll(".pba", "Results.csv");
    }
    else
    {
      String patientName = job.patientFile.substring(0,job.patientFile.length()-4);
      log = outputFile.replaceAll(".pba", "-"+patientName+".log");
      results = outputFile.replaceAll(".pba", "-"+patientName+"Results.csv");
      
      switch(builder.getStartTypeCase())      
      {
      	case INITIALPARAMETERS:
      		builder.getInitialParametersBuilder().clearPatient();
      		builder.getInitialParametersBuilder().setPatientFile(job.patientFile);
      		break;
      	case ENGINESTATEFILE:
      		builder.clearEngineStateFile();
      		builder.getInitialParametersBuilder().setPatientFile(job.patientFile);
        	break;
      }      
    }
    if(job.useState && builder.getStartTypeCase()==StartTypeCase.INITIALPARAMETERS)      
    {
      	String pFile = pBuilder.getScenario().getInitialParameters().getPatientFile();
      	pFile =  pFile.substring(0, pFile.indexOf(".pba"));
      	pFile = "./states/"+pFile+"@0s.pba";
      	builder.clearInitialParameters();
      	builder.setEngineStateFile(pFile);
    }

    if(job.autoSerialization!=null)
    	pBuilder.getConfigurationBuilder().setAutoSerialization(SEAutoSerialization.unload(job.autoSerialization)); 

    pba = pBuilder.toString();
    //System.out.println(pba);
    PulseScenarioExec pse = new PulseScenarioExec();
    pse.setListener(job);      
    pse.runScenario(log, pba, results);
    Log.info("Completed running "+job.name);
    pse=null;
    return true;
  }
  
  public static void main(String[] args)
  {
    SETestDriver.main(args);
  }
}
