#------------------------------------------------------------------------------
# global options
#------------------------------------------------------------------------------

option(PROJECT_DEBUG "Enable debugging messages support" ON)

set(CMAKE_THIRD_PARTY_DIR "${CMAKE_BINARY_DIR}/3rdparty" CACHE PATH "Directory where 3rdparty libs will be installed during build time")

include(CMakeDependentOption)

set(_official_build_default OFF)
if(WIN32 AND NOT MINGW
   AND "${CMAKE_GENERATOR}" MATCHES "Make"
   AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(_official_build_default ON)
endif()

option(OFFICIAL_BUILD "do you want to compile an official build?" ${_official_build_default})

if(NOT DEFINED _OFFICIAL_BUILD_INFERRED)
  # First configure: option() above just applied our inferred default.
  set(_OFFICIAL_BUILD_INFERRED ${_official_build_default} CACHE INTERNAL "Last OFFICIAL_BUILD value inferred by the build system")
elseif(OFFICIAL_BUILD STREQUAL _OFFICIAL_BUILD_INFERRED)
  set(OFFICIAL_BUILD ${_official_build_default} CACHE BOOL "do you want to compile an official build?" FORCE)
  set(_OFFICIAL_BUILD_INFERRED ${_official_build_default} CACHE INTERNAL "Last OFFICIAL_BUILD value inferred by the build system")
else()
  set(_OFFICIAL_BUILD_INFERRED "user" CACHE INTERNAL "OFFICIAL_BUILD was set by hand; stop inferring it")
endif()
unset(_official_build_default)

option(BUILD_LUA "Build Lua" ON)
option(BUILD_API_DOC "Build doxygen documentation" OFF)
option(ENABLE_ASAN "Enable address sanitizer" OFF)
option(BUILD_MYSQL "Download and build mysql libs if not found on the system" ON)
option(ENABLE_SSL "Enable TLS/SSL support via OpenSSL and asio::ssl (requires OpenSSL)" OFF)

if(OFFICIAL_BUILD)
  message(STATUS "Official Build is set - Turning off dev options")
endif()

if(MINGW)
  # Crashpad's MinGW support needs an external MASM assembler (uasm/jwasm),
  # which we do not require anywhere else in the build.
  message(STATUS "MINGW doesn't support gtest or crashpad. Disabling these features...")
endif()

cmake_dependent_option(DEV_SETTING "Enable developer defaults (unit tests, gtest)" OFF "NOT OFFICIAL_BUILD" OFF)

cmake_dependent_option(BUILD_TOOLS "Build server tools" OFF "NOT OFFICIAL_BUILD" OFF)
cmake_dependent_option(BUILD_TESTS "Build various unit tests." ${DEV_SETTING} "NOT MINGW;NOT OFFICIAL_BUILD" OFF)
cmake_dependent_option(WITH_GTEST "Add GTest support" ${DEV_SETTING} "BUILD_TESTS" OFF)
cmake_dependent_option(WITH_CRASH_REPORTS "Enable crash dump generation via Google Crashpad" OFF "NOT MINGW" OFF)

option(WITH_MYSQL "Use MySQL connection" OFF)
option(WITH_MARIADB "Use MariaDB connection" ON)
cmake_dependent_option(DEBUG "enable debug build" OFF "NOT OFFICIAL_BUILD" OFF)

if(BUILD_TESTS OR WITH_GTEST)
  include(CTest)
  enable_testing()
  
  option(WITH_COVERAGE_REPORTS "Enable generating code coverage report" OFF)
endif()

option(FORCE_SYS_DEPS "Force the use of system libs")
if(FORCE_SYS_DEPS)
  set(REQUIRED_IF_OPTION REQUIRED)
else()
  unset(REQUIRED_IF_OPTION)
endif()

#------------------------------------------------------------------------------
# unix-only options
#------------------------------------------------------------------------------

if(UNIX)
  option(DEBUG_EXTERNAL "build external libs with debug support" OFF)
endif()

#------------------------------------------------------------------------------
# windows-only options
#------------------------------------------------------------------------------

if(WIN32)
endif()


# Other options
option(DISABLE_CRYPT "Disables encryption functions" OFF)
option(WITH_EXPERMENTAL "Enable expermental features" OFF)

