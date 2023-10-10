# Always Install pulse python files, there is pure python functionality we use in development
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/python/pulse DESTINATION ${CMAKE_INSTALL_PREFIX}/python)

if(NOT Pulse_PYTHON_API)
  return()
endif()
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(PyPulse
  pybind/PyPulse.cpp
  pybind/PulseEngines.cpp
  pybind/MultiplexVentilation.cpp)

target_include_directories(PyPulse PUBLIC ${CMAKE_SOURCE_DIR}/src/cpp)
target_include_directories(PyPulse PRIVATE ${pybind11_INCLUDE_DIRS} ${DST_ROOT}/cpp)

if(APPLE)
    set_target_properties(PyPulse PROPERTIES MACOSX_RPATH ON)
endif()

target_link_libraries(PyPulse
                      PUBLIC MultiplexVentilationEngine pybind11::module
)

set_target_properties(PyPulse PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET PyPulse POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PyPulse> ${CMAKE_INSTALL_PREFIX}/bin)

install(TARGETS PyPulse
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

set_target_properties (PyPulse PROPERTIES FOLDER ${PROJECT_NAME})

# Install protobuf python support, we don't get this unless python api is enabled
install(DIRECTORY ${CMAKE_BINARY_DIR}/src/python/pulse DESTINATION ${CMAKE_INSTALL_PREFIX}/python)
install(DIRECTORY ${CMAKE_BINARY_DIR}/src/python/google DESTINATION ${CMAKE_INSTALL_PREFIX}/python)
