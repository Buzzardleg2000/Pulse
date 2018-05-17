# Files in the project (Relative to this CMAKE file)
file(GLOB CDM_TEST_FILES
  "cdm/cpp/*.h"
  "cdm/cpp/*.cpp"
)
set(SOURCE ${CDM_TEST_FILES})
source_group("" FILES ${CDM_TEST_FILES})

# The DLL we are building
add_library(CommonDataModelUnitTests ${SOURCE})
# Preprocessor Definitions and Include Paths
set(CDM_TEST_FLAGS)
if(${BUILD_SHARED_LIBS}) 
  set(CDM_TEST_FLAGS "${CDM_TEST_FLAGS} -D SHARED_CDM_TEST")
endif()
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp)
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(CommonDataModelUnitTests PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(CommonDataModelUnitTests PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(CommonDataModelUnitTests PRIVATE ${PROTOBUF_INCLUDE_DIR})
set_target_properties(CommonDataModelUnitTests PROPERTIES COMPILE_FLAGS "${CDM_TEST_FLAGS}" PREFIX "")

IF(APPLE)
    set_target_properties(CommonDataModelUnitTests PROPERTIES MACOSX_RPATH ON)
ENDIF()

# Dependent Libraries
target_link_libraries(CommonDataModelUnitTests CommonDataModel)

if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET CommonDataModelUnitTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_BIN}/${CONFIGURATION}
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CommonDataModelUnitTests> ${INSTALL_BIN}/${CONFIGURATION})

  if(WIN32)# Copy dll files to the bin
    install(TARGETS CommonDataModelUnitTests 
            RUNTIME CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release
            LIBRARY CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release)
    install(TARGETS CommonDataModelUnitTests 
            RUNTIME CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug
            LIBRARY CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug)
    install(TARGETS CommonDataModelUnitTests 
            RUNTIME CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo
            LIBRARY CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo)
  else()# Copy so files to the bin
    install(TARGETS CommonDataModelUnitTests 
            LIBRARY CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release)
    install(TARGETS CommonDataModelUnitTests 
            LIBRARY CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug)
    install(TARGETS CommonDataModelUnitTests 
            LIBRARY CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo)
  endif()
endif()
