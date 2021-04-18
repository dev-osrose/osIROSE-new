
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT NodeServer)

project(${PROJECT})

if (GCC)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
  add_linker_flags(-Wl,-rpath -Wl,./)
endif()

set( SOURCES_MAIN
  src/main.cpp )

set( SOURCES_HEADER
  include/nodeserver.h
  include/nodeclient.h
  include/nodesessions.h)

set( SOURCES_BASE
  src/nodeserver.cpp
  src/nodeclient.cpp
  src/nodesessions.cpp)

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/nodeSessions.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME nodeSessionDB
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

generate_sql_header(SESSIONS_HEADER ${args})

add_executable(${PROJECT} ${SESSIONS_HEADER} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_dependencies(${PROJECT} sqlite3::sqlite3 SQLPP11::sqlpp11_sqlite3)
target_link_libraries(${PROJECT}
  PUBLIC
    sqlite3::sqlite3
    SQLPP11::sqlpp11_sqlite3
    osirose_common::osirose_common
)

if (NOT CURL_FOUND)
  add_dependencies(${PROJECT} curl)
endif ()
target_link_libraries(${PROJECT}
    INTERFACE
    CURL::libcurl
    )

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT}
  PRIVATE
    RoseCommon
)

if(WITH_CRASH_REPORTS)
  if (NOT CRASHPAD)
    generate_symbol_data(${PROJECT})
  else()
    generate_crashpad_symbol_data(${PROJECT})
  endif()
endif()

install(TARGETS ${PROJECT}
        RUNTIME
          DESTINATION bin
          COMPONENT Runtime)

set_target_properties(${PROJECT} PROPERTIES COTIRE_PREFIX_HEADER_IGNORE_PATH "${COTIRE_PREFIX_IGNORE_PATHS};${CMAKE_CURRENT_SOURCE_DIR}/include/")

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
