cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR} ${PROTOBUF_BINARY_DIR} ${MYSQL_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT Core)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF(ENABLE_MYSQL_ONE_INSTANCE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MYSQL_ONE_INSTANCE")
ENDIF()

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat -stdlib=libc++")
ENDIF()

set( SOURCES_HEADER
  include/logconsole
  include/cnetwork_asio
  include/config 
  ${PBJSON_ROOT}/pbjson.hpp
  include/logconsole
  include/cmysql_database
  include/network_thread_pool
  include/idatabase
  include/inetwork
  include/idatabasepool
  include/cmysql_databasepool
  include/singleton
  include/riterator
  include/platform_defines
)

set( SOURCES_BASE
  src/logconsole
  src/cnetwork_asio
  src/config
  ${PBJSON_ROOT}/pbjson.cpp
  src/cmysql_database
)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT} protoGen)
target_link_libraries(${PROJECT} mysqlpp)

