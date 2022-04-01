/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"

using namespace pulse;

namespace pulse::study::patient_variability
{
  PVRunner::PVRunner(const std::string& logfileName, const std::string& dataDir) : Loggable(logfileName)
  {
    m_DataDir = dataDir;
    m_SimulationList = nullptr;
    m_SimulationResultsList = nullptr;
  }
  PVRunner::~PVRunner()
  {
    SAFE_DELETE(m_SimulationList);
    SAFE_DELETE(m_SimulationResultsList);
  }

  bool PVRunner::Run(pulse::study::bind::patient_variability::SimulationListData& simList, const std::string& resultsFilename)
  {
    m_SimulationResultsListFile = simList.outputrootdir() + "/" + resultsFilename;
    SAFE_DELETE(m_SimulationList);
    SAFE_DELETE(m_SimulationResultsList);
    m_SimulationList = &simList;
    m_SimulationResultsList = new pulse::study::bind::patient_variability::SimulationListData();
    if (FileExists(m_SimulationResultsListFile))
    {
      if (!SerializeFromFile(m_SimulationResultsListFile, *m_SimulationResultsList, eSerializationFormat::JSON))
      {
        GetLogger()->Warning("Unable to read found results file");
      }
    }
    bool b = Run();
    m_SimulationList = nullptr;
    SAFE_DELETE(m_SimulationResultsList);
    return b;
  }

  bool PVRunner::Run(const std::string& filename, eSerializationFormat f)
  {
    SAFE_DELETE(m_SimulationList);
    SAFE_DELETE(m_SimulationResultsList);
    m_SimulationList = new pulse::study::bind::patient_variability::SimulationListData();
    m_SimulationResultsList = new pulse::study::bind::patient_variability::SimulationListData();

    if (!SerializeFromFile(filename, *m_SimulationList, eSerializationFormat::JSON))
      return false;
    // Let's try to read in a results file
    m_SimulationResultsListFile = filename.substr(0, filename.length() - 5) + "_results.json";
    if (FileExists(m_SimulationResultsListFile))
    {
      if (!SerializeFromFile(m_SimulationResultsListFile, *m_SimulationResultsList, eSerializationFormat::JSON))
      {
        GetLogger()->Warning("Unable to read found results file");
      }
    }
    return Run();
  }
  bool PVRunner::Run()
  {
    TimingProfile profiler;
    profiler.Start("Total");

    // Get the ID's of simulations we need to run
    m_SimulationsToRun.clear();
    for (int i = 0; i < m_SimulationList->simulations_size(); i++)
      m_SimulationsToRun.insert(m_SimulationList->simulations()[i].id());
    // Remove any id's we have in the results
    if (m_SimulationResultsList->simulations_size() > 0)
    {
      Info("Found " + std::to_string(m_SimulationResultsList->simulations_size()) + " previously run simulations");
      for (int i = 0; i < m_SimulationResultsList->simulations_size(); i++)
        m_SimulationsToRun.erase(m_SimulationResultsList->simulations()[i].id());
    }

    size_t numSimsToRun = m_SimulationList->simulations_size() - m_SimulationResultsList->simulations_size();
    if (numSimsToRun == 0)
    {
      Info("All simulations are run in the results file");
      return true;
    }
    size_t processor_count = std::thread::hardware_concurrency();
    if (processor_count == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }
    // Let's not kill the computer this is running on...
    if (processor_count > 1)
      processor_count -= 1;
    // Let's not kick off more threads than we need
    if (processor_count > numSimsToRun)
      processor_count = numSimsToRun;
    Info("Starting " + std::to_string(processor_count) + " Threads to run " + std::to_string(m_SimulationsToRun.size()) + " simulations");
    // Crank up our threads
    for (size_t p = 0; p < processor_count; p++)
      m_Threads.push_back(std::thread(&PVRunner::ControllerLoop, this));

    for (size_t p = 0; p < processor_count; p++)
      m_Threads[p].join();

    Info("It took " + std::to_string(profiler.GetElapsedTime_s("Total")) + "s to run this simulation list");
    profiler.Clear();
    return true;
  }

  void PVRunner::ControllerLoop()
  {
    pulse::study::bind::patient_variability::SimulationData* sim = nullptr;
    while (true)
    {
      try
      {
        sim = GetNextSimulation();
        if (sim == nullptr)
          break;
        RunSimulationUntilStable(*sim);
        FinalizeSimulation(*sim);
      }
      catch (CommonDataModelException& cdm_ex)
      {
        GetLogger()->Fatal("Exception caught runnning simulation " + sim->outputbasefilename());
        GetLogger()->Fatal(cdm_ex.what());
        std::cerr << cdm_ex.what() << std::endl;
      }
      catch (std::exception& ex)
      {
        GetLogger()->Fatal("Exception caught runnning simulation " + sim->outputbasefilename());
        GetLogger()->Fatal(ex.what());
        std::cerr << ex.what() << std::endl;
      }
      catch (...)
      {
        std::cerr << "Unable to run simulation " << sim->outputbasefilename() << std::endl;
      }
    }
  }

  bool PVRunner::RunSimulationUntilStable(pulse::study::bind::patient_variability::SimulationData& sim, const std::string& dataDir)
  {
    TimingProfile profiler;
    profiler.Start("Total");
    profiler.Start("Status");

    auto pulse = CreatePulseEngine();
    pulse->GetLogger()->SetLogFile(sim.outputbasefilename() + ".log");

    // No logging to console (when threaded)
    pulse->GetLogger()->LogToConsole(false);
    // Setup data requests
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("IntracranialPressure", PressureUnit::mmHg);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CerebralPerfusionPressure", PressureUnit::mmHg);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Brain, "Pressure", PressureUnit::mmHg);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Brain, "Volume", VolumeUnit::mL);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Brain, "InFlow", VolumePerTimeUnit::mL_Per_min);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Brain, "Oxygen", "PartialPressure");
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::CerebrospinalFluidCompartment::IntracranialSpace, "Volume", VolumeUnit::mL);
    //pulse->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::CerebrospinalFluidCompartment::IntracranialSpace, "Pressure", PressureUnit::mmHg);
    //pulse->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(outDir + "/" + std::to_string(sim.id()) + " - " + sim.name() + ".csv");

    // Stabilize the engine
    SEPatientConfiguration pc;
    pc.SetPatientFile("./patients/StandardMale.json");
    if (!pulse->InitializeEngine(pc))
    {
      sim.set_achievedstabilization(false);
      sim.set_stabilizationtime_s(profiler.GetElapsedTime_s("Total"));
      return false;
    }

    pulse->GetLogger()->Info("It took " + cdm::to_string(sim.stabilizationtime_s()) + "s to run this simulation");

    profiler.Clear();
    return true;
  }

  pulse::study::bind::patient_variability::SimulationData* PVRunner::GetNextSimulation()
  {
    m_mutex.lock();
    pulse::study::bind::patient_variability::SimulationData* sim = nullptr;
    if (!m_SimulationsToRun.empty())
    {
      auto id = *m_SimulationsToRun.begin();
      for (int i = 0; i < m_SimulationList->simulations_size(); i++)
      {
        sim = &(*m_SimulationList->mutable_simulations())[i];
        if (sim->id() == id)
          break;
      }
      Info("Simulating Run " + std::to_string(id));
      m_SimulationsToRun.erase(id);
    }
    m_mutex.unlock();
    return sim;
  }

  void PVRunner::FinalizeSimulation(pulse::study::bind::patient_variability::SimulationData& sim)
  {
    m_mutex.lock();
    auto rSim = m_SimulationResultsList->mutable_simulations()->Add();
    rSim->CopyFrom(sim);
    SerializeToFile(*m_SimulationResultsList, m_SimulationResultsListFile, eSerializationFormat::JSON);
    Info("Completed Simulation " + std::to_string(m_SimulationResultsList->simulations_size()) + " of " + std::to_string(m_SimulationList->simulations_size()));
    if (sim.achievedstabilization())
      Info("  Stabilized Run " + std::to_string(sim.id()));
    else
      Info("  FAILED STABILIZATION FOR RUN " + std::to_string(sim.id()));
    std::ofstream plots;
    plots.open(m_SimulationList->outputrootdir() + "/plot_pairs.config", std::fstream::in | std::fstream::out | std::fstream::app);
    plots << sim.outputbasefilename() << "multiplex_patient_0_results.csv, " << sim.outputbasefilename() << "multiplex_patient_1_results.csv\n";
    plots.close();
    m_mutex.unlock();
  }

  bool PVRunner::SerializeToString(pulse::study::bind::patient_variability::SimulationListData& src, std::string& output, eSerializationFormat f) const
  {
    google::protobuf::util::JsonPrintOptions printOpts;
    printOpts.add_whitespace = true;
    printOpts.preserve_proto_field_names = true;
    printOpts.always_print_primitive_fields = true;
    if (!google::protobuf::util::MessageToJsonString(src, &output, printOpts).ok())
    {
      Error("Unable to serialize simulation list");
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeToFile(pulse::study::bind::patient_variability::SimulationListData& src, const std::string& filename, eSerializationFormat f) const
  {
    return PBUtils::SerializeToFile(src, filename, GetLogger());
  }
  bool PVRunner::SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::SimulationListData& dst, eSerializationFormat f)
  {
    google::protobuf::util::JsonParseOptions parseOpts;
    google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
    {
      std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
    });
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
    if (!stat.ok())
    {
      Error("Unable to parse json in string : " + stat.ToString());
      return false;
    }
    return true;
  }
  bool PVRunner::SerializeFromFile(const std::string& filename, pulse::study::bind::patient_variability::SimulationListData& dst, eSerializationFormat f)
  {
    return PBUtils::SerializeFromFile(filename, dst, GetLogger());
  }
}