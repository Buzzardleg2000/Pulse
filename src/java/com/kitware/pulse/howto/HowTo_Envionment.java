package com.kitware.pulse.howto;

import java.util.List;

import com.kitware.pulse.cdm.engine.SEDataRequestManager;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.system.environment.actions.SEChangeEnvironmentalConditions;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TemperatureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;

public class HowTo_Envionment
{
  protected static class MyListener extends LogListener
  {
    protected MyListener()
    {
      super();
      listen(false);
    }    
    @Override public void handleDebug(String msg) { Log.debug(msg); }
    @Override public void handleInfo(String msg)  { Log.info(msg); }
    @Override public void handleWarn(String msg)  { Log.warn(msg); }
    @Override public void handleError(String msg) { Log.error(msg); }
    @Override public void handleFatal(String msg) { Log.fatal(msg); }
  }

  public static void main(String[] args)
  {
    List<Double> dataValues;
    
    SEScalarTime time = new SEScalarTime(10,TimeUnit.min);

    JNIBridge.initialize();
    PulseEngine pe = new PulseEngine();
    pe.setLogListener(new MyListener());
    pe.setLogFilename("./test_results/howto/HowTo_Environment.java.log");

    SEDataRequestManager dataRequests = new SEDataRequestManager();
    dataRequests.createPhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL);
    dataRequests.createPhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("CoreTemperature", TemperatureUnit.F);
    dataRequests.createPhysiologyDataRequest("SkinTemperature", TemperatureUnit.F);

    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    
    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);

    SEChangeEnvironmentalConditions env = new SEChangeEnvironmentalConditions();
    env.setEnvironmentalConditionsFile("./environments/AnchorageDecember.json");
    pe.processAction(env);

    pe.advanceTime(time);

    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);
  }

}
