﻿/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_MechanicalVentilator
{
  class Example
  {
    public static void Run()
    {
      // Create our pulse engine
      PulseEngine pulse = new PulseEngine();
      pulse.LogToConsole(true);// Easily view what is happening

      List<SEDataRequest> data_requests = new List<SEDataRequest>
      {
        // Vitals Monitor Data
        SEDataRequest.CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min),
        SEDataRequest.CreatePhysiologyDataRequest("TidalVolume", VolumeUnit.mL),
        SEDataRequest.CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL),
        SEDataRequest.CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit.cmH2O_s_Per_L),
        SEDataRequest.CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit.cmH2O_s_Per_L),
        SEDataRequest.CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit.L_Per_cmH2O),
        SEDataRequest.CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit.L_Per_min),
        // Ventilator Monitor Data
        SEDataRequest.CreateMechanicalVentilatorDataRequest("AirwayPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("BreathState"),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxideFraction"),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxidePressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("EndTidalOxygenFraction"),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("EndTidalOxygenPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("ExpiratoryFlow", VolumePerTimeUnit.L_Per_s),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("ExpiratoryTidalVolume", VolumeUnit.L),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("InspiratoryExpiratoryRatio"),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("InspiratoryFlow", VolumePerTimeUnit.L_Per_s),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("InspiratoryTidalVolume", VolumeUnit.L),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("IntrinsicPositiveEndExpiredPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("LeakFraction"),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("MeanAirwayPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("PlateauPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("PositiveEndExpiratoryPressure", PressureUnit.cmH2O),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("RespirationRate", FrequencyUnit.Per_min),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("TidalVolume", VolumeUnit.L),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("TotalLungVolume", VolumeUnit.L),
        SEDataRequest.CreateMechanicalVentilatorDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit.L_Per_s),
      };
      SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
      data_mgr.SetResultsFilename("./test_results/howto/HowTo_MechanicalVentilator.cs.csv");
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      if (!pulse.SerializeFromFile("./states/StandardMale@0s.pbb", data_mgr))
      {
        Console.WriteLine("Error Initializing Pulse!");
        return;
      }
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      // Note BreathState is an enum, so we can cast that
      Console.WriteLine("Breath State "+ Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      // Give the patient Dyspnea
      SEDyspnea dyspnea = new SEDyspnea();
      dyspnea.GetSeverity().SetValue(1.0);
      pulse.ProcessAction(dyspnea);
      pulse.AdvanceTime_s(10);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);

      // We have action support for several commonly used ventilator modes
      // Pulse is not limited to these modes, These modes are designe for simple understanding
      // Our implementation supports any ventilator mode, you will just need to translate the user facing inputs
      // to a timing/control profile using our configuration settings
      // These modes are internally converted into a configuration setting,
      // You can retrieve the resulting settings action and use it as a basis for more configurations if you want.
      // For example, if you wanted to lengthen the InspirationPatientTriggerFlow of the mode

      SEMechanicalVentilatorContinuousPositiveAirwayPressure cpap = new SEMechanicalVentilatorContinuousPositiveAirwayPressure();
      cpap.SetConnection(eSwitch.On);
      cpap.GetFractionInspiredOxygen().SetValue(0.21);
      cpap.GetDeltaPressureSupport().SetValue(8.0, PressureUnit.cmH2O);
      cpap.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit.cmH2O);
      cpap.GetSlope().SetValue(0.2, TimeUnit.s);
      pulse.ProcessAction(cpap);
      pulse.AdvanceTime_s(10);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      SEMechanicalVentilatorPressureControl pc_ac = new SEMechanicalVentilatorPressureControl();
      pc_ac.SetConnection(eSwitch.On);
      pc_ac.SetMode(eMechanicalVentilator_PressureControlMode.AssistedControl);
      pc_ac.GetFractionInspiredOxygen().SetValue(0.21);
      pc_ac.GetInspiratoryPeriod().SetValue(1.0,TimeUnit.s);
      pc_ac.GetInspiratoryPressure().SetValue(13.0, PressureUnit.cmH2O);
      pc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit.cmH2O);
      pc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit.Per_min);
      pc_ac.GetSlope().SetValue(0.1, TimeUnit.s);
      pulse.ProcessAction(pc_ac);
      pulse.AdvanceTime_s(10);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      SEMechanicalVentilatorVolumeControl vc_ac = new SEMechanicalVentilatorVolumeControl();
      vc_ac.SetConnection(eSwitch.On);
      vc_ac.SetMode(eMechanicalVentilator_VolumeControlMode.AssistedControl);
      vc_ac.GetFlow().SetValue(50.0, VolumePerTimeUnit.L_Per_min);
      vc_ac.GetFractionInspiredOxygen().SetValue(0.21);
      vc_ac.GetInspiratoryPeriod().SetValue(1.0, TimeUnit.s);
      vc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit.cmH2O);
      vc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit.Per_min);
      vc_ac.GetTidalVolume().SetValue(600.0, VolumeUnit.mL);
      pulse.ProcessAction(vc_ac);
      pulse.AdvanceTime_s(10);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      // Here is an example of programming a custom ventilator mode
      SEMechanicalVentilatorConfiguration mv_config = new SEMechanicalVentilatorConfiguration();
      SEMechanicalVentilatorSettings mv = mv_config.GetSettings();
      mv.SetConnection(eSwitch.On);
      mv.SetInspirationWaveform(eDriverWaveform.Square);
      mv.SetExpirationWaveform(eDriverWaveform.Square);
      mv.GetPeakInspiratoryPressure().SetValue(21.0, PressureUnit.cmH2O);
      mv.GetPositiveEndExpiredPressure().SetValue(10.0, PressureUnit.cmH2O);
      SESubstanceFraction fractionFiO2 = mv.GetFractionInspiredGas("Oxygen");
      fractionFiO2.GetFractionAmount().SetValue(0.5);
      double respirationRate_per_min = 20.0;
      double IERatio = 0.5;
      // Translate ventilator settings
      double totalPeriod_s = 60.0 / respirationRate_per_min;
      double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
      double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
      mv.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit.s);
      mv.GetExpirationCycleTime().SetValue(inspiratoryPeriod_s, TimeUnit.s);
      pulse.ProcessAction(mv_config);
      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(10);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      // You can also perform a hold
      SEMechanicalVentilatorHold hold = new SEMechanicalVentilatorHold();
      hold.SetState(eSwitch.On);
      pulse.ProcessAction(hold);
      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(5);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      hold.SetState(eSwitch.Off);
      pulse.ProcessAction(hold);
      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(5);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));

      // A leak can be specified
      SEMechanicalVentilatorLeak leak = new SEMechanicalVentilatorLeak();
      leak.GetSeverity().SetValue(0.5);
      pulse.ProcessAction(leak);
      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(5);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      leak.GetSeverity().SetValue(0.0);// Turn off the leak
      pulse.ProcessAction(leak);
      // Advance some time and print out the vitals
      pulse.AdvanceTime_s(5);
      // Get the values of the data you requested at this time
      data_values = pulse.PullData();
      // And write it out to the console
      data_mgr.WriteData(data_values);
      Console.WriteLine("Breath State " + Enum.GetName(typeof(eBreathState), (eBreathState)data_values[9]));
    }
  }
}
