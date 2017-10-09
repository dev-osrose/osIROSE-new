
# update this
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
set(CMAKE_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")
set(CMAKE_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/cmake/scripts")
set(CMAKE_PATCH_DIR "${CMAKE_SOURCE_DIR}/cmake/patches")
set(CMAKE_GEN_SRC_DIR "${CMAKE_BINARY_DIR}/gen")
set(CMAKE_TEST_PROJECTS "${CMAKE_SOURCE_DIR}/src/tests")
set(CMAKE_TEST_PROJECTS_BIN "${CMAKE_BINARY_DIR}/cmake_test")

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

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

if(Debug)
  set(DEBUG 1)
endif()

if(DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
else()
  if(OFFICIAL_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
  else()
    set(CMAKE_BUILD_TYPE Release)
  endif()
endif()

if(OFFICIAL_DEBUG_BUILD)
  set(CMAKE_BUILD_TYPE Debug)
endif()

include(CheckOptions)
include(ExternalProject)
include(macros)
include(parsearguments)
include(CheckCompiler)
include(SetInstallPaths)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#------------------------------------------------------------------------------
# find all packages which are needed
#------------------------------------------------------------------------------

if(NOT WIN32)
  set(REQUIRED_ON_UNIX REQUIRED)
endif()

IF(WITH_MYSQL)
  set(SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/sqlpp11-connector-mysql/include)
  find_package(MySQL REQUIRED)
ENDIF()

find_package(PythonInterp REQUIRED)

if(WIN32)
  find_package(PythonLibs REQUIRED)
endif()

find_package(Threads REQUIRED)

if(WITH_GTEST)
    find_package(GTest ${REQUIRED_IF_OPTION})
    add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY -DWITH_GTEST)
endif()

if(WITH_GTEST AND WITH_GMOCK)
  find_package(GMock ${REQUIRED_IF_OPTION})
  add_definitions(-DGMOCK_LINKED_AS_SHARED_LIBRARY -DWITH_GMOCK) 
endif()

if (WITH_TRACING)
  add_definitions(-DWITH_TRACING)
endif()

#------------------------------------------------------------------------------
# build some external projects
#------------------------------------------------------------------------------

if(NOT BREAKPAD_EXCEPTION_HANDLER_FOUND)
  include(BuildGoogleBreakpad)
endif()

if(NOT GTEST_FOUND AND WITH_GTEST)
  include(BuildGTest)
endif()

if(NOT GMOCK_FOUND AND WITH_GTEST AND WITH_GMOCK)
  include(BuildGMock)
endif() 

add_subdirectory(3rdparty)

#--------------------------------
# Build API Docs
#--------------------------------
IF (BUILD_API_DOC)
  find_package(Doxygen)
  IF (DOXYGEN_FOUND)
    add_subdirectory(doc)
  ENDIF (DOXYGEN_FOUND)
ENDIF (BUILD_API_DOC)

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







# Old System

#include(OsIROSECMakeTools)
#
##--------------------------------
## Setup version info
##--------------------------------
#set(VERSION_MAJOR 1)
#set(VERSION_MINOR 2)
#set(VERSION_PATCH 0)
#set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
#
## Evaluate ABI version number.
#MATH(EXPR PROJECT_ABI_VERSION_SUFFIX "${VERSION_MAJOR} * 10000 + ${VERSION_MINOR} * 100 + ${VERSION_PATCH} * 1")
#
#set(PROJECT_ABI_VERSION v${PROJECT_ABI_VERSION_SUFFIX})
#
#CREATE_VERSION_FILE()
#
##--------------------------------
## Setup output directories
##--------------------------------
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#
#
##--------------------------------
## Setup options
##--------------------------------
#option(PROJECT_DEBUG
#  "Enable debugging messages support" ON)
#option(ENABLE_TESTING
#  "Enable testing" OFF)
#option(ENABLE_TESTING_COVERAGE,
#  "Enable generating code coverage report" OFF)
#option(BUILD_TOOLS
#  "Build server tools" ON)
#option(BUILD_API_DOC
#  "Build doxygen documentation" OFF)
#option(ENABLE_EXPERMENTAL
#  "Enable expermental features" OFF)
#option(DISABLE_CRYPT
#  "Enable this to not use encryption functions" OFF)
#option(ENABLE_MYSQL
#    "Use Mysql connection" ON)
#
#

#
##--------------------------------
## Setup/build external libaries
##--------------------------------
#set(EXTERNAL_DEPS_ROOT ${CMAKE_SOURCE_DIR}/3rdparty)
#
#IF(ENABLE_TESTING)
#  enable_testing() #enable ctest
#
#  include( CTest )
#  include( Dart )
#
#  set(CMAKE_RULE_MESSAGES OFF)
#
#  add_subdirectory(${EXTERNAL_DEPS_ROOT}/googletest/googlemock)
#  
#  IF (MSVC)
#    set(GMOCK_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/googletest/googlemock/include)
#    set(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
#    set(GTEST_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/googletest/googletest/include)
#    set(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
#  ELSE()
#    set(GMOCK_INCLUDE_DIR ${gmock_SOURCE_DIR}/include)
#    set(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
#    set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
#    set(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
#  ENDIF()
#
#  set(CMAKE_RULE_MESSAGES ON)
#ENDIF()
#
#set(SOL2_ROOT ${EXTERNAL_DEPS_ROOT}/sol2)
#set(VISIT_STRUCT ${EXTERNAL_DEPS_ROOT}/visit_struct/include)
#set(CONFIGURU ${EXTERNAL_DEPS_ROOT}/Configuru)
#set(LUA_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/lua/src)
#set(CXXOPT_ROOT ${EXTERNAL_DEPS_ROOT}/cxxopts/include)
#set(SPDLOG_ROOT ${EXTERNAL_DEPS_ROOT}/spdlog/include)
#set(SELENE_ROOT ${EXTERNAL_DEPS_ROOT}/Selene/include)
#set(ASIO_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/asio/asio/include)
#set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/include)
#set(ENTITYX_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/entityx/entityx)
#set(SQLPP_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/sqlpp11/include)
#set(DATE_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/date)
#
#IF(ENABLE_MYSQL)
#  set(SQLPP_CONNECTOR_MYSQL_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/sqlpp11-connector-mysql/include)
#  find_package(MySQL REQUIRED)
#ENDIF()
#
#IF (MSVC)
#
#  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4221")
#  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4221")
#
#ELSE()
#  IF(ENABLE_MYSQL AND NOT MYSQL_INCLUDE_DIR)
#    set(MYSQL_INCLUDE_DIR /usr/include/mysql)
#  ENDIF()
#
#  set(LINK_FLAGS "${LINK_FLAGS} -lnsl -lz -lm")
#ENDIF()
#
#IF(ENABLE_EXPERMENTAL)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_EXPERMENTAL")
#ENDIF()
#
#include_directories(${CMAKE_BINARY_DIR}/generated)
#
## We have to build lua for the project
#set(BUILD_LUA ON CACHE BOOL "" FORCE)
#add_subdirectory(3rdparty)
#add_subdirectory(src)
#add_subdirectory(tools)
#