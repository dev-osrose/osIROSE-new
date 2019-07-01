
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT NodeServer)

project(${PROJECT})

if (GCC)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
  add_linker_flags(-Wl,-rpath -Wl,./)
endif()

set( SOURCES_MAIN
  src/main )

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
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

add_dependencies(${PROJECT} RosePackets_generated)

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} CURL::libcurl)


if (MINGW)
  target_link_libraries(${PROJECT} wsock32 ws2_32)
elseif (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
endif()

if(WITH_CRASH_REPORTS)
  target_link_libraries(${PROJECT} Breakpad::Breakpad)
endif()

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} RoseCommon)

target_link_libraries(${PROJECT} sqlite3::sqlite3)
target_link_libraries(${PROJECT} SQLPP11::sqlpp11_sqlite3)

if(WITH_CRASH_REPORTS)
  generate_symbol_data(${PROJECT})
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
