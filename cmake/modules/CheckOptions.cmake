#------------------------------------------------------------------------------
# global options
#------------------------------------------------------------------------------

option(PROJECT_DEBUG "Enable debugging messages support" ON)

option(WITH_COTIRE "Enable cotire pch and unity build targets" ON)
option(OFFICIAL_BUILD "do you want to compile an official build?" OFF)
option(BUILD_LUA "Build Lua" ON)
option(BUILD_API_DOC "Build doxygen documentation" OFF)
option(ENABLE_ASAN "Enable address sanitizer" OFF)

set(DEV_SETTING ON)
if(OFFICIAL_BUILD)
  message(STATUS "Official Build is set - Turning off dev options")
  set(DEV_SETTING OFF)
endif()

option(BUILD_TOOLS "Build server tools" OFF)
option(BUILD_TESTS "Build various unit tests." ${DEV_SETTING})
option(WITH_GTEST "Add GTest support" ${DEV_SETTING})
option(WITH_GMOCK "Add GMock support" ${DEV_SETTING})
if(BUILD_TESTS OR WITH_GTEST)
  include(CTest)
  enable_testing()
  
  option(WITH_COVERAGE_REPORTS "Enable generating code coverage report" OFF)
endif()

option(WITH_CRASH_REPORTS "Enable crash dump generation" ON)
option(WITH_MYSQL "Use MySQL connection" ON)
option(DEBUG "enable debug build" OFF)

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

