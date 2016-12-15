cmake_minimum_required(VERSION 2.6)

# Prepare testing infrastructure.
link_directories(${EXTERNAL_BINARY_DIR} ${GTEST_BINARY_DIR} ${GMOCK_BINARY_DIR} ${MYSQL_BINARY_DIR})

# Configure some handy variables.
set(PROJECT osirose-tests)
set(CMAKE_BUILD_TYPE "Debug")

IF(ENABLE_MYSQL_ONE_INSTANCE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MYSQL_ONE_INSTANCE")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
set(GCC_LINK_FLAGS "${GCC_LINK_FLAGS}")
set(Clang_LINK_FLAGS "")
set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

add_definitions(-DSERVER_TESTING)

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

set(SOURCE_MAIN main)

set(SOURCES_BASE
  src/test_config
  src/test_logconsole
  src/test_mysql_database
  src/test_network_asio
  src/test_crosecrypt
  src/test_crose_network
  src/test_login
  src/test_char
  src/test_servers
  src/test_crosepacket
)

set(SOURCES
  ${SERVER_SOURCE}
  ${SOURCE_MAIN}
  ${SOURCES_BASE}
)

project(${PROJECT})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/login/include
  ${CMAKE_SOURCE_DIR}/src/char/include
  ${CMAKE_SOURCE_DIR}/src/map/include
)

find_package( Threads REQUIRED )

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

IF(ENABLE_TESTING_COVERAGE)
  include(CodeCoverage)
  
  set(COVERAGE_IGNORE_LIST 
    'tools/*'
    '3rdparty/*'
    'protobuf/*'
    '*.pb.*'
    'rapidjson/*'
    'pbjson.*'
    'bin2ascii.h'
  )
    
  SETUP_TARGET_FOR_COVERAGE(coverage ctest "coverage --output-on-failure")
ENDIF() # ENABLE_TESTING_COVERAGE

IF(ENABLE_MYSQL)
  target_link_libraries(${PROJECT} mysqlpp)
ENDIF()

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} gtest)
target_link_libraries(${PROJECT} gmock)
target_link_libraries(${PROJECT} ${COVERAGE_LIBRARIES})

IF (MSVC)
  target_link_libraries(${PROJECT} libprotobuf)
  target_link_libraries(${PROJECT} wsock32.lib)
ELSE()
  target_link_libraries(${PROJECT} ${PROTOBUF_LIBRARY})
  target_link_libraries(${PROJECT} mysqlclient)
ENDIF()

add_test(NAME SetupTesting COMMAND osirose-tests --gtest_filter=SetupTesting.*)
add_test(NAME TestConfig COMMAND osirose-tests --gtest_filter=Config.*)
add_test(NAME TestNetworking COMMAND osirose-tests --gtest_filter=Test*Network*.*)
add_test(NAME TestDatabases COMMAND osirose-tests --gtest_filter=Test*Database.*)
add_test(NAME TestServers COMMAND osirose-tests --gtest_filter=Test*Server*.*)
