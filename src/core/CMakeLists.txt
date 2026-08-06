
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
    src/crash_report.cpp
    src/health_server.cpp
    )


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

# Crashpad stays PRIVATE: crash_report.h is pimpl'd, so nothing outside
# crash_report.cpp needs crashpad's headers or libraries. The servers only need
# crashpad_handler to exist next to them at runtime.
if (WITH_CRASH_REPORTS)
  target_compile_definitions(${PROJECT} PRIVATE ENABLE_CRASH_REPORTS)
  target_link_libraries(${PROJECT}
      PRIVATE
      utils::crashpad
      )
  # Nothing links the handler, so make sure it is still built and copied.
  add_dependencies(${PROJECT} crashpad_handler)
endif ()

if (WITH_MARIADB)
  add_dependencies(${PROJECT} sqlpp23::mariadb)
  target_link_libraries(${PROJECT}
      PUBLIC
      sqlpp23::mariadb
      )
elseif (WITH_MYSQL)
  add_dependencies(${PROJECT} sqlpp23::mysql)
  target_link_libraries(${PROJECT}
      PUBLIC
      sqlpp23::mysql
      )
endif ()

set_property(TARGET ${PROJECT} PROPERTY FOLDER "Servers")
