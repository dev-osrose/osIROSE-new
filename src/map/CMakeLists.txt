
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT MapServer)
project(${PROJECT})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

SET( SOURCES_HEADER
  include/cmapserver.h
  include/cmapclient.h
  include/cmapisc.h
  include/entitysystem.h
  include/systemmanager.h
  include/id_manager.h
  include/script_loader.h
  include/map_manager.h
  include/gm_commands.h
  include/systems/system.h
  include/systems/movementsystem.h
  include/systems/updatesystem.h
  include/systems/chatsystem.h
  include/systems/inventorysystem.h
  include/systems/partysystem.h
  include/systems/mapsystem.h
  include/systems/callbacksystem.h
  include/systems/combat_system.h
  )

SET( SOURCES_MAIN
  src/main
  src/cmapserver.cpp
  src/cmapclient.cpp
  src/cmapisc.cpp
  src/entitysystem.cpp
  src/systemmanager.cpp
  src/id_manager.cpp
  src/script_loader.cpp
  src/map_manager.cpp
  src/gm_commands.cpp
  src/systems/movementsystem.cpp
  src/systems/updatesystem.cpp
  src/systems/chatsystem.cpp
  src/systems/inventorysystem.cpp
  src/systems/partysystem.cpp
  src/systems/mapsystem.cpp
  src/systems/callbacksystem.cpp
  src/systems/combat_system.cpp
  )

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} CURL::libcurl)
if(WITH_MYSQL)
  target_link_libraries(${PROJECT} mysql::mysql)
endif()


if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
  target_link_libraries(${PROJECT} wsock32.lib)
else()
    if (WITH_MYSQL)
      target_link_libraries(${PROJECT} mysqlclient)
    endif()
endif()

if(WITH_CRASH_REPORTS)
  target_link_libraries(${PROJECT} Breakpad::Breakpad)
endif()

add_dependencies(${PROJECT} RoseCommon)
target_link_libraries(${PROJECT} RoseCommon)

set_target_properties(${PROJECT} PROPERTIES COTIRE_PREFIX_HEADER_IGNORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")

generate_symbol_data(${PROJECT})
if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
