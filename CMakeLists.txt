cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_USER_MAKE_RULES_OVERRIDE
   ${CMAKE_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
   ${CMAKE_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

IF(DEFINED ENV{BUILD_TYPE})
  set(BUILD_TYPE ENV{BUILD_TYPE})
ELSE()
  set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
ENDIF()

set(CMAKE_BUILD_TYPE ${BUILD_TYPE})

project("osIROSE")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(OsIROSECMakeTools)
include(FindProtocolBuffer)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Evaluate ABI version number.
MATH(EXPR PROJECT_ABI_VERSION_SUFFIX "${VERSION_MAJOR} * 10000 + ${VERSION_MINOR} * 100 + ${VERSION_PATCH} * 1")

set(PROJECT_ABI_VERSION v${PROJECT_ABI_VERSION_SUFFIX})

CREATE_VERSION_FILE()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(PROJECT_DEBUG 
  "Enable debugging messages support" ON)
option(ENABLE_TESTING 
  "Enable testing" OFF)
option(ENABLE_TESTING_COVERAGE, 
  "Enable generating code coverage report" OFF)
option(BUILD_TOOLS 
  "Build server tools" ON)
option(BUILD_PROTOBUF 
  "Build Protobuf (Leave this off)" OFF)
option(BUILD_API_DOC
  "Build doxygen documentation" OFF)
option(ENABLE_MYSQL_ONE_INSTANCE 
	"Enable use of the single instance mysql database" ON)
option(ENABLE_EXPERMENTAL
  "Enable expermental features" OFF)
option(DISABLE_CRYPT
  "Enable this to not use encryption functions" OFF)

#build API doc
IF (BUILD_API_DOC)
  find_package(Doxygen)
  IF (DOXYGEN_FOUND)
    add_subdirectory(doc)
  ENDIF (DOXYGEN_FOUND)
ENDIF (BUILD_API_DOC)
#build API doc

IF(ENABLE_MYSQL_ONE_INSTANCE OR BUILD_MYSQLPP)
  set(ENABLE_MYSQL ON)
ENDIF()

set(EXTERNAL_DEPS_ROOT ${CMAKE_SOURCE_DIR}/tools)

IF(ENABLE_TESTING)
  enable_testing() #enable ctest
  
  include( CTest )
  include( Dart )

  set(CMAKE_RULE_MESSAGES OFF)
  
  IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_subdirectory(${EXTERNAL_DEPS_ROOT}/googletest/googlemock)
    set(GMOCK_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/googletest/googlemock/include)
    set(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
    set(GTEST_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/googletest/googletest/include)
    set(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
  ELSE()
    add_subdirectory(${EXTERNAL_DEPS_ROOT}/googletest/googlemock)
    set(GMOCK_INCLUDE_DIR ${gmock_SOURCE_DIR}/include)
    set(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
    set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
    set(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
  ENDIF()
  
  set(CMAKE_RULE_MESSAGES ON)
ENDIF()

set(PROTOBUF_ROOT ${CMAKE_BINARY_DIR})
set(SOL2_ROOT ${EXTERNAL_DEPS_ROOT}/sol2)
set(PBJSON_ROOT ${EXTERNAL_DEPS_ROOT}/pbjson/src)
set(LUA_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/lua/src)
set(CXXOPT_ROOT ${EXTERNAL_DEPS_ROOT}/cxxopts/include)
set(SPDLOG_ROOT ${EXTERNAL_DEPS_ROOT}/spdlog/include)
set(SELENE_ROOT ${EXTERNAL_DEPS_ROOT}/Selene/include)
set(EXTERNAL_BINARY_DIR ${CMAKE_SOURCE_DIR}/3rdparty/lib)
set(ASIO_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/asio/asio/include)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/include)
set(PROTOBUF_INCLUDE_DIR "${EXTERNAL_DEPS_ROOT}/protobuf/src" CACHE STRING "${EXTERNAL_DEPS_ROOT}/protobuf/src" FORCE)
set(MYSQLpp_INCLUDE_DIR "${EXTERNAL_DEPS_ROOT}/mysqlpp/lib" CACHE STRING "${EXTERNAL_DEPS_ROOT}/mysqlpp/lib" FORCE)
set(ENTITYX_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/entityx/entityx)
set(SQLPP_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/sqlpp11/include)
set(DATE_INCLUDE_DIR ${EXTERNAL_DEPS_ROOT}/date)

IF(ENABLE_MYSQL)  
  message(STATUS "MySQL++ Include Path: ${MYSQLpp_INCLUDE_DIR}")
  
  find_package(MySQL REQUIRED)
  
  configure_file(
    ${MYSQLpp_INCLUDE_DIR}/mysql++.h.in
    ${MYSQLpp_INCLUDE_DIR}/mysql++.h
  )
ENDIF()

IF (MSVC)
  set(PROTOBUF_PROTOC_EXECUTABLE "${CMAKE_BINARY_DIR}/bin/${BUILD_TYPE}/protoc.exe" CACHE STRING "${CMAKE_BINARY_DIR}/bin/${BUILD_TYPE}/protoc.exe" FORCE)
  set(MYSQLpp_LIBRARY_PATH "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/" CACHE STRING "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/" FORCE)
  
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4221")
  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4221")
  
  IF (${BUILD_TYPE} STREQUAL Debug)
    set(PROTOBUF_LIBRARY "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/libprotobufd.lib" CACHE STRING "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/libprotobufd.lib" FORCE)
  ELSE()
    set(PROTOBUF_LIBRARY "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/libprotobuf.lib" CACHE STRING "${CMAKE_BINARY_DIR}/lib/${BUILD_TYPE}/libprotobuf.lib" FORCE)
  ENDIF()
ELSE()
  IF(ENABLE_MYSQL)
    set(MYSQL_INCLUDE_DIR /usr/include/mysql)
  ENDIF()

  IF(BUILD_PROTOBUF)
    set(PROTOBUF_PROTOC_EXECUTABLE ${CMAKE_BINARY_DIR}/bin/protoc)
  ELSE()
      set(PROTOBUF_ROOT ${CMAKE_SOURCE_DIR}/protobuf)
      set(PROTOBUF_PROTOC_EXECUTABLE ${CMAKE_SOURCE_DIR}/protobuf/bin/protoc)
    set(MYSQLpp_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/protobuf/lib" CACHE STRING "${CMAKE_SOURCE_DIR}/protobuf/lib" FORCE)
    set(PROTOBUF_LIBRARY "${CMAKE_SOURCE_DIR}/protobuf/lib/libprotobuf.a" CACHE STRING "${CMAKE_SOURCE_DIR}/protobuf/lib/libprotobuf.a" FORCE)
  ENDIF()
  
  set(LINK_FLAGS "${LINK_FLAGS} -lnsl -lz -lm")
ENDIF()

IF(ENABLE_EXPERMENTAL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_EXPERMENTAL")
ENDIF()

include_directories(${CMAKE_BINARY_DIR}/generated)

set(BUILD_LUA ON CACHE BOOL "" FORCE)
add_subdirectory(tools)
add_subdirectory(src)
