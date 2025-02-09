cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /EHsc")
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")
endif()

if(NOT CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(STATUS "Setting install dir inside build dir ${CMAKE_BINARY_DIR} ")
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install location" FORCE)
else()
  message(STATUS "Using preset install directory ${CMAKE_INSTALL_PREFIX}")
endif()

#------------------------------------------------------------------------------
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_DEBUG_POSTFIX "d")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Pulse VERSION 4.2.0 LANGUAGES C CXX)
string(TOLOWER ${PROJECT_NAME} project_name)
include(GNUInstallDirs)

#-----------------------------------------------------------------------------
# Project install directories
#-----------------------------------------------------------------------------
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install location" FORCE)
endif()
set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
# Let's go ahead and make these directories
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)

#-----------------------------------------------------------------------------
# Update CMake module path & cmake dir
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/utilities
    ${CMAKE_INSTALL_PREFIX}
    ${CMAKE_INSTALL_PREFIX}/lib/cmake # Many packages install their cmake to lib
    )
set(${PROJECT_NAME}_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# C_AS_STATIC builds a Pulse library for use in iOS builds
# AS_SHARED builds a Pulse library for use by Unreal

option(${PROJECT_NAME}_AS_SHARED "Build Pulse as a shared library (for use in Unreal builds)" OFF)
option(${PROJECT_NAME}_C_AS_STATIC "Build PulseC as a static library (for use in iOS/WASM builds)" OFF)
option(${PROJECT_NAME}_DOWNLOAD_BASELINES "Download all V&V Scenarios and their baseline result files" OFF)
option(${PROJECT_NAME}_CSHARP_API "Build C# API" OFF)
option(${PROJECT_NAME}_JAVA_API "Build Java API and utils for data generation and testing" ON)
option(${PROJECT_NAME}_PYTHON_API "Build Python API and utils for data generation and testing" ON)
option(${PROJECT_NAME}_DEPENDENT_BUILD "Pulse is being built as part of a different superbuild" OFF)
# This dir inteded for use during cross compiling, provide the directory to the generated bindings from a native pulse build
# This will be in a <build>/Innerbuild/src/schema
set(${PROJECT_NAME}_NATIVE_BUILD_DIR "" CACHE PATH "For crosscompiling, provide the root folder of a native build with protobuf bindings")
set(NATIVE_BIND_DIR "")
if(EXISTS ${${PROJECT_NAME}_NATIVE_BUILD_DIR})
  message(STATUS "Checking native bind directory...")
  set(NATIVE_BIND_DIR ${${PROJECT_NAME}_NATIVE_BUILD_DIR}/Innerbuild/src/cpp/pulse)
  if(NOT EXISTS ${NATIVE_BIND_DIR})
    message(FATAL_ERROR "Could not find expected binding directory in provided NATIVE_BUILD_DIR, expecting:  ${NATIVE_BIND_DIR}")
  endif()
endif()
# Variable configurations between a static and a shared build
set(_private_protobuf)
set(${PROJECT_NAME}_LIB_TYPE STATIC)
if (NOT ${PROJECT_NAME}_AS_SHARED)
  set(_private_protobuf protobuf::libprotobuf)
else()
  if(${PROJECT_NAME}_C_AS_STATIC)
    set(${PROJECT_NAME}_C_AS_STATIC OFF CACHE BOOL INTERNAL FORCE)
    message(WARNING "Disabling Pulse_C_AS_STATIC, as Pulse_AS_SHARED is enabled")
  endif()
  set(${PROJECT_NAME}_LIB_TYPE SHARED)
endif()

#-----------------------------------------------------------------------------
# iOS Support
#-----------------------------------------------------------------------------
if(iOS_TOOLCHAIN)
  message(STATUS "Setting up build for iOS")

  if(POLICY CMP0114)# For new xcode build system
    cmake_policy(SET CMP0114 NEW)
  endif()

  set(${PROJECT_NAME}_C_AS_STATIC ON CACHE BOOL INTERNAL FORCE)
  if(${PROJECT_NAME}_CSHARP_API)
    set(${PROJECT_NAME}_CSHARP_API OFF CACHE BOOL INTERNAL FORCE)
    message(WARNING "Disabling C# API for iOS build")
  endif()
  if(${PROJECT_NAME}_JAVA_API)
    set(${PROJECT_NAME}_JAVA_API OFF CACHE BOOL INTERNAL FORCE)
    message(WARNING "Disabling Java API for iOS build")
  endif()
  if(${PROJECT_NAME}_PYTHON_API)
    set(${PROJECT_NAME}_PYTHON_API OFF CACHE BOOL INTERNAL FORCE)
    message(WARNING "Disabling Python API for iOS build")
  endif()

  if(NOT EXISTS ${NATIVE_BIND_DIR})
    message(FATAL_ERROR "Crosscompiling for iOS, you must provide ${PROJECT_NAME}_NATIVE_BUILD_DIR")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
endif()

#-----------------------------------------------------------------------------
# SUPERBUILD
#-----------------------------------------------------------------------------
option(${PROJECT_NAME}_SUPERBUILD "Initial pull and build of all dependent libraries/executables" ON)
if(${PROJECT_NAME}_SUPERBUILD)
  #-----------------------------------------------------------------------------
  # Define External dependencies
  #-----------------------------------------------------------------------------
  macro(define_dependency extProj)
    list(APPEND ${PROJECT_NAME}_DEPENDENCIES ${extProj})
    option(USE_SYSTEM_${extProj} "Exclude ${extProj} from superbuild and use an existing build." OFF)
    mark_as_advanced(USE_SYSTEM_${extProj})
  endmacro()

  define_dependency(Eigen3)
  define_dependency(protobuf)
  message(STATUS "We are using protobuf ${Protobuf_VERSION}")
  if (Protobuf_VERSION VERSION_GREATER_EQUAL "22.0")
    message(STATUS "Added ABSL dependency")
    define_dependency(absl)
  endif()
  if(${PROJECT_NAME}_PYTHON_API)
    find_package (Python3 COMPONENTS Interpreter Development)
    if(Python3_FOUND)
      define_dependency(pybind11)
      message(STATUS "Using python: ${Python3_EXECUTABLE}")
      else()
        message(FATAL_ERROR "Unable to support Python API, cannot find Python3")
    endif()
  endif()

  if(${PROJECT_NAME}_DEPENDENCIES)
    if(MSVC OR XCode)
      if(NOT ${PROJECT_NAME}_DEPENDENT_BUILD)
        message(STATUS "Optimizing your superbuild experience")
        set(${PROJECT_NAME}_MULTI_BUILD ON)
        set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING INTERNAL FORCE)
      else()
        set(${PROJECT_NAME}_MULTI_BUILD OFF)
        set(CMAKE_CONFIGURATION_TYPES Debug MinSizeRel Release RelWithDebInfo CACHE STRING INTERNAL FORCE)
      endif()
    endif()
    #-----------------------------------------------------------------------------
    # Solve project dependencies
    #-----------------------------------------------------------------------------
    # Call CMakeLists.txt in cmake/external which will solve the dependencies
    # and add the external projects, including this one: this top-level
    # CMakeLists.txt will be called back with SUPERBUILD=OFF, to execute
    # the rest of the code below (INNERBUILD), which explains the `return`
    add_subdirectory(cmake/external)
    return()
  else()
    message(STATUS "No dependencies, skipping superbuild")
  endif()
endif()

#-----------------------------------------------------------------------------
#                               INNERBUILD
#-----------------------------------------------------------------------------

if(MSVC)
# Using MD as that seems to be what I run into alot, you could change these to /MT and /MTd if you want...
  set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob2 /Oi /Od /RTC1 /MP /EHsc" CACHE STRING INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "/MD /MP /EHsc" CACHE STRING INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /MP /EHsc" CACHE STRING INTERNAL FORCE)
endif()
if ( CMAKE_COMPILER_IS_GNUCC )
  # Protobuf is not using the same variable name in its swap specificiation definitions
  # Resulting in a LOT of warnings per file that includes protobuf headers, slowing the build down
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-attributes")
  if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
  endif()
endif()

set(${PROJECT_NAME}_INSTALL_FOLDER ${project_name})

include(Find)# Our find macros
include(AddLibrary)# Extras to do when creating a library
include(AddExecutable)# Extras to do when creating an executable

#-----------------------------------------------------------------------------
# Find external dependencies
#-----------------------------------------------------------------------------

find_package(Eigen3 REQUIRED)

# Tell protobuf-config.cmake to try to be compatible with FindProtobuf.cmake;
# this makes it easier for us to work with either one
set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "CMake built-in FindProtobuf.cmake module compatible")
mark_as_advanced(protobuf_MODULE_COMPATIBLE)
if(iOS_TOOLCHAIN)
  set(protobuf_MODULE_COMPATIBLE OFF)
endif()

# Try to use the config written by protobuf
find_package(protobuf QUIET CONFIG NO_DEFAULT_PATH)
if(protobuf_DIR AND protobuf_FOUND)
  # Success; set flag so we will reexport protobuf for our users
  set(Pulse_USING_PROTOBUF_DIR ON)
elseif(NOT protobuf_FOUND)
  message(STATUS "Could not find protobuf.cmake, falling back to CMake's FindProtobuf.cmake")
  # Nope; fall back on FindProtobuf.cmake
  find_package(Protobuf REQUIRED)
endif()

if(MSVC OR XCode)
  # By default, CMake will choose debug for relwithdebinfo configurations if a relwithdebinfo export package is not provided.
  # This is the case when Pulse builds protobuf. (It only builds release and debug for MSVC)
  # This default behavior prevents the Pulse relwithdebinfo from building. (Link mismatch)
  # So I am just forcing these non standard configurations to release in multiconfig builders
  set_target_properties(protobuf::libprotobuf PROPERTIES
    MAP_IMPORTED_CONFIG_MINSIZEREL Release
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
endif()

set(SCHEMA_SRC "${CMAKE_SOURCE_DIR}/schema")
set(SCHEMA_DST "${CMAKE_SOURCE_DIR}/schema/bind")

add_subdirectory(src)
add_subdirectory(data)

#--------------------------------------------------------------------------
# Setup development utility scripts
#--------------------------------------------------------------------------

# Pulse uses Python in its development utility scripts
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  message(STATUS "Using Python: ${Python3_EXECUTABLE}")
  #get_filename_component(Python_CMD ${Python3_EXECUTABLE} NAME)
  set(Python_CMD ${Python3_EXECUTABLE})
else()
  set(Python_CMD "python")
  message(WARNING "The Pulse `run` utility uses python, which could not be found")
  message(WARNING "To use the `run` utility, setup python in this environment and reconfigure cmake")
endif()

# Pulse uses Java in its development utility scripts
find_package(Java)
if(Java_FOUND)
  include(UseJava)
  if(Java_JAVA_EXECUTABLE)
    message(STATUS "Using Java executable : ${Java_JAVA_EXECUTABLE}")
    set(Java_CMD ${Java_JAVA_EXECUTABLE})
  else()
    message(STATUS "Could not find Java_JAVA_EXECUTABLE")
  endif()
else()
  message(WARNING "The Pulse `run` utility uses Java, which could not be found")
  message(WARNING "To use the `run` utility, setup Java in this environment and reconfigure cmake")
endif()

if(NOT WIN32)
  # Create setup shell script to create an environment for running examples
  set(LIBRARY_PATH_VAR "LD_LIBRARY_PATH")
  if( APPLE )
    set(LIBRARY_PATH_VAR "DYLD_FALLBACK_LIBRARY_PATH")
  endif()
endif()

file(GLOB JSON_FILES ${CMAKE_SOURCE_DIR}/bin/*.json)
file(COPY ${JSON_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
if(WIN32)
  file(COPY ${CMAKE_SOURCE_DIR}/bin/run.bat DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
else()
  file(COPY ${CMAKE_SOURCE_DIR}/bin/run.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
file(COPY ${CMAKE_SOURCE_DIR}/bin/resource DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
configure_file(${CMAKE_SOURCE_DIR}/bin/rebase.py.in "${CMAKE_INSTALL_PREFIX}/bin/rebase.py" @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/run.cmake.in "${CMAKE_INSTALL_PREFIX}/bin/run.cmake" @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/run.config.in "${CMAKE_INSTALL_PREFIX}/bin/run.config" @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/docs/Doxygen/full.doxy.in ${CMAKE_INSTALL_PREFIX}/bin/docs/full.doxy @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/docs/Doxygen/report.doxy.in ${CMAKE_INSTALL_PREFIX}/bin/docs/report.doxy @ONLY)


#-----------------------------------------------------------------------------
# This variable controls the prefix used to generate the following files:
#  ${PROJECT_NAME}ConfigVersion.cmake
#  ${PROJECT_NAME}Config.cmake
#  ${PROJECT_NAME}Targets.cmake
# and it also used to initialize ${PROJECT_NAME}_INSTALL_CONFIG_DIR value.
set(export_config_name ${PROJECT_NAME})
set(${PROJECT_NAME}_INSTALL_CONFIG_DIR "lib/cmake/${${PROJECT_NAME}_INSTALL_FOLDER}")
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_INSTALL_CONFIG_DIR})
# Get the latest abbreviated commit hash of the working branch
find_package(Git)
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE export_git_hash
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
  set(export_git_hash "unknown")
endif()

configure_file(cmake/PulseBuildInformation.cpp.in ${CMAKE_BINARY_DIR}/src/cpp/PulseBuildInformation.cpp)

#------------------------------------------------------------------------------
# Configure ${PROJECT_NAME}ConfigVersion.cmake common to build and install tree
include(CMakePackageConfigHelpers)
set(config_version_file ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
write_basic_package_version_file(
  ${config_version_file}
  VERSION "${${PROJECT_NAME}_VERSION}"
  COMPATIBILITY ExactVersion
  )
#------------------------------------------------------------------------------
# Export '${PROJECT_NAME}Targets.cmake' for a build tree
export(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE Pulse::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  )

# Configure '${PROJECT_NAME}Config.cmake' for a build tree
set(build_config ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
configure_package_config_file(
  cmake/${PROJECT_NAME}ConfigBuild.cmake.in
  ${build_config}
  INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
  )

#------------------------------------------------------------------------------
# Export '${PROJECT_NAME}Targets.cmake' for an install tree
install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE Pulse::
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${${PROJECT_NAME}_INSTALL_CONFIG_DIR}
  )

set(install_config ${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake)
configure_package_config_file(
  cmake/${PROJECT_NAME}ConfigInstall.cmake.in
  ${install_config}
  INSTALL_DESTINATION ${${PROJECT_NAME}_INSTALL_CONFIG_DIR}
  )

# Install config files
install(
  FILES ${config_version_file} ${install_config}
  DESTINATION "${${PROJECT_NAME}_INSTALL_CONFIG_DIR}"
  )
