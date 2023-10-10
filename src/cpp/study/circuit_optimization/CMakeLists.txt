add_library_ex(CircuitOptimizationEngine
  H_FILES
    CircuitOptimizer.h
  CPP_FILES
    CircuitOptimizer.cpp
  PUBLIC_DEPENDS
    Pulse
  INSTALL_HEADER_DIR
    study/circuit_optimization
  NO_INSTALL
  )
set_target_properties(CircuitOptimizationEngine PROPERTIES FOLDER "Studies//Circuit Optimization")
target_include_directories(CircuitOptimizationEngine PRIVATE ${EIGEN3_INCLUDE_DIR})

add_executable_ex(CircuitOptimizationDriver main.cpp)
set_target_properties(CircuitOptimizationDriver PROPERTIES FOLDER "Studies//Circuit Optimization")
target_link_libraries(CircuitOptimizationDriver CircuitOptimizationEngine protobuf::libprotobuf)