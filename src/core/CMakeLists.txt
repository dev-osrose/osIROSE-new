
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Core)
project(${PROJECT})

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

IF (WITH_MYSQL)
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
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

GENERATE_SQL_HEADER(SQL_HEADER ${args})

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER} ${SQL_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

IF (WITH_MYSQL)
  target_link_libraries(${PROJECT} sqlpp-mysql)
ENDIF()

