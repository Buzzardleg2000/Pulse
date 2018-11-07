/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.pulse.engine;

import com.kitware.physiology.cdm.EngineEnums.eDataRequest;

import com.kitware.physiology.datamodel.actions.SEAdvanceTime;
import com.kitware.physiology.datamodel.datarequests.SEDataRequest;
import com.kitware.physiology.datamodel.properties.CommonUnits.FrequencyUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.TimeUnit;
import com.kitware.physiology.datamodel.properties.CommonUnits.VolumeUnit;
import com.kitware.physiology.datamodel.scenario.SEScenario;
import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.utilities.LogListener;
import com.kitware.physiology.utilities.jniBridge;

public class HowTo_RunScenario
{
  // Create a callback object that Pulse will call at the specified update frequency.
  // Note the time provided refers to simulation time, not real time
  // For example, if you request a callback every second, it will be called
  // every time 1 second is simulated
  protected static class MyCallback extends CDMUpdatedCallback
  {
    protected Pulse pe;
    protected MyCallback(Pulse pe, double updateFrequency_s)
    {
      super(updateFrequency_s);
      this.pe = pe;
    }
    
    public void update(double time_s)
    {
      // Note only the data requested (As a DataRequest) in the scenario file will be updated in the Pulse CDM objects
      // All data is pulled from the engine at callback time, so when we get here, the CDM objects have the latest values      
      Log.info("Heart Rate " + pe.cardiovascular.getHeartRate());
      Log.info("Respiration Rate " + pe.respiratory.getRespirationRate());
      Log.info("Total Lung Volume " + pe.respiratory.getTotalLungVolume());      
    }    
  }
  
  // Create a listener that will catch any messages logged in C++
  // This class will take the messages and add them to the log created in Java
  protected static class MyListener extends LogListener
  {
    protected MyListener()
    {
      super();
      // Since we are just pushing this data into the log
      // We set listen to false, when you create a LogListener
      // is will automatically register itself with the static Log class,
      // and when ever a Log even class is called, this will be called
      // and since this just calls the Log to log you will get into a recursive infinite loop.
      // This is just because I use the LogListener interface to listen to any log messages coming
      // from C++. Technically it is a LogListener as it is 'listening' to log events in C++.
      // At any rate the Java side will receive log events from C++, and you create your own 
      // LogListener so you can do as you see fit with those messages. BUT if you do want to 
      // push those messages to the Java Log, you will need this class to not listen so you
      // don't get into an infinite recursive loop
      listen(false);
    }    
    public void handleDebug(String msg) { Log.debug(msg); }
    public void handleInfo(String msg)  { Log.info(msg); }
    public void handleWarn(String msg)  { Log.warn(msg); }
    public void handleError(String msg) { Log.error(msg); }
    public void handleFatal(String msg) { Log.fatal(msg); }
  }
  
  public static void main(String[] args)
  {
    // Initialize the JNI bridge between Java and C++
    // While not required (yet), it's recommended for program completeness
    jniBridge.initialize();
    example();
    // Shutdown all C++ classes
    // Since Pulse supports running multiple engines within the same process
    // (Each could be on its own thread as well)
    // deinitialize the C++ so it can properly clean up resources
    // Generally this is not needed, but I did find it necessary on windows using mingw
    // If not, the JVM would find itself in a threadlock condition due to threadpool deconstruction
    // So if your Java app using Pulse has shutdown issues, use this method at your apps end of life
    jniBridge.deinitialize();
  }
  
  public static void example()
  {
    // Create a log file for this example (by default, a Pulse.log will be made)
    // NOTE the engine will have its own log, so there is a Java log and an Engine log!!
    // You don't have to have a Java log, but if you want to, this is how you can do it.
    Log.setFileName("./test_results/scenarios/HowTo_StaticEngineDriver.log");
    // Note there is a single singleton logger in Java, but a log per engine in C++
    // i.e. in Java, multiple engines will write to the same log, where as in C++, each engine will write to its own log
    // The listener and callback objects are unique to this engine
    
    // Create a Pulse Engine
    PulseScenarioExec pe = new PulseScenarioExec();
        
    // I am going to create a listener that will get any log messages (Info, Warnings, Errors, Fatal Errors)
    // that come from the engine. The default listener will just put them into the log file
    // If you want to do custom logic that occurs when the engine throws an error (or any other message type), just create a class just like this one
    pe.setListener(new MyListener());
    
    // Static Execution refers to fact that the engine is only going to execute and record outputs as defined in a predefined scenario file
    // You cannot request different data from the engine, and you cannot give any new actions to the engine.
    // You provide the name of the log file to associate with running this scenario,
    // the scenario file, 
    // the name and location of a results file
    // an optional callback that will be called so you can get the latest data values and do some custom logic
    String pba = FileUtils.readFile("./verification/scenarios/patient/BasicStandard.pba");
    pe.runScenario("./test_results/scenarios/patient/BasicStandard.log", pba,"./test_results/scenarios/patient/BasicStandardResults.csv", null);// No Callback, just write out the file
    
    // You could create and provide an SEScenario object as well
    SEScenario sce = new SEScenario(pe.substanceManager);
    sce.setName("HowTo_StaticEngine");
    sce.setDescription("Simple Scenario to demonstraight building a scenario by the CDM API");
    sce.getInitialParameters().setPatientFile("Standard.pba");
    // When filling out a data request, units are optional
    // The units will be set to whatever units the engine uses.
    SEDataRequest hr = new SEDataRequest();
    hr.setCategory(eDataRequest.Category.Physiology);
    hr.setPropertyName("HeartRate");
    hr.setUnit(FrequencyUnit.Per_min.toString());
    sce.getDataRequestManager().getRequestedData().add(hr);
    SEDataRequest rr = new SEDataRequest();
    rr.setCategory(eDataRequest.Category.Physiology);
    rr.setPropertyName("RespirationRate");
    rr.setUnit(FrequencyUnit.Per_min.toString());
    sce.getDataRequestManager().getRequestedData().add(rr);
    SEDataRequest tlv = new SEDataRequest();   
    tlv.setCategory(eDataRequest.Category.Physiology); 
    tlv.setPropertyName("TotalLungVolume");
    tlv.setUnit(VolumeUnit.mL.toString());
    sce.getDataRequestManager().getRequestedData().add(tlv);
    // Let's just run for 2 minutes
    SEAdvanceTime adv = new SEAdvanceTime();
    adv.getTime().setValue(2,TimeUnit.min);
    sce.getActions().add(adv);
    pe.runScenario("./test_results/scenarios/HowToStaticEngine.log",sce,"./test_results/scenarios/HowToStaticEngineResults.pba", new MyCallback(pe,1.0));// Callback with new data every simulated second
    
    // Note if your engine is running in another thread, you can call pe.cancelScenario to halt the engine
    
    // Be nice to your memory and deallocate the native memory associated with this engine if you are done with it
    pe.cleanUp();
    // Note you can now run a static (scenario) or another dynamic engine with the pe object, it will allocate and manage a new C++ engine 
  }
}
