
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
  include/platform_defines.h
  include/factory.h
  include/crash_report.h
  include/hash.h
  include/fuzzy_matching.h
  include/random.h
  include/mwsrqueue.h
  include/enumerate.h
  include/transform.h
  include/fire_once.h
  include/tuple_iterate.h
)

set( SOURCES_BASE
  src/logconsole.cpp
  src/cnetwork_asio.cpp
  src/config.cpp
  src/connection.cpp
  src/fuzzy_matching.cpp
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

generate_sql_header(OSIROSE_HEADER ${args})

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/logs.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME logDatabase
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

generate_sql_header(LOGS_HEADER ${args})

add_library(${PROJECT} STATIC ${OSIROSE_HEADER} ${LOGS_HEADER} ${SOURCES_BASE} ${SOURCES_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Make sure the SQL headers are created before core is created
add_dependencies(${PROJECT} osiroseDatabase_generated logDatabase_generated)

if(NOT LUA_FOUND)
  add_dependencies(${PROJECT} lua::lua)
endif()

if(NOT CURL_FOUND)
  add_dependencies(${PROJECT} curl)
endif()

if(WITH_CRASH_REPORTS)
  if(NOT BREAKPAD_FOUND)
    add_dependencies(${PROJECT} breakpad-build)
  endif()
  
  target_link_libraries(${PROJECT} Breakpad::Breakpad)
endif()

if (MINGW)
  target_link_libraries(${PROJECT} wsock32 ws2_32)
elseif (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
endif()

if (WITH_MYSQL)
  add_dependencies(${PROJECT} SQLPP11::SQLPP11_MYSQL)
  target_link_libraries(${PROJECT} SQLPP11::SQLPP11_MYSQL)
endif()

add_dependencies(${PROJECT} sqlite3::sqlite3)
target_link_libraries(${PROJECT} sqlite3::sqlite3)
add_dependencies(${PROJECT} SQLPP11::sqlpp11_sqlite3)
target_link_libraries(${PROJECT} SQLPP11::sqlpp11_sqlite3)

set_target_properties(${PROJECT} PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER FALSE)
if (COMMAND cotire)
  cotire(${PROJECT})
endif()

set_property(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
