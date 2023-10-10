set(_c_lib_type SHARED)
if(${PROJECT_NAME}_C_AS_STATIC)
  set(_c_lib_type STATIC)
endif()

add_library_ex(PulseC ${_c_lib_type}
  CPP_FILES
    PulseEngineC.cpp
  PUBLIC_DEPENDS
    Pulse
  #VERBOSE
)
set_target_properties (PulseC PROPERTIES FOLDER ${PROJECT_NAME})
