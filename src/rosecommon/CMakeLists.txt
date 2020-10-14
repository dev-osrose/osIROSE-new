
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT RoseCommon)
project(${PROJECT})

set( SOURCES_HEADER
  include/crosesocket.h
  include/croseserver.h
  include/croseclient.h
  include/croseisc.h
  include/itemdb.h
  include/components/basic_info.h
  include/components/combat.h
  include/components/computed_values.h
  include/components/destination.h
  include/components/faction.h
  include/components/character_graphics.h
  include/components/guild.h
  include/components/hotbar.h
  include/components/inventory.h
  include/components/item.h
  include/components/level.h
  include/components/life.h
  include/components/lua.h
  include/components/magic.h
  include/components/mob.h
  include/components/npc.h
  include/components/owner.h
  include/components/pat.h
  include/components/position.h
  include/components/skills.h
  include/components/stamina.h
  include/components/stats.h
  include/components/status_effects.h
  include/components/target.h
  include/components/warpgate.h
  include/components/wishlist.h
)

SET(SOURCES_BASE
  src/crosesocket.cpp
  src/croseserver.cpp
  src/croseclient.cpp
  src/croseisc.cpp
  src/itemdb.cpp
)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_HEADER})
target_include_directories(${PROJECT}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/components>
)

if (NOT MSVC)
  target_compile_options(${PROJECT} PRIVATE -Werror)
endif()

add_dependencies(${PROJECT} Core)
add_dependencies(${PROJECT} Core osirose_common::osirose_common)
target_link_libraries(${PROJECT}
  PUBLIC
    Core
    osirose_common::osirose_common
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES_BASE})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${SOURCES_HEADER})

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
