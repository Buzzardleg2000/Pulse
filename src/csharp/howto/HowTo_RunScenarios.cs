﻿/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_RunScenarios
{
  class Example
  {
    public static void Run()
    {
      // Note, you don't need a RunConfiguration, if you know where your files all are
      // This is a convienance utility for this example to use a scenario from our repository
      // This class informs me where the repository is on your computer (generated by the CMake build)
      RunConfiguration cfg = new RunConfiguration();
      SEScenarioExec execOpts = new SEScenarioExec();
      execOpts.SetDataRequestCSVFilename("");// Set to blank if you do not want a csv file
      //execOpts.SetDataRequestCSVFilename("./test_results/scenarios/patient/BasicStandardResults.csv");
      execOpts.SetLogFilename("./test_results/scenarios/patient/BasicStandardResults.log");
      execOpts.SetScenarioFilename(cfg.getScenarioDirectory() + "/patient/BasicStandard.json");
      // Where do you want any states created by scenarios to go?
      execOpts.SetSerializationDirectory("./states/");
      PulseEngine pulse = new PulseEngine();
      if(!pulse.ExecuteScenario(execOpts))
        System.Console.Out.WriteLine("Error running scenario");
    }
  }
}
