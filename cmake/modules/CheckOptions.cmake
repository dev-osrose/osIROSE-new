#------------------------------------------------------------------------------
# global options
#------------------------------------------------------------------------------

option(PROJECT_DEBUG "Enable debugging messages support" ON)

OPTION(BUILD_LUA "Build Lua" ON)
option(BUILD_TOOLS "Build server tools" ON)
option(BUILD_API_DOC "Build doxygen documentation" OFF)

option(BUILD_TESTS "Build various unit tests." ON)
option(WITH_GTEST "Add GTest support" ON)
option(WITH_GMOCK "Add GMock support" ON)
if(BUILD_TESTS OR WITH_GTEST)
  include(CTest)
  enable_testing()
endif()

option(WITH_CRASH_REPORTS, "Enable crash dump generation" ON)
option(WITH_COVERAGE_REPORTS, "Enable generating code coverage report" OFF)
option(WITH_MYSQL "Use MySQL connection" ON)

option(DEBUG "enable debug build" OFF)
option(OFFICIAL_BUILD "do you want to compile an official build?" OFF)

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

