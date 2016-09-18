CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Prepare testing infrastructure.
LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR} ${GTEST_BINARY_DIR} ${GMOCK_BINARY_DIR} ${MYSQL_BINARY_DIR})

# Configure some handy variables.
SET(PROJECT osirose-tests)
SET(CMAKE_BUILD_TYPE "Debug")

IF(ENABLE_MYSQL_ONE_INSTANCE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MYSQL_ONE_INSTANCE")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
set(GCC_LINK_FLAGS "${GCC_LINK_FLAGS}")
set(Clang_LINK_FLAGS "")
set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

ADD_DEFINITIONS(-DSERVER_TESTING)

file(GLOB_RECURSE CORE_HEADERS . ${CMAKE_SOURCE_DIR}/src/core/*.[h])
file(GLOB_RECURSE ROSE_HEADERS . ${CMAKE_SOURCE_DIR}/src/rosecommon/*.[h])
file(GLOB_RECURSE LOGIN_HEADERS . ${CMAKE_SOURCE_DIR}/src/login/*.[h])
file(GLOB_RECURSE CHAR_HEADERS . ${CMAKE_SOURCE_DIR}/src/char/*.[h])
file(GLOB_RECURSE MAP_HEADERS . ${CMAKE_SOURCE_DIR}/src/map/*.[h])
file(GLOB_RECURSE PROJECT_TESTING_HEADERS . *.[h])

file(GLOB_RECURSE LOGIN_SOURCE . ${CMAKE_SOURCE_DIR}/src/login/[!main]*.[c][p][p])
file(GLOB_RECURSE CHAR_SOURCE . ${CMAKE_SOURCE_DIR}/src/char/[!main]*.[c][p][p])
file(GLOB_RECURSE MAP_SOURCE . ${CMAKE_SOURCE_DIR}/src/map/[!main]*.[c][p][p])

SET(SERVER_SOURCE
   ${LOGIN_SOURCE}
   ${CHAR_SOURCE}
   ${MAP_SOURCE}
)

SET(HEADERS
    ${CORE_HEADERS}
    ${ROSE_HEADERS}
    ${LOGIN_HEADERS}
    ${CHAR_HEADERS}
    ${MAP_HEADERS}
    ${PROJECT_TESTING_HEADERS}
)

#message(STATUS "HEADERS=${HEADERS}")
#message(STATUS "SERVER_SOURCE=${SERVER_SOURCE}")

SET(SOURCE_MAIN main)

SET(SOURCES_BASE
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

SET(SOURCES
    ${SERVER_SOURCE}
    ${SOURCE_MAIN}
    ${SOURCES_BASE}
)

PROJECT(${PROJECT})

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src/proto/include
    ${CMAKE_SOURCE_DIR}/src/core/include
    ${CMAKE_SOURCE_DIR}/src/rosecommon/include
    ${CMAKE_SOURCE_DIR}/src/login/include
    ${CMAKE_SOURCE_DIR}/src/char/include
    ${CMAKE_SOURCE_DIR}/src/map/include
    ${CMAKE_SOURCE_DIR}/tools/
    ${CMAKE_SOURCE_DIR}/src/tests
)

FIND_PACKAGE( Threads REQUIRED )

ADD_EXECUTABLE(${PROJECT} ${HEADERS} ${SOURCES})

ADD_DEPENDENCIES(${PROJECT} gtest)
ADD_DEPENDENCIES(${PROJECT} gmock)
ADD_DEPENDENCIES(${PROJECT} Core)
ADD_DEPENDENCIES(${PROJECT} RoseCommon)
ADD_DEPENDENCIES(${PROJECT} protoGen)
ADD_DEPENDENCIES(${PROJECT} lua)
IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#	ADD_DEPENDENCIES(${PROJECT} libprotobuf)
	ADD_DEPENDENCIES(${PROJECT} mysqlpp)
ENDIF()

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINK_FLAGS
                "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

IF(ENABLE_TESTING_COVERAGE)
    INCLUDE(CodeCoverage)
    SETUP_TARGET_FOR_COVERAGE(coverage ${PROJECT} coverage)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF() # ENABLE_TESTING_COVERAGE

IF(ENABLE_MYSQL)
	TARGET_LINK_LIBRARIES(${PROJECT}
			${MYSQL_LIBRARIES}
        )
ENDIF()

TARGET_LINK_LIBRARIES(${PROJECT}
    ${CMAKE_THREAD_LIBS_INIT}
    Core
    RoseCommon
    protoGen
    mysqlpp
    lua
    gtest
    gmock
    ${COVERAGE_LIBRARIES}
)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	TARGET_LINK_LIBRARIES(${PROJECT} libprotobuf
		wsock32.lib 
	)
ELSE()
	TARGET_LINK_LIBRARIES(${PROJECT} 
		protobuf
		mysqlclient
		)
ENDIF()

add_test(NAME SetupTesting
         COMMAND osirose-tests --gtest_filter=SetupTesting.*)
add_test(NAME TestConfig
         COMMAND osirose-tests --gtest_filter=Config.*)
add_test(NAME TestNetworking
         COMMAND osirose-tests --gtest_filter=Test*Network*.*)
add_test(NAME TestDatabases
         COMMAND osirose-tests --gtest_filter=Test*Database.*)
add_test(NAME TestServers
         COMMAND osirose-tests --gtest_filter=Test*Server*.*)
