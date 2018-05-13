project("osIROSE")

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

#------------------------------------------------------------------------------
# some global variables
#------------------------------------------------------------------------------

set(CMAKE_OUTPUT "${CMAKE_SOURCE_DIR}/build_out")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_PLATFORM_PATH "${CMAKE_SOURCE_DIR}/cmake/platform")
set(CMAKE_EXTERNAL_BINARY_DIR "${CMAKE_BINARY_DIR}/external")
set(CMAKE_EXTERNAL_LIBRARY_DIR "${CMAKE_EXTERNAL_BINARY_DIR}/lib")
set(CMAKE_EXTERNAL_INCLUDE_DIR "${CMAKE_EXTERNAL_BINARY_DIR}/include")
set(CMAKE_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")
set(CMAKE_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/cmake/scripts")
set(CMAKE_PATCH_DIR "${CMAKE_SOURCE_DIR}/cmake/patches")
set(CMAKE_GEN_SRC_DIR "${CMAKE_BINARY_DIR}/gen")
set(CMAKE_TEST_PROJECTS "${CMAKE_SOURCE_DIR}/src/tests")
set(CMAKE_TEST_PROJECTS_BIN "${CMAKE_BINARY_DIR}/cmake_test")
set(CMAKE_THIRD_PARTY_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/include)

set(SOL2_ROOT ${CMAKE_THIRD_PARTY_DIR}/sol2)
set(VISIT_STRUCT ${CMAKE_THIRD_PARTY_DIR}/visit_struct/include)
set(CONFIGURU ${CMAKE_THIRD_PARTY_DIR}/Configuru)
set(LUA_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/lua/src)
set(CXXOPT_ROOT ${CMAKE_THIRD_PARTY_DIR}/cxxopts/include)
set(SPDLOG_ROOT ${CMAKE_THIRD_PARTY_DIR}/spdlog/include)
set(ASIO_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/asio/asio/include)
set(ENTITYX_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/entityx/entityx)
set(SQLPP_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/sqlpp11/include)
set(DATE_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/date)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

include(CMakeParseArguments)
include(ExternalProject)

if(Debug)
  set(DEBUG 1)
endif()

include(CheckOptions)

if(NOT CMAKE_BUILD_TYPE)
  if(OFFICIAL_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
  elseif(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
  else()
    set(CMAKE_BUILD_TYPE Release)
  endif()
  
  if(OFFICIAL_DEBUG_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
  endif()
endif()

include(GNUInstallDirs)
include(ExternalProject)

include(macros)
include(CheckPlatform)
include(SetInstallPaths)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GMOCK_ROOT ${CMAKE_THIRD_PARTY_DIR}/googletest/googlemock)
set(GTEST_ROOT ${CMAKE_THIRD_PARTY_DIR}/googletest/googletest)

set(SOL2_ROOT ${CMAKE_THIRD_PARTY_DIR}/sol2)
set(VISIT_STRUCT ${CMAKE_THIRD_PARTY_DIR}/visit_struct/include)
set(CONFIGURU ${CMAKE_THIRD_PARTY_DIR}/Configuru)
set(LUA_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/lua/src)
set(CXXOPT_ROOT ${CMAKE_THIRD_PARTY_DIR}/cxxopts/include)
set(SPDLOG_ROOT ${CMAKE_THIRD_PARTY_DIR}/spdlog/include)
set(ASIO_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/asio/asio/include)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/3rdparty/include)
set(ENTITYX_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/entityx/entityx)
set(SQLPP_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/sqlpp11/include)
set(DATE_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/date)

#------------------------------------------------------------------------------
# find all packages which are needed
#------------------------------------------------------------------------------

if(NOT WIN32)
  set(REQUIRED_ON_UNIX REQUIRED)
endif()

if(WITH_COTIRE)
  include(cotire OPTIONAL)
endif()

IF(WITH_MYSQL)
  set(SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/sqlpp11-connector-mysql/include)
  find_package(MySQL REQUIRED)
ENDIF()

find_package(PythonInterp 2.7 REQUIRED)

if(WIN32)
  find_package(PythonLibs 2.7 REQUIRED)
endif()

find_package(Threads REQUIRED)

find_package(CURL ${REQUIRED_IF_OPTION})

if(WITH_GTEST)
    find_package(GTest ${REQUIRED_IF_OPTION})
    add_definitions(-DWITH_GTEST)
endif()

if(WITH_GTEST AND WITH_GMOCK)
  find_package(GMock ${REQUIRED_IF_OPTION})
  add_definitions(-DWITH_GMOCK) 
endif()

if (WITH_TRACING)
  add_definitions(-DWITH_TRACING)
endif()

#------------------------------------------------------------------------------
# build some external projects
#------------------------------------------------------------------------------

if(NOT BREAKPAD_EXCEPTION_HANDLER_FOUND AND WITH_CRASH_REPORTS)
  include(BuildGoogleBreakpad)
endif()

if(NOT GTEST_FOUND AND WITH_GTEST)
  include(BuildGTest)
endif()

if(NOT CURL_FOUND)
  include(BuildCURL)
endif()

add_subdirectory(3rdparty)

#--------------------------------
# Build API Docs
#--------------------------------
if (BUILD_API_DOC)
  find_package(Doxygen REQUIRED)
  if (DOXYGEN_FOUND)
    add_subdirectory(doc)
  endif (DOXYGEN_FOUND)
endif (BUILD_API_DOC)

#--------------------------------
# Setup version info
#--------------------------------
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Evaluate ABI version number.
MATH(EXPR PROJECT_ABI_VERSION_SUFFIX "${VERSION_MAJOR} * 10000 + ${VERSION_MINOR} * 100 + ${VERSION_PATCH} * 1")

set(PROJECT_ABI_VERSION v${PROJECT_ABI_VERSION_SUFFIX})

CREATE_VERSION_FILE()
include_directories(${CMAKE_GEN_SRC_DIR})


add_subdirectory(src)
add_subdirectory(tools)
