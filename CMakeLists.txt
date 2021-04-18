cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)
project("osIROSE")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_LAUNCHER)
  set(ENV{CCACHE_SLOPPINESS} "pch_defines,time_macros")
endif ()

#------------------------------------------------------------------------------
# some global variables
#------------------------------------------------------------------------------

set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/dev-osrose/osIROSE-new")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_PLATFORM_PATH "${CMAKE_SOURCE_DIR}/cmake/platform")
set(CMAKE_SUBMODULE_DIR "${CMAKE_SOURCE_DIR}/3rdparty")
set(CMAKE_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/cmake/scripts")
set(CMAKE_PATCH_DIR "${CMAKE_SOURCE_DIR}/cmake/patches")
set(CMAKE_TESTS_PATH "${CMAKE_SOURCE_DIR}/cmake/tests")
set(CMAKE_GEN_SRC_DIR "${CMAKE_BINARY_DIR}/gen")
set(CMAKE_TEST_PROJECTS "${CMAKE_SOURCE_DIR}/src/tests")
set(CMAKE_TEST_PROJECTS_BIN "${CMAKE_BINARY_DIR}/cmake_test")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

include(CMakeParseArguments)
include(GNUInstallDirs)
include(ExternalProject)
include(CheckOptions)

if (NOT CMAKE_BUILD_TYPE)
  if (OFFICIAL_BUILD)
    set(CMAKE_BUILD_TYPE RelWithDebInfo FORCE)
  elseif (DEBUG)
    set(CMAKE_BUILD_TYPE Debug FORCE)
    add_compiler_flag(-DSPDLOG_TRACE_ON)
  else ()
    set(CMAKE_BUILD_TYPE Release FORCE)
  endif ()

  if (OFFICIAL_DEBUG_BUILD)
    set(CMAKE_BUILD_TYPE Debug FORCE)
  endif ()
endif ()

set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")

if ("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
  set(BUILD_TYPE "$<CONFIG>")
endif ()

set(CMAKE_THIRD_PARTY_BINARY_DIR ${CMAKE_THIRD_PARTY_DIR}/bin)
set(CMAKE_THIRD_PARTY_LIBRARY_DIR ${CMAKE_THIRD_PARTY_DIR}/lib)
set(CMAKE_THIRD_PARTY_INCLUDE_DIR ${CMAKE_THIRD_PARTY_DIR}/include)
set(CMAKE_PREFIX_PATH "${CMAKE_THIRD_PARTY_DIR}")

# Create folders
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_INCLUDE_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_LIBRARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_THIRD_PARTY_DIR}/share)

include(macros)
include(CheckPlatform)
include(SetInstallPaths)
include(CPack)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#TODO: move these to find/build modules
set(SOL2_ROOT ${CMAKE_SUBMODULE_DIR}/sol2)
set(SPDLOG_ROOT ${CMAKE_SUBMODULE_DIR}/spdlog/include)
set(CXXOPT_ROOT ${CMAKE_SUBMODULE_DIR}/cxxopts/include)

set(CONFIGURU ${CMAKE_SUBMODULE_DIR}/Configuru)
set(VISIT_STRUCT ${CMAKE_SUBMODULE_DIR}/visit_struct/include)
set(ASIO_INCLUDE_DIR ${CMAKE_SUBMODULE_DIR}/asio/asio/include)
set(ENTT_INCLUDE_DIR ${CMAKE_SUBMODULE_DIR}/entt/src/entt)

#------------------------------------------------------------------------------
# find all packages which are needed
#------------------------------------------------------------------------------

if (WITH_COTIRE)
  include(cotire OPTIONAL)
endif ()

IF (WITH_MYSQL)
  find_package(MySQL 5.7 REQUIRED)
ENDIF ()

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_python_module(pyparsing REQUIRED)
if (NOT PY_PYPARSING)
  install_python_module(pyparsing)
endif()

find_package(Patch REQUIRED)
if (MSVC)
  configure_file(${CMAKE_SCRIPT_PATH}/patch.bat ${CMAKE_GEN_SRC_DIR}/scripts/patch.bat @ONLY)
  set(PATCH_SCRIPT_PATH ${CMAKE_GEN_SRC_DIR}/scripts/patch.bat)
else ()
  set(PATCH_SCRIPT_PATH ${CMAKE_SCRIPT_PATH}/patch.sh)
endif ()

find_package(Threads ${REQUIRED_IF_OPTION})
find_package(ZLIB ${REQUIRED_IF_OPTION})
find_package(CURL ${REQUIRED_IF_OPTION})

if (WITH_GTEST)
  find_package(GTest ${REQUIRED_IF_OPTION})
  add_definitions(-DWITH_GTEST)
endif ()

if (WITH_GTEST AND WITH_GMOCK)
  find_package(GMock ${REQUIRED_IF_OPTION})
  add_definitions(-DWITH_GMOCK)
endif ()

if (WITH_CRASH_REPORTS)
  if (NOT CRASHPAD)
    find_package(Breakpad ${REQUIRED_IF_OPTION})
    include(BreakpadSymbolGeneration)
  else ()
#    find_package(Crashpad ${REQUIRED_IF_OPTION})
    include(crashpad-symbol-generation)
  endif ()
endif ()

if (WITH_TRACING)
  add_definitions(-DWITH_TRACING)
endif ()

#------------------------------------------------------------------------------
# build some external projects
#------------------------------------------------------------------------------

if (NOT LUA_FOUND)
  include(BuildLua)
endif ()

if (NOT ZLIB_FOUND)
  include(Build-zlib)
endif ()

if (NOT CURL_FOUND)
  include(BuildCURL)
endif ()

if (NOT SQLITE3_FOUND)
  include(Build-sqlite3)
endif ()
include(Build-HinnantDate)
include(Build-sqlpp11)
include(Build-sqlpp11-connector-sqlite3)
include(SQLPPGenerator)
#include(Build-BehaviorTree)
include(Build-osirose-common)

if (WITH_MYSQL)
  if (MSVC AND BUILD_MYSQL AND NOT MySQL_FOUND)
    include(Build-mysql)
  endif ()

  include(Build-sqlpp11-connector-mysql)
endif ()

if (NOT CRASHPAD)
  if (NOT Breakpad_FOUND AND WITH_CRASH_REPORTS)
    include(BuildGoogleBreakpad)
  endif ()
else ()
  include(Build-gn)
  include(Build-crashpad)
endif ()

if (NOT GTEST_FOUND AND WITH_GTEST)
  include(BuildGTest)
endif ()

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
set(VERSION_MINOR 3)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Evaluate ABI version number.
MATH(EXPR PROJECT_ABI_VERSION_SUFFIX "${VERSION_MAJOR} * 10000 + ${VERSION_MINOR} * 100 + ${VERSION_PATCH} * 1")

set(PROJECT_ABI_VERSION v${PROJECT_ABI_VERSION_SUFFIX})

CREATE_VERSION_FILE()

file(COPY "${CMAKE_SOURCE_DIR}/scripts"
    DESTINATION "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(src)
add_subdirectory(tools)

install(DIRECTORY Database/ DESTINATION Database/
    COMPONENT Runtime
    PATTERN "Database/*"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ)

install(DIRECTORY scripts/ DESTINATION scripts/
    COMPONENT Runtime
    FILES_MATCHING PATTERN "*.lua"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ)
