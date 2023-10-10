#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
if(${PROJECT_NAME}_MULTI_BUILD)
  message(STATUS "Building multiple protobuf configurations at once")
  set(PROTOBUF_DEBUG_BLD COMMAND ${CMAKE_COMMAND} --build . --config debug)
  set(PROTOBUF_RELEASE_BLD COMMAND ${CMAKE_COMMAND} --build . --config release)
  set(PROTOBUF_MULTI_BUILD BUILD_COMMAND ${PROTOBUF_DEBUG_BLD} ${PROTOBUF_RELEASE_BLD})
  set(PROTOBUF_DEBUG_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config debug)
  set(PROTOBUF_RELEASE_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config release)
  set(PROTOBUF_MULTI_INSTALL BUILD_COMMAND ${PROTOBUF_DEBUG_INSTALL} ${PROTOBUF_RELEASE_INSTALL})
endif()

# Support getting various versions, as end users could be using a different versions
# And their application runtime environment needs to have all consistent dlls, or they fight!
# Note that the Protobuf C++ API changed and required Pulse updates
# Versions older than 22.0 are not supported anymore
# Generally, We only support the latest version at the time of a release
# And any release that somebody has requested we support for compatibility with their application

set(Protobuf_VERSION "24.3" CACHE STRING "Select the  version of ProtoBuf to build.")
set_property(CACHE Protobuf_VERSION PROPERTY STRINGS "24.3")

set(Protobuf_url "https://github.com/protocolbuffers/protobuf/releases/download/v${Protobuf_VERSION}/protobuf-${Protobuf_VERSION}.zip" )

if (Protobuf_VERSION VERSION_EQUAL 24.3)# Latest, Can change
  set(Protobuf_md5 "d425496d9fbfaeaecde6fb07fdf41312" )
else()
  message(STATUS "Using Protobuf Version ${Protobuf_VERSION}, with no git hash, will redownload if you rebuild.")
  set(Protobuf_md5 "" )
endif()

set(BUILD_PROTOC_BINARIES ON)
if(${PROJECT_NAME}_C_AS_STATIC)
  set(BUILD_PROTOC_BINARIES OFF)
endif()

include(AddExternalProject)
define_external_dirs_ex(protobuf)
add_external_project_ex( protobuf
  URL ${Protobuf_url}
  URL_MD5 ${Protobuf_md5}
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -Dprotobuf_BUILD_TESTS:BOOL=OFF
    -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
    -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
    -Dprotobuf_BUILD_PROTOC_BINARIES:BOOL=${BUILD_PROTOC_BINARIES}
    -Dprotobuf_ABSL_PROVIDER:STRING=package
    -Dabsl_DIR:PATH=${absl_DIR}
  ${PROTOBUF_MULTI_BUILD}
  ${PROTOBUF_MULTI_INSTALL}
  RELATIVE_INCLUDE_PATH "include"
  DEPENDENCIES absl
  #VERBOSE
)

if (NOT USE_SYSTEM_protobuf)
  set(protobuf_INSTALL ${CMAKE_INSTALL_PREFIX})
  if(WIN32)
    set(protobuf_DIR ${protobuf_INSTALL}/cmake)
  else()
    set(protobuf_DIR ${protobuf_INSTALL}/${CMAKE_INSTALL_LIBDIR}/cmake/protobuf)
  endif()
  set(protobuf_SRC ${protobuf_PREFIX}/src)
  set(utf8_range_DIR "${protobuf_INSTALL}/${CMAKE_INSTALL_LIBDIR}/cmake/utf8_range")
  
  message(STATUS "protobuf_DIR : ${protobuf_DIR}")
  message(STATUS "utf8_range_DIR : ${utf8_range_DIR}")
endif()
