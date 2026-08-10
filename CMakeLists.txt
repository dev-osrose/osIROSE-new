cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)
project("osIROSE")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
include(FetchContent)
include(CheckOptions)

# Multi-config generators pick the configuration at build time and ignore
# CMAKE_BUILD_TYPE, so only supply a default for the single-config ones.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  if (OFFICIAL_DEBUG_BUILD)
    set(_default_build_type Debug)
  elseif (OFFICIAL_BUILD)
    set(_default_build_type RelWithDebInfo)
  elseif (DEBUG)
    set(_default_build_type Debug)
  else ()
    set(_default_build_type Release)
  endif ()

  # FORCE is only meaningful together with CACHE, and it is needed here because
  # CMake pre-creates CMAKE_BUILD_TYPE as an empty cache entry. The bare
  # `set(CMAKE_BUILD_TYPE Release FORCE)` this replaces instead stored the
  # two-element list "Release;FORCE" in a normal variable, which matched no
  # known configuration - so no CMAKE_<LANG>_FLAGS_<CONFIG> were ever applied
  # and default builds came out with no optimisation flags at all.
  set(CMAKE_BUILD_TYPE "${_default_build_type}" CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
  message(STATUS "No build type specified - defaulting to ${CMAKE_BUILD_TYPE}")
  unset(_default_build_type)
endif ()

# The Debug branch above used to also call `add_compiler_flag(-DSPDLOG_TR_ON)`.
# That was dead in three separate ways: the macro is spelled add_compiler_flags
# and is not even included until further down this file, the define is spelled
# SPDLOG_TRACE_ON in the sources that test it, and the blocks it would enable
# still use fmt::MemoryWriter, which fmt removed long before the version spdlog
# pulls in now. Enabling it verbatim would break the build, so it is left out
# until that tracing code is ported.

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
#include(CPack)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CONFIGURU ${CMAKE_SUBMODULE_DIR}/Configuru)
set(VISIT_STRUCT ${CMAKE_SUBMODULE_DIR}/visit_struct/include)

#------------------------------------------------------------------------------
# find all packages which are needed
#------------------------------------------------------------------------------

find_package(Python COMPONENTS Interpreter REQUIRED)
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

if (NOT MSVC)
  find_package(Curses ${REQUIRED_IF_OPTION})
endif()

find_package(Threads ${REQUIRED_IF_OPTION})
find_package(ZLIB ${REQUIRED_IF_OPTION})
find_package(CURL ${REQUIRED_IF_OPTION})
find_package(SQLite3 ${REQUIRED_IF_OPTION})

if (WITH_GTEST)
  find_package(GTest ${REQUIRED_IF_OPTION})
  add_definitions(-DWITH_GTEST)
endif ()

if (WITH_TRACING)
  add_definitions(-DWITH_TRACING)
endif ()

#------------------------------------------------------------------------------
# build some external projects
#------------------------------------------------------------------------------

include(Build-sol2)
include(Build-entt)
include(Build-cxxopts)
include(Build-spdlog)

if (NOT LUA_FOUND)
  include(BuildLua)
endif ()

#if (NOT ZLIB_FOUND)
#  include(Build-zlib)
#endif ()

if (NOT CURL_FOUND)
  include(BuildCURL)
endif ()

include(Build-sqlite3)
include(Build-sqlpp23)
include(SQLPPGenerator)
include(Build-osirose-common)

if (NOT GTEST_FOUND AND WITH_GTEST)
  include(BuildGTest)
endif ()

# generate_crash_symbols() is called by the server targets whether or not crash
# reports are on, so the module is always included and no-ops when disabled.
include(crashpad-symbol-generation)

if (WITH_CRASH_REPORTS)
  include(Build-crashpad)
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

install(DIRECTORY schema/ DESTINATION schema/
    COMPONENT Runtime
    PATTERN "schema/*"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ)

install(DIRECTORY scripts/ DESTINATION scripts/
    COMPONENT Runtime
    FILES_MATCHING PATTERN "*.lua"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ)
