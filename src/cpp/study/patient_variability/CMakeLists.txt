add_library_ex(PatientVariabilityEngine
  H_FILES
    PVGenerator.h
    PVRunner.h
  CPP_FILES
    PVGenerator.cpp
    PVRunner.cpp
  PUBLIC_DEPENDS
    Pulse
  INSTALL_HEADER_DIR
    study/patient_variability
  NO_INSTALL
  )
set_target_properties(PatientVariabilityEngine PROPERTIES FOLDER "Studies//PatientVariability")

add_executable_ex(PatientVariabilityDriver main.cpp)
set_target_properties(PatientVariabilityDriver PROPERTIES FOLDER "Studies//PatientVariability")
target_link_libraries(PatientVariabilityDriver PatientVariabilityEngine protobuf::libprotobuf)