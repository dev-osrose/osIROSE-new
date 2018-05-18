
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Core)
project(${PROJECT})

set( SOURCES_HEADER
  include/threadpool.h
  include/logconsole.h
  include/cnetwork_asio.h
  include/config.h
  include/logconsole.h
  include/network_thread_pool.h
  include/inetwork.h
  include/connectionpool.h
  include/connection.h
  include/singleton.h
  include/riterator.h
  include/platform_defines.h
  include/iscontainer.h
  include/makevector.h
  include/factory.h
  include/crash_report.h
  include/hash.h
)

set( SOURCES_BASE
  src/logconsole.cpp
  src/cnetwork_asio.cpp
  src/config.cpp
  src/connection.cpp
)

if( GCC OR CLANG )
  list(APPEND SOURCES_HEADER
    include/unix/crash_report.h
  )

  list(APPEND SOURCES_BASE
    src/unix/crash_report.cpp
  )
else()
  list(APPEND SOURCES_HEADER
    include/win32/crash_report.h
  )
  list(APPEND SOURCES_BASE
    src/win32/crash_report
  )
endif()


if (WITH_MYSQL)
  list(APPEND SOURCES_HEADER
    include/mysqlconnection.h
  )
  list(APPEND SOURCES_BASE
    src/mysqlconnection.cpp
  )
endif()

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/osirose.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME osiroseDatabase
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

GENERATE_SQL_HEADER(OSIROSE_HEADER ${args})

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/logs.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME logDatabase
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

GENERATE_SQL_HEADER(LOGS_HEADER ${args})

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER} ${OSIROSE_HEADER} ${LOGS_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(WITH_CRASH_REPORTS)
  target_link_libraries(${PROJECT} Breakpad::Breakpad)
endif()

if (WITH_MYSQL)
  target_link_libraries(${PROJECT} sqlpp-mysql)
endif()

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
