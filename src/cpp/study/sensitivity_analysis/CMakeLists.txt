add_library_ex(SensitivityAnalysisEngine STATIC
  H_FILES
    SARunner.h
  CPP_FILES
    SARunner.cpp
  PUBLIC_DEPENDS
    Pulse
  INSTALL_HEADER_DIR
    study/sensitivity_analysis
  NO_INSTALL
  )
set_target_properties(SensitivityAnalysisEngine PROPERTIES FOLDER "Studies//SensitivityAnalysis")

add_executable_ex(SensitivityAnalysisDriver main.cpp)
set_target_properties(SensitivityAnalysisDriver PROPERTIES FOLDER "Studies//SensitivityAnalysis")
target_link_libraries(SensitivityAnalysisDriver SensitivityAnalysisEngine protobuf::libprotobuf)