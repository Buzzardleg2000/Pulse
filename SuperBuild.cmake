include(ExternalProject)
include(CMakeDetermineSystem)
project(OuterBuild)

set(BUILD_SHARED_LIBS OFF)
if(MSVC OR XCode)
# For multi configuration IDE environments,
# these dependent libs only need to be built in release
# There should not be a need to debug into xerces, but if 
# you really want to, you can go into the xerces dir and build 
# a debug version of the library
  set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE INTERNAL FORCE )
endif()
##################################
## EIGEN                        ##
## Used for general matrix math ##
##################################

message( STATUS "External project - Eigen" )
set(Eigen_VERSION "3.3.3" )
set(Eigen_DIR "${CMAKE_BINARY_DIR}/eigen/src/eigen")
set(Eigen_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/eigen/install")

ExternalProject_Add( eigen
  PREFIX eigen
  URL "http://bitbucket.org/eigen/eigen/get/${Eigen_VERSION}.tar.gz"
  URL_HASH MD5=f21cee193e15e55cfd15ebbc16fc00a7
  INSTALL_DIR "${Eigen_INSTALL}"
  CMAKE_ARGS
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${Eigen_INSTALL}
        -DINCLUDE_INSTALL_DIR:STRING=${Eigen_INSTALL}/include
)
list(APPEND Pulse_DEPENDENCIES eigen)
# Install Headers
list(APPEND CMAKE_PREFIX_PATH ${Eigen_INSTALL})
message(STATUS "Eigen is here : ${Eigen_DIR}" )

###################################################
## LOG4CPP                                       ##
## General logging utility                       ##
## We have our own cmake file to build as dll/so ##
###################################################

message( STATUS "External project - Log4cpp" )
set(log4cpp_VERSION "1.1.2" )
set(log4cpp_DIR "${CMAKE_BINARY_DIR}/log4cpp/src/log4cpp")
set(log4cpp_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/log4cpp/install")

if(UNIX)
  set(CONFIGURE "./configure")
endif()
		
ExternalProject_Add( log4cpp
  PREFIX log4cpp
  URL "https://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.2.tar.gz"
  URL_HASH MD5=c70eac7334e2f3cbeac307dc78532be4
  UPDATE_COMMAND 
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/log4cpp-patches/CMakeLists.txt ${log4cpp_DIR}/CMakeLists.txt
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/log4cpp-patches/config.guess ${log4cpp_DIR}/config/config.guess
    COMMAND ${CONFIGURE}
# Build this in the Inner build
# It will be easier to switch cofigurations in MSVC/XCode
# INSTALL_DIR "${log4cpp_INSTALL}"
# CMAKE_ARGS
#       -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
#       -DCMAKE_INSTALL_PREFIX:STRING=${log4cpp_INSTALL}
#       -DINCLUDE_INSTALL_DIR:STRING=${log4cpp_INSTALL}/include
# do nothing
  CONFIGURE_COMMAND "" 
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)
list(APPEND Pulse_DEPENDENCIES log4cpp)
list(APPEND CMAKE_PREFIX_PATH ${log4cpp_INSTALL})

###################################################
## Google Proto Buffers                          ##
## Multi-language serialization                  ##
###################################################

message( STATUS "External project - protobuf" )
set(protobuf_VERSION "3.4.0" )
set(protobuf_MD5 "ca583ee8bfc9e23b257e8724d5aae180" )
set(protobuf_DIR "${CMAKE_BINARY_DIR}/protobuf/src/protobuf")
set(protobuf_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/protobuf/install")

ExternalProject_Add( protobuf
  PREFIX protobuf 
  URL "https://github.com/google/protobuf/archive/v3.4.0rc2.zip"
  URL_MD5 ${protobuf_MD5}
  DOWNLOAD_DIR ${protobuf_DIR}
  INSTALL_DIR "${protobuf_INSTALL}"
  SOURCE_SUBDIR ./cmake
  CMAKE_ARGS 
    -Dprotobuf_BUILD_TESTS:BOOL=OFF
    -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
    -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:STRING=${protobuf_INSTALL}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG}
    -DCMAKE_CXX_FLAGS_RELEASE:STRING=${CMAKE_CXX_FLAGS_RELEASE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
    ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DMAKECOMMAND:STRING=${MAKECOMMAND}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
)
list(APPEND Pulse_DEPENDENCIES protobuf)
list(APPEND CMAKE_PREFIX_PATH ${protobuf_INSTALL})

message(STATUS "protobuf is here : ${protobuf_DIR}" )

if(WIN32)
  ##########################################
  ## DIRENT                               ##
  ## Platform generic file system utility,##
  ## that is not included in msvc/mingw   ##
  ##########################################

  message( STATUS "External project - dirent" )
  set(dirent_DIR "${CMAKE_BINARY_DIR}/dirent/src/dirent")
  set(dirent_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/dirent/install")
  set(dirent_MD5 "cf5b4499d163604732f4dc91654056be" )
  ExternalProject_Add( dirent
    PREFIX dirent
    URL "https://github.com/tronkko/dirent/archive/1.22.zip"
    URL_MD5 ${dirent_MD5}
    INSTALL_DIR "${dirent_INSTALL}"
    CMAKE_ARGS
          -DBUILD_SHARED_LIBS:BOOL=OFF
          -DCMAKE_INSTALL_PREFIX:STRING=${dirent_INSTALL}
          -DINCLUDE_INSTALL_DIR:STRING=${dirent_INSTALL}/include
  )
  message(STATUS "dirent is here : ${dirent_DIR}" )
  list(APPEND Pulse_DEPENDENCIES dirent)
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/dirent/install)
endif()

# Support ccache
if( CMAKE_VERSION VERSION_LESS 3.4 )
  set( CMAKE_CXX_COMPILER_LAUNCHER_FLAG )
  set( CMAKE_C_COMPILER_LAUNCHER_FLAG )
else()
  set( CMAKE_CXX_COMPILER_LAUNCHER_FLAG
    -DCMAKE_CXX_COMPILER_LAUNCHER:FILEPATH=${CMAKE_CXX_COMPILER_LAUNCHER} )
  set( CMAKE_C_COMPILER_LAUNCHER_FLAG
    -DCMAKE_C_COMPILER_LAUNCHER:FILEPATH=${CMAKE_C_COMPILER_LAUNCHER} )
endif()

# ExternalProject_Add doesn't like to work with lists: it keeps only the first element
string(REPLACE ";" "::" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

# Generate the Pulse project after dependencies have been built
ExternalProject_Add( Pulse
  PREFIX Pulse
  DEPENDS ${Pulse_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  BINARY_DIR ${CMAKE_BINARY_DIR}/Pulse
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  BUILD_AWAYS 1
  LIST_SEPARATOR ::
  CMAKE_ARGS
    -DSUPERBUILD:BOOL=OFF
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
    ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DMAKECOMMAND:STRING=${MAKECOMMAND}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}

    -DBUILD_SHARED_LIBS:BOOL=${shared}
    -DBUILD_TESTING:BOOL=${BUILD_TESTING}
    # Let InnerBuild build and install these
    -DEigen_INSTALL=${Eigen_INSTALL}
    -Dlog4cpp_DIR=${log4cpp_DIR}
    -DLOG4CPP_INCLUDE_DIR=${log4cpp_DIR}/include
    -Dlog4cpp_INSTALL=${log4cpp_INSTALL}
    -Dprotobuf_DIR=${protobuf_DIR}
    -Dprotobuf_INSTALL=${protobuf_INSTALL}
    -Ddirent_INSTALL=${dirent_INSTALL}
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
)

add_custom_target(PulseData ALL)
add_dependencies(PulseData Pulse)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genData -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genStates -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
if(PULSE_DOWNLOAD_BASELINES)
  message(STATUS "I am going to pull the latest Pulse Baselines!")
  add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=updateBaselines -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
else()
  message(STATUS "I am NOT going to pull the latest Pulse Baselines!")
endif()
