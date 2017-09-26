cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT Core)
project(${PROJECT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")

IF (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
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
  include/factory
)

set( SOURCES_BASE
  src/logconsole
  src/cnetwork_asio
  src/config
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

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/osirose.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME osiroseDatabase
  OUTPATH ${CMAKE_BINARY_DIR}/generated
)

GENERATE_SQL_HEADER(OSIROSE_HEADER ${args})

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/logs.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME logDatabase
  OUTPATH ${CMAKE_BINARY_DIR}/generated
)

GENERATE_SQL_HEADER(LOGS_HEADER ${args})

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER} ${OSIROSE_HEADER} ${LOGS_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

IF (MSVC)
  set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS "/wd4503")
ENDIF()

IF (ENABLE_MYSQL)
  target_link_libraries(${PROJECT} sqlpp-mysql)
ENDIF()

