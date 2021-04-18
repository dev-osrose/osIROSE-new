cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Prepare testing infrastructure.
link_directories(${GTEST_BINARY_DIR} ${GMOCK_BINARY_DIR} ${MYSQL_BINARY_DIR})

# Configure some handy variables.
set(PROJECT_LIB osirose-lib)

# Grab all other files that can be used in the tests
file(GLOB_RECURSE LOGIN_HEADERS . ${CMAKE_SOURCE_DIR}/src/login/*.[h])
file(GLOB_RECURSE CHAR_HEADERS . ${CMAKE_SOURCE_DIR}/src/char/*.[h])
file(GLOB_RECURSE MAP_HEADERS . ${CMAKE_SOURCE_DIR}/src/map/*.[h])

file(GLOB_RECURSE LOGIN_SOURCE . ${CMAKE_SOURCE_DIR}/src/login/[!main]*.[c][p][p])
file(GLOB_RECURSE CHAR_SOURCE . ${CMAKE_SOURCE_DIR}/src/char/[!main]*.[c][p][p])
file(GLOB_RECURSE MAP_SOURCE . ${CMAKE_SOURCE_DIR}/src/map/*.cpp)

#FIXME : this is a temporary fix until I find out the correct regexp for excluding main and not ^m*
file(GLOB MAP_MAIN . ${CMAKE_SOURCE_DIR}/src/map/src/main.cpp)

list(REMOVE_ITEM MAP_SOURCE ${MAP_MAIN})

set(SERVER_SOURCE
  ${LOGIN_SOURCE}
  ${CHAR_SOURCE}
  ${MAP_SOURCE}
)

set(HEADERS
  ${CORE_HEADERS}
  ${ROSE_HEADERS}
  ${LOGIN_HEADERS}
  ${CHAR_HEADERS}
  ${MAP_HEADERS}
  ${PROJECT_TESTING_HEADERS}
)

#build the lib
project(${PROJECT_LIB})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/login/include
  ${CMAKE_SOURCE_DIR}/src/char/include
  ${CMAKE_SOURCE_DIR}/src/map/include
)


add_library(${PROJECT_LIB} STATIC ${SERVER_SOURCE} ${HEADERS})
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/login/include>)
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/char/include>)
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/map/include>)
target_include_directories(${PROJECT_LIB} PUBLIC $<BUILD_INTERFACE:${SQLPP_INCLUDE_DIR}/../tests>)

target_compile_definitions(${PROJECT_LIB} PUBLIC SERVER_TESTING)

if (MSVC)
#  set_target_properties(${PROJECT_LIB} PROPERTIES COMPILE_FLAGS "/w")
endif()

add_dependencies(${PROJECT_LIB} RoseCommon)
target_link_libraries(${PROJECT_LIB} PUBLIC RoseCommon)
if (COMMAND cotire)
  cotire(${PROJECT_LIB})
endif()

SET_PROPERTY(TARGET ${PROJECT_LIB}                PROPERTY FOLDER "Tests")

#-----------------------------------------------------------------------------
# Unit test helper Macro
#-----------------------------------------------------------------------------
macro (compile_test name path)
  set(TEST_NAME Test${name})
  project(${TEST_NAME})

  include_directories(
    SYSTEM ${GTEST_INCLUDE_DIRS}
    SYSTEM ${GMOCK_INCLUDE_DIRS}
    SYSTEM ${BREAKPAD_EXCEPTION_HANDLER_INCLUDE_DIR}
  )

  add_executable(${TEST_NAME} main.cpp ${path})
  target_include_directories(${TEST_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )

  target_link_libraries(${TEST_NAME}
    PRIVATE
      ${COVERAGE_LIBRARIES}
  )

  if(WITH_MYSQL)
      target_link_libraries(${TEST_NAME}
        PUBLIC
          mysql::mysql
      )
  endif()

  add_dependencies(${TEST_NAME} ${PROJECT_LIB})
  target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_LIB})

  add_dependencies(${TEST_NAME} RoseCommon)
  target_link_libraries(${TEST_NAME}
    PRIVATE
      RoseCommon
      GTest::GTest
  )

  if (MSVC)
    set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS "/wd4503")
    target_link_libraries(${TEST_NAME} PRIVATE wsock32.lib)
  else()
    target_link_libraries(${TEST_NAME} PRIVATE mysqlclient)
  endif()

  if (COMMAND cotire)
    cotire(${TEST_NAME})
  endif()

  gtest_discover_tests(${TEST_NAME})

  if(WITH_CRASH_REPORTS)
    if (NOT CRASHPAD)
      generate_symbol_data(${TEST_NAME})
    else()
      generate_crashpad_symbol_data(${TEST_NAME})
    endif()
  endif()

  SET_PROPERTY(TARGET ${TEST_NAME}                PROPERTY FOLDER "Tests")
endmacro (compile_test)

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

compile_test(Config src/test_config.cpp)
#compile_test(Console src/test_logconsole.cpp)
#compile_test(MySQLDB src/test_mysql_database.cpp)
#compile_test(ASIONetwork src/test_network_asio.cpp)
#compile_test(RoseCrypt src/test_crosecrypt.cpp)
#compile_test(RoseNetwork src/test_crose_network.cpp)
#compile_test(LoginServer src/test_login.cpp)
#compile_test(CharServer src/test_char.cpp)
#compile_test(AllServers src/test_servers.cpp)
#compile_test(RosePackets src/test_crosepacket.cpp)
#compile_test(Systems src/test_system.cpp)
