
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT RoseCommon)
project(${PROJECT})


if (NOT MSVC)
  add_compiler_flags(-Werror)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
)

set(packets
    cli_acceptreq.fbs
)

set(args
    FLATBUFFERSFILE ${packets}
    FLATBUFFERSROOT "${CMAKE_CURRENT_SOURCE_DIR}/packets"
    OUTPATH ${CMAKE_GEN_SRC_DIR}
)

generate_flatbuffers_headers(FLATBUFFERS ${args})

set( SOURCES_HEADER
  include/crosesocket.h
  include/crosecrypt.h
  include/croseserver.h
  include/croseclient.h
  include/croseisc.h
  include/crosepacket.h
  include/epackettype.h
  include/isccommon.h
  include/item.h
  include/itemdb.h
  include/partybase.h
  include/luaapi.h
  include/itemapi.h
  include/entityapi.h
  include/components/advancedinfo.h
  include/components/basicinfo.h
  include/components/bulletitems.h
  include/components/charactergraphics.h
  include/components/characterinfo.h
  include/components/destination.h
  include/components/graphics.h
  include/components/hotbar.h
  include/components/inventory.h
  include/components/position.h
  include/components/ridingitems.h
  include/components/skills.h
  include/components/socketconnector.h
  include/components/stats.h
  include/components/statuseffects.h
  include/components/wishlist.h
  include/components/quests.h
  include/components/npc.h
  include/components/warpgate.h
  include/components/spawner.h
  include/components/callbacks.h
  include/components/buffs.h
  include/components/damage.h
  include/components/playerspawn.h
)

SET(SOURCES_BASE
  src/crosesocket.cpp
  src/crosecrypt.cpp
  src/croseserver.cpp
  src/croseclient.cpp
  src/croseisc.cpp
  src/item.cpp
  src/itemdb.cpp
  src/entitycomponents.cpp
  src/isccommon.cpp
  src/itemapi.cpp
)

SET(SOURCES_PACKETS
  src/packets/cli_acceptreq.cpp
  include/packets/cli_acceptreq.h
  src/packets/cli_alive.cpp
  include/packets/cli_alive.h
  src/packets/cli_changemapreq.cpp
  include/packets/cli_changemapreq.h
  src/packets/cli_channellistreq.cpp
  include/packets/cli_channellistreq.h
  src/packets/cli_createcharreq.cpp
  include/packets/cli_createcharreq.h
  src/packets/cli_deletecharreq.cpp
  include/packets/cli_deletecharreq.h
  src/packets/cli_joinserverreq.cpp
  include/packets/cli_joinserverreq.h
  src/packets/cli_loginreq.cpp
  include/packets/cli_loginreq.h
  src/packets/cli_logoutreq.cpp
  include/packets/cli_logoutreq.h
  src/packets/cli_mousecmd.cpp
  include/packets/cli_mousecmd.h
  src/packets/cli_normalchat.cpp
  include/packets/cli_normalchat.h
  src/packets/cli_revivereq.cpp
  include/packets/cli_revivereq.h
  src/packets/cli_screenshottimereq.cpp
  include/packets/cli_screenshottimereq.h
  src/packets/cli_selectcharreq.cpp
  include/packets/cli_selectcharreq.h
  src/packets/cli_srvselectreq.cpp
  include/packets/cli_srvselectreq.h
  src/packets/cli_stop.cpp
  include/packets/cli_stop.h
  src/packets/cli_stopmoving.cpp
  include/packets/cli_stopmoving.h
  src/packets/cli_whisperchat.cpp
  include/packets/cli_whisperchat.h
  src/packets/cli_equipitem.cpp
  include/packets/cli_equipitem.h
  src/packets/cli_partyreq.cpp
  include/packets/cli_partyreq.h
  src/packets/cli_partyreply.cpp
  include/packets/cli_partyreply.h
  src/packets/cli_partychat.cpp
  include/packets/cli_partychat.h
  src/packets/cli_charlistreq.cpp
  include/packets/cli_charlistreq.h
  src/packets/cli_dropitem.cpp
  include/packets/cli_dropitem.h
  src/packets/cli_attack.cpp
  include/packets/cli_attack.h
  src/packets/cli_hpreq.cpp
  include/packets/cli_hpreq.h
  #------------------------------------------------
  src/packets/srv_acceptreply.cpp
  include/packets/srv_acceptreply.h
  src/packets/srv_changemapreply.cpp
  include/packets/srv_changemapreply.h
  src/packets/srv_createcharreply.cpp
  include/packets/srv_createcharreply.h
  src/packets/srv_deletecharreply.cpp
  include/packets/srv_deletecharreply.h
  src/packets/srv_initdata.cpp
  include/packets/srv_initdata.h
  src/packets/srv_inventorydata.cpp
  include/packets/srv_inventorydata.h
  src/packets/srv_joinserverreply.cpp
  include/packets/srv_joinserverreply.h
  src/packets/srv_logoutreply.cpp
  include/packets/srv_logoutreply.h
  src/packets/srv_mousecmd.cpp
  include/packets/srv_mousecmd.h
  src/packets/srv_normalchat.cpp
  include/packets/srv_normalchat.h
  src/packets/srv_playerchar.cpp
  include/packets/srv_playerchar.h
  src/packets/srv_removeobject.cpp
  include/packets/srv_removeobject.h
  src/packets/srv_screenshottimereply.cpp
  include/packets/srv_screenshottimereply.h
  src/packets/srv_selectcharreply.cpp
  include/packets/srv_selectcharreply.h
  src/packets/srv_serverdata.cpp
  include/packets/srv_serverdata.h
  src/packets/srv_srvselectreply.cpp
  include/packets/srv_srvselectreply.h
  src/packets/srv_stop.cpp
  include/packets/srv_stop.h
  src/packets/srv_stopmoving.cpp
  include/packets/srv_stopmoving.h
  src/packets/srv_switchserver.cpp
  include/packets/srv_switchserver.h
  src/packets/srv_whisperchat.cpp
  include/packets/srv_whisperchat.h
  src/packets/srv_equipitem.cpp
  include/packets/srv_equipitem.h
  src/packets/srv_setmoney.cpp
  include/packets/srv_setmoney.h
  src/packets/srv_setitem.cpp
  include/packets/srv_setitem.h
  src/packets/srv_partyreq.cpp
  include/packets/srv_partyreq.h
  src/packets/srv_partyreply.cpp
  include/packets/srv_partyreply.h
  src/packets/srv_partymember.cpp
  include/packets/srv_partymember.h
  src/packets/srv_partychat.cpp
  include/packets/srv_partychat.h
  src/packets/srv_loginreply.cpp
  include/packets/srv_loginreply.h
  src/packets/srv_channellistreply.cpp
  include/packets/srv_channellistreply.h
  src/packets/srv_questdata.cpp
  include/packets/srv_questdata.h
  src/packets/srv_dropitem.cpp
  include/packets/srv_dropitem.h
  src/packets/srv_mobchar.cpp
  include/packets/srv_mobchar.h
  src/packets/srv_npcchar.cpp
  include/packets/srv_npcchar.h
  src/packets/srv_adjustposition.cpp
  include/packets/srv_adjustposition.h
  src/packets/srv_teleportreply.cpp
  include/packets/srv_teleportreply.h
  src/packets/srv_setmoney.cpp
  include/packets/srv_setmoney.h
  src/packets/srv_attack.cpp
  include/packets/srv_attack.h
  src/packets/srv_hpreply.cpp
  include/packets/srv_hpreply.h
  src/packets/srv_sethpandmp.cpp
  include/packets/srv_sethpandmp.h
  src/packets/srv_damage.cpp
  include/packets/srv_damage.h
  include/packets/srv_billingmsg.h
  #---------------------------------------------------------
  src/packets/isc_serverregister.cpp
  include/packets/isc_serverregister.h
  src/packets/isc_shutdown.cpp
  include/packets/isc_shutdown.h
  src/packets/srv_characterlistreply.cpp
  include/packets/srv_characterlistreply.h

)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_PACKETS} ${SOURCES_HEADER} ${FLATBUFFERS})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/packets>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/components>)

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
endif()

add_dependencies(${PROJECT} flatbuffers)

add_dependencies(${PROJECT} Core)
target_link_libraries(${PROJECT} Core)

if(NOT LUA_FOUND)
  add_dependencies(${PROJECT} lua)
endif()
target_link_libraries(${PROJECT} lua::lua)


if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
