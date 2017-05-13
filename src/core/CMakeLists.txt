cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR} ${PROTOBUF_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT Core)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

set( SOURCES_HEADER
  include/threadpool
  include/logconsole
  include/cnetwork_asio
  include/config
  ${PBJSON_ROOT}/pbjson.hpp
  include/logconsole
  include/network_thread_pool
  include/inetwork
  include/connectionpool
  include/connection
  include/singleton
  include/riterator
  include/platform_defines
  include/iscontainer
  include/makevector
)

set( SOURCES_BASE
  src/logconsole
  src/cnetwork_asio
  src/config
  ${PBJSON_ROOT}/pbjson.cpp
  src/connection
)

IF (ENABLE_MYSQL)
    list(APPEND SOURCES_HEADER
        include/mysqlconnection
        )
    list(APPEND SOURCES_BASE
        src/mysqlconnection
        )
ENDIF()

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT} protoGen)
IF (ENABLE_MYSQL)
    target_link_libraries(${PROJECT} sqlpp-mysql)
ENDIF()

