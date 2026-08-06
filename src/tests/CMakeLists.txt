cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(GoogleTest)

# Configure some handy variables.
set(PROJECT_LIB osirose-lib)

# Grab all other files that can be used in the tests.
file(GLOB_RECURSE LOGIN_HEADERS . ${CMAKE_SOURCE_DIR}/src/login/*.h)
file(GLOB_RECURSE CHAR_HEADERS . ${CMAKE_SOURCE_DIR}/src/char/*.h)
file(GLOB_RECURSE MAP_HEADERS . ${CMAKE_SOURCE_DIR}/src/map/*.h)

file(GLOB_RECURSE LOGIN_SOURCE . ${CMAKE_SOURCE_DIR}/src/login/*.cpp)
file(GLOB_RECURSE CHAR_SOURCE . ${CMAKE_SOURCE_DIR}/src/char/*.cpp)
file(GLOB_RECURSE MAP_SOURCE . ${CMAKE_SOURCE_DIR}/src/map/*.cpp)

# Each server's main() would give the test binaries a second entry point.
list(REMOVE_ITEM LOGIN_SOURCE ${CMAKE_SOURCE_DIR}/src/login/src/main.cpp)
list(REMOVE_ITEM CHAR_SOURCE ${CMAKE_SOURCE_DIR}/src/char/src/main.cpp)
list(REMOVE_ITEM MAP_SOURCE ${CMAKE_SOURCE_DIR}/src/map/src/main.cpp)

set(SERVER_SOURCE
  ${LOGIN_SOURCE}
  ${CHAR_SOURCE}
  ${MAP_SOURCE}
)

set(HEADERS
  ${LOGIN_HEADERS}
  ${CHAR_HEADERS}
  ${MAP_HEADERS}
)

#build the lib
project(${PROJECT_LIB})

add_library(${PROJECT_LIB} STATIC ${SERVER_SOURCE} ${HEADERS})
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/login/include>)
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/char/include>)
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/map/include>)

add_dependencies(${PROJECT_LIB} RoseCommon)
target_link_libraries(${PROJECT_LIB} PUBLIC RoseCommon)

# The map sources need Lua and curl; without these the first test that pulls in
# entity_system.o or any lua_*.cpp fails to link.
if(NOT LUA_FOUND)
  add_dependencies(${PROJECT_LIB} lua::lua)
endif()
target_link_libraries(${PROJECT_LIB} PUBLIC lua::lua CURL::libcurl)

SET_PROPERTY(TARGET ${PROJECT_LIB}                PROPERTY FOLDER "Tests")

#-----------------------------------------------------------------------------
# Unit test helper macros
#
# compile_test      - links the whole server lib. Use when the unit under test
#                     lives in a translation unit that reaches EntitySystem.
# compile_unit_test - compiles the named sources straight into the test binary
#                     and links only Core. Keeps pure tests hermetic and fast.
#-----------------------------------------------------------------------------
macro (_test_common TEST_NAME)
  target_include_directories(${TEST_NAME}
    PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )

  target_link_libraries(${TEST_NAME}
    PRIVATE
      ${COVERAGE_LIBRARIES}
      GTest::gtest
      GTest::gtest_main
  )

  if (MSVC)
    set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS "/wd4503")
    target_link_libraries(${TEST_NAME} PRIVATE wsock32.lib)
  endif()

  gtest_discover_tests(${TEST_NAME})

  SET_PROPERTY(TARGET ${TEST_NAME}                PROPERTY FOLDER "Tests")
endmacro (_test_common)

macro (compile_test name path)
  set(TEST_NAME Test${name})

  add_executable(${TEST_NAME} ${path})

  add_dependencies(${TEST_NAME} ${PROJECT_LIB})
  target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_LIB} RoseCommon)

  _test_common(${TEST_NAME})
endmacro (compile_test)

macro (compile_unit_test name)
  set(TEST_NAME Test${name})

  add_executable(${TEST_NAME} ${ARGN})

  add_dependencies(${TEST_NAME} Core)
  target_link_libraries(${TEST_NAME} PRIVATE Core)

  _test_common(${TEST_NAME})
endmacro (compile_unit_test)

#---------------------------------
#build the actual tests
if(WITH_COVERAGE_REPORTS)
  include(CodeCoverage)

  set(COVERAGE_IGNORE_LIST
    "/usr/*"
    "${CMAKE_SOURCE_DIR}/tools/*"
    "${CMAKE_SOURCE_DIR}/3rdparty/*"
    "${CMAKE_SOURCE_DIR}/src/tests/*"
    "${CMAKE_SOURCE_DIR}/src/rosecommon/*/packets/*"
  )
  SETUP_TARGET_FOR_COVERAGE(
    NAME coverage
    EXECUTABLE ctest coverage "--output-on-failure"
    EXCLUDES ${COVERAGE_IGNORE_LIST}
  )
endif() # WITH_COVERAGE_REPORTS

# Hermetic: unit under test compiled directly, no server lib, no database.
compile_unit_test(IdManager src/test_id_manager.cpp ${CMAKE_SOURCE_DIR}/src/map/src/id_manager.cpp)
compile_unit_test(EscapeData src/test_escape_data.cpp)
compile_unit_test(InventoryLayout src/test_inventory_layout.cpp)
compile_unit_test(Config src/test_config.cpp)
compile_unit_test(CrashReport src/test_crash_report.cpp)

# Core defines ENABLE_CRASH_REPORTS PRIVATE (crash_report.h is pimpl'd, so no
# consumer needs crashpad's headers). The test does need to know which of the
# two implementations it is linked against, so it can skip instead of passing
# vacuously - hence repeating the definition here rather than making it PUBLIC.
if (WITH_CRASH_REPORTS)
  target_compile_definitions(TestCrashReport PRIVATE ENABLE_CRASH_REPORTS)
endif ()

target_include_directories(TestIdManager PRIVATE ${CMAKE_SOURCE_DIR}/src/map/include)
target_include_directories(TestInventoryLayout PRIVATE ${CMAKE_SOURCE_DIR}/src/map/include)

# These reach translation units that transitively require the server lib.
compile_test(ExpCurve src/test_exp_curve.cpp)
compile_test(PartyUser src/test_party_user.cpp)
compile_test(LuaData src/test_lua_data.cpp)
