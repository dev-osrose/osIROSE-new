
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT NodeServer)

project(${PROJECT})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

set( SOURCES_MAIN
  src/main )

set( SOURCES_HEADER
  include/nodeserver.h
  include/nodeclient.h
  include/nodeisc.h
  include/nodesessions.h)

set( SOURCES_BASE
  src/nodeserver.cpp
  src/nodeclient.cpp
  src/nodeisc.cpp
  src/nodesessions.cpp)

set(args
  SQLFILE ${CMAKE_SOURCE_DIR}/Database/nodeSessions.sql
  SQLROOT ${CMAKE_SOURCE_DIR}/Database
  NAME nodeSessions
  OUTPATH ${CMAKE_GEN_SRC_DIR}
)

generate_sql_header(SESSIONS_HEADER ${args})

add_executable(${PROJECT} ${SESSIONS_HEADER} ${SOURCES_MAIN} ${SOURCES_BASE} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} CURL::libcurl)

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
endif()

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} RoseCommon)

add_dependencies(${PROJECT} SQLPP11::sqlpp11_sqlite3)
target_link_libraries(${PROJECT} SQLPP11::sqlpp11_sqlite3)

generate_symbol_data(${PROJECT})

set_property (DIRECTORY PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/nodesessions.h")

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
