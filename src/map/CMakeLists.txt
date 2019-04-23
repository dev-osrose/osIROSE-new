
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT MapServer)
project(${PROJECT})

if (GCC)
  add_compiler_flags(-Wl,-Map=${PROJECT}.map)
endif()

SET( SOURCES_HEADER
  include/cmapserver.h
  include/cmapclient.h
  include/cmapisc.h
  include/id_manager.h
  include/map_manager.h
  include/entity_system.h
  include/nearby.h
  include/gm_commands.h
  include/chat/normal_chat.h
  include/chat/shout_chat.h
  include/chat/whisper_chat.h
  include/combat/combat.h
  include/map/change_map.h
  include/mouse/mouse_cmd.h
  include/lua_api.h
  include/lua_db.h
  include/lua_item.h
  include/lua_loader.h
  include/lua_storage.h
  include/components/spawner.h
  include/items/inventory.h
  )

SET( SOURCES_MAIN
  src/main
  src/cmapserver.cpp
  src/cmapclient.cpp
  src/cmapisc.cpp
  src/id_manager.cpp
  src/map_manager.cpp
  src/entity_system.cpp
  src/nearby.cpp
  src/gm_commands.cpp
  src/chat/normal_chat.cpp
  src/chat/shout_chat.cpp
  src/chat/whisper_chat.cpp
  src/combat/combat.cpp
  src/map/change_map.cpp
  src/mouse/mouse_cmd.cpp
  src/lua_db.cpp
  src/lua_item.cpp
  src/lua_loader.cpp
  src/lua_storage.cpp
  src/items/inventory.cpp
  )

add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")

add_dependencies(${PROJECT} RosePackets_generated)

target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT} CURL::libcurl)

if(WITH_MYSQL)
  target_link_libraries(${PROJECT} mysql::mysql)
endif()

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

set_target_properties(${PROJECT} PROPERTIES COTIRE_PREFIX_HEADER_IGNORE_PATH "${COTIRE_PREFIX_IGNORE_PATHS};${CMAKE_CURRENT_SOURCE_DIR}/include/")

if(WITH_CRASH_REPORTS)
  generate_symbol_data(${PROJECT})
endif()

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
