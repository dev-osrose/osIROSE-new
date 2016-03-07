cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

INCLUDE_DIRECTORIES(
	SYSTEM ${PROTOBUF_INCLUDE_DIR}
	SYSTEM ${ASIO_INCLUDE_DIR}
	SYSTEM ${EXTERNAL_INCLUDE_DIR}
	SYSTEM ${MYSQL_INCLUDE_DIRS}
	SYSTEM ${MYSQLpp_INCLUDE_DIR}
)

LINK_DIRECTORIES(${EXTERNAL_BINARY_DIR} ${PROTOBUF_BINARY_DIR} ${MYSQL_BINARY_DIR})

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(PROJECT Core)
project(${PROJECT})

#SET(CMAKE_BUILD_TYPE $ENV{BUILD_TYPE})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

#foreach(flag_var
#        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
#        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
#   if(${flag_var} MATCHES "/MD")
#      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#   endif(${flag_var} MATCHES "/MD")
#endforeach(flag_var)

ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()
 
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src/proto/include
)

SET( SOURCES_HEADER
  include/logconsole
  include/cnetwork_asio
  include/config 
  include/pbjson
  include/logconsole
  include/cmysql_database
  include/network_thread_pool
  include/idatabase
  include/inetwork
  include/riterator
)

SET( SOURCES_BASE
  src/logconsole
  src/cnetwork_asio
  src/config
  src/pbjson.cpp
  src/logconsole
  src/cmysql_database
)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	ADD_DEPENDENCIES(${PROJECT} protoGen)
#ELSE()
#	TARGET_LINK_LIBRARIES(${PROJECT} ${PROTOBUF_BINARY_DIR}/libprotobuf.a)
ENDIF()

