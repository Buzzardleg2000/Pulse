add_library_ex(PulseEngine
  H_FILES
    PulseConfiguration.h
    PulseEngine.h
    PulseEnginePool.h
    PulseEngineThunk.h
    PulseScenario.h
    PulseScenarioExec.h
    # Common
    CommonDefs.h
    common/Engine.h
    common/controller/BlackBoxManager.h
    common/controller/CircuitManager.h
    common/controller/CompartmentManager.h
    common/controller/Controller.h
    common/controller/SubstanceManager.h
    common/system/Model.h
    common/system/Systems.h
    common/system/environment/EnvironmentModel.h
    common/system/equipment/AnesthesiaMachineModel.h
    common/system/equipment/BagValveMaskModel.h
    common/system/equipment/ECMOModel.h
    common/system/equipment/ElectroCardioGramModel.h
    common/system/equipment/InhalerModel.h
    common/system/equipment/MechanicalVentilatorModel.h
    common/system/physiology/BloodChemistryModel.h
    common/system/physiology/CardiovascularModel.h
    common/system/physiology/DrugModel.h
    common/system/physiology/EndocrineModel.h
    common/system/physiology/EnergyModel.h
    common/system/physiology/GastrointestinalModel.h
    common/system/physiology/HepaticModel.h
    common/system/physiology/NervousModel.h
    common/system/physiology/RenalModel.h
    common/system/physiology/RespiratoryModel.h
    common/system/physiology/Saturation.h
    common/system/physiology/TissueModel.h
    # Human Adult
    #  Whole Body
    human_adult/whole_body/Engine.h
    human_adult/whole_body/controller/BlackBoxManager.h
    human_adult/whole_body/controller/CircuitManager.h
    human_adult/whole_body/controller/CompartmentManager.h
    human_adult/whole_body/controller/Controller.h
    human_adult/whole_body/controller/SubstanceManager.h
    human_adult/whole_body/system/environment/EnvironmentModel.h
    human_adult/whole_body/system/equipment/AnesthesiaMachineModel.h
    human_adult/whole_body/system/equipment/BagValveMaskModel.h
    human_adult/whole_body/system/equipment/ECMOModel.h
    human_adult/whole_body/system/equipment/ElectroCardioGramModel.h
    human_adult/whole_body/system/equipment/InhalerModel.h
    human_adult/whole_body/system/equipment/MechanicalVentilatorModel.h
    human_adult/whole_body/system/physiology/BloodChemistryModel.h
    human_adult/whole_body/system/physiology/CardiovascularModel.h
    human_adult/whole_body/system/physiology/DrugModel.h
    human_adult/whole_body/system/physiology/EndocrineModel.h
    human_adult/whole_body/system/physiology/EnergyModel.h
    human_adult/whole_body/system/physiology/GastrointestinalModel.h
    human_adult/whole_body/system/physiology/HepaticModel.h
    human_adult/whole_body/system/physiology/NervousModel.h
    human_adult/whole_body/system/physiology/RenalModel.h
    human_adult/whole_body/system/physiology/RespiratoryModel.h
    human_adult/whole_body/system/physiology/Saturation.h
    human_adult/whole_body/system/physiology/TissueModel.h
    #  Hemodynamics
    human_adult/hemodynamics/Engine.h
    human_adult/hemodynamics/controller/CompartmentManager.h
    human_adult/hemodynamics/controller/Controller.h
    #  Ventilation Mechanics
    human_adult/ventilation_mechanics/Engine.h
    human_adult/ventilation_mechanics/controller/CompartmentManager.h
    human_adult/ventilation_mechanics/controller/Controller.h
    # Serialization I/O
    io/protobuf/PBConfiguration.h
    io/protobuf/PBEnvironment.h
    io/protobuf/PBEquipment.h
    io/protobuf/PBPhysiology.h
    io/protobuf/PBScenario.h
    io/protobuf/PBState.h
  CPP_FILES
    PulseConfiguration.cpp
    PulseEnginePool.cpp
    PulseEngineThunk.cpp
    PulseScenario.cpp
    PulseScenarioExec.cpp
    # Common
    common/Engine.cpp
    common/controller/BlackBoxManager.cpp
    common/controller/CircuitManager.cpp
    common/controller/CompartmentManager.cpp
    common/controller/Controller.cpp
    common/controller/SetupCircuitsAndCompartments.cpp
    common/controller/SetupExpandedCardiovascular.cpp
    common/controller/SubstanceManager.cpp
    common/system/environment/EnvironmentModel.cpp
    common/system/equipment/AnesthesiaMachineModel.cpp
    common/system/equipment/BagValveMaskModel.cpp
    common/system/equipment/ECMOModel.cpp
    common/system/equipment/ElectroCardioGramModel.cpp
    common/system/equipment/InhalerModel.cpp
    common/system/equipment/MechanicalVentilatorModel.cpp
    common/system/physiology/BloodChemistryModel.cpp
    common/system/physiology/CardiovascularModel.cpp
    common/system/physiology/DrugModel.cpp
    common/system/physiology/EndocrineModel.cpp
    common/system/physiology/EnergyModel.cpp
    common/system/physiology/GastrointestinalModel.cpp
    common/system/physiology/HepaticModel.cpp
    common/system/physiology/NervousModel.cpp
    common/system/physiology/RenalModel.cpp
    common/system/physiology/RespiratoryModel.cpp
    common/system/physiology/Saturation.cpp
    common/system/physiology/TissueModel.cpp
    # Human Adult
    #  Whole Body
    human_adult/whole_body/Engine.cpp
    human_adult/whole_body/controller/Controller.cpp
    human_adult/whole_body/controller/SetupPatient.cpp
    #  Hemodynamics
    human_adult/hemodynamics/Engine.cpp
    human_adult/hemodynamics/controller/CompartmentManager.cpp
    human_adult/hemodynamics/controller/Controller.cpp
    human_adult/hemodynamics/controller/SetupCircuitsAndCompartments.cpp
    #  Ventilation Mechanics
    human_adult/ventilation_mechanics/Engine.cpp
    human_adult/ventilation_mechanics/controller/CompartmentManager.cpp
    human_adult/ventilation_mechanics/controller/Controller.cpp
    human_adult/ventilation_mechanics/controller/SetupCircuitsAndCompartments.cpp
    # Serialization I/O
    io/protobuf/PBConfiguration.cpp
    io/protobuf/PBEnums.cpp
    io/protobuf/PBEnvironment.cpp
    io/protobuf/PBEquipment.cpp
    io/protobuf/PBPhysiology.cpp
    io/protobuf/PBScenario.cpp
    io/protobuf/PBState.cpp
  CONFIG_CPP_FILES
    PulseBuildInformation.cpp
  PUBLIC_DEPENDS
    CommonDataModel
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  INSTALL_HEADER_DIR
    engine
)
target_include_directories(PulseEngine PRIVATE ${EIGEN3_INCLUDE_DIR})

add_library_ex(PulseEngineTests
  H_FILES
    # Human Adult
    #  Whole Body
    human_adult/whole_body/test/EngineTest.h
    #  Ventilation Mechanics
    human_adult/ventilation_mechanics/test/EngineTest.h
  CPP_FILES
    # Human Adult
    #  Whole Body
    human_adult/whole_body/test/AcidBaseTests.cpp
    human_adult/whole_body/test/AerosolTest.cpp
    human_adult/whole_body/test/AnesthesiaCircuit.cpp
    human_adult/whole_body/test/BlackBoxTests.cpp
    human_adult/whole_body/test/CardiovascularCircuit.cpp
    human_adult/whole_body/test/ConditionsTest.cpp
    human_adult/whole_body/test/EnginePoolTests.cpp
    human_adult/whole_body/test/EngineTest.cpp
    human_adult/whole_body/test/FourCompartmentTest.cpp
    human_adult/whole_body/test/MechanicalVentilatorCircuit.cpp
    human_adult/whole_body/test/NervousTests.cpp
    human_adult/whole_body/test/ReadScenarios.cpp
    human_adult/whole_body/test/RenalCircuit.cpp
    human_adult/whole_body/test/RespiratoryCircuit.cpp
    human_adult/whole_body/test/ReuseEngine.cpp
    human_adult/whole_body/test/SerializationTest.cpp
    human_adult/whole_body/test/SetupPatientTest.cpp
    human_adult/whole_body/test/SolverTests.cpp
    human_adult/whole_body/test/TemperatureCircuit.cpp
    human_adult/whole_body/test/TissueTests.cpp
    #  Ventilation Mechanics
    human_adult/ventilation_mechanics/test/EngineTest.cpp
  PUBLIC_DEPENDS
    PulseEngine
  LIB_INSTALL_ONLY
)

