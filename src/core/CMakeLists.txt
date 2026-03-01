
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT Core)
project(${PROJECT})

set(SOURCES_HEADER
    include/config.h
    include/connectionpool.h
    include/connection.h
    include/crash_report.h
    include/health_server.h
    )

set(SOURCES_BASE
    src/config.cpp
    src/connection.cpp
    src/health_server.cpp
    )

if (GCC OR CLANG)
  list(APPEND SOURCES_HEADER
      include/unix/crash_report.h
      )

  list(APPEND SOURCES_BASE
      src/unix/crash_report.cpp
      )
else ()
  list(APPEND SOURCES_HEADER
      include/win32/crash_report.h
      )
  list(APPEND SOURCES_BASE
      src/win32/crash_report.cpp
      )
endif ()


if (WITH_MYSQL OR WITH_MARIADB)
  list(APPEND SOURCES_HEADER
      include/mysqlconnection.h
      )
  list(APPEND SOURCES_BASE
      src/mysqlconnection.cpp
      )
endif ()

set(args
    SQLFILE ${CMAKE_SOURCE_DIR}/schema/migrations/01-osirose.sql
    SQLROOT ${CMAKE_SOURCE_DIR}/schema
    NAME osiroseDatabase
    OUTPATH ${CMAKE_GEN_SRC_DIR}
    )

generate_sql_header(OSIROSE_HEADER ${args})

set(args
    SQLFILE ${CMAKE_SOURCE_DIR}/schema/migrations/01-logs.sql
    SQLROOT ${CMAKE_SOURCE_DIR}/schema
    NAME logDatabase
    OUTPATH ${CMAKE_GEN_SRC_DIR}
    )

generate_sql_header(LOGS_HEADER ${args})

add_library(${PROJECT} STATIC ${OSIROSE_HEADER} ${LOGS_HEADER} ${SOURCES_BASE} ${SOURCES_HEADER})
target_include_directories(${PROJECT}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_GEN_SRC_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

# Make sure the SQL headers are created before core is created
add_dependencies(${PROJECT} osiroseDatabase_generated logDatabase_generated)

add_dependencies(${PROJECT} osirose_common::osirose_common)
target_link_libraries(${PROJECT}
    PUBLIC
    osirose_common::osirose_common
)

target_link_libraries(${PROJECT}
    INTERFACE
    ${CMAKE_THREAD_LIBS_INIT}
    )

if (NOT CURL_FOUND)
  add_dependencies(${PROJECT} CURL::libcurl)
endif ()
target_link_libraries(${PROJECT}
    INTERFACE
    CURL::libcurl
    )

if (MINGW)
  target_link_libraries(${PROJECT}
      PUBLIC
      wsock32 ws2_32
      )
endif ()

if (WITH_MARIADB)
  add_dependencies(${PROJECT} sqlpp11::mariadb)
  target_link_libraries(${PROJECT}
      PUBLIC
      sqlpp11::mariadb
      )
elseif (WITH_MYSQL)
  add_dependencies(${PROJECT} sqlpp11::mysql)
  target_link_libraries(${PROJECT}
      PUBLIC
      sqlpp11::mysql
      )
endif ()

set_property(TARGET ${PROJECT} PROPERTY FOLDER "Servers")
