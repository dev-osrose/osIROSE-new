
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT RoseCommon)
project(${PROJECT})


if (NOT MSVC)
  add_compiler_flags(-Werror)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
)

set( SOURCES_HEADER
  include/crosesocket.h
  include/crosecrypt.h
  include/croseserver.h
  include/croseclient.h
  include/croseisc.h
  include/crosepacket.h
  include/epackettype.h
  include/isccommon.h
  include/itemdb.h
  include/crosewriter.h
  include/components/basic_info.h
  include/components/computed_values.h
  include/components/destination.h
  include/components/graphics.h
  include/components/hotbar.h
  include/components/inventory.h
  include/components/item.h
  include/components/level.h
  include/components/life.h
  include/components/magic.h
  include/components/npc.h
  include/components/owner.h
  include/components/party.h
  include/components/position.h
  include/components/skills.h
  include/components/stats.h
  include/components/status_effects.h
  include/components/target.h
  include/components/wishlist.h
)

SET(SOURCES_BASE
  src/crosesocket.cpp
  src/crosecrypt.cpp
  src/croseserver.cpp
  src/croseclient.cpp
  src/croseisc.cpp
  src/itemdb.cpp
  src/isccommon.cpp
  src/packetfactory.cpp
)

SET(SOURCES_PACKETS
    src/packets/cli_accept_req.cpp
    include/packets/cli_accept_req.h
    src/packets/cli_alive.cpp
    include/packets/cli_alive.h
    #  src/packets/cli_changemapreq.cpp
    #  include/packets/cli_changemapreq.h
    src/packets/cli_channel_list_req.cpp
    include/packets/cli_channel_list_req.h
    src/packets/cli_create_char_req.cpp
    include/packets/cli_create_char_req.h
    src/packets/cli_delete_char_req.cpp
    include/packets/cli_delete_char_req.h
    src/packets/cli_join_server_req.cpp
    include/packets/cli_join_server_req.h
    src/packets/cli_login_req.cpp
    include/packets/cli_login_req.h
    #  src/packets/cli_logoutreq.cpp
    #  include/packets/cli_logoutreq.h
    #  src/packets/cli_mousecmd.cpp
    #  include/packets/cli_mousecmd.h
    src/packets/cli_normal_chat.cpp
    include/packets/cli_normal_chat.h
    #  src/packets/cli_revivereq.cpp
    #  include/packets/cli_revivereq.h
    #  src/packets/cli_screenshottimereq.cpp
    #  include/packets/cli_screenshottimereq.h
    src/packets/cli_select_char_req.cpp
    include/packets/cli_select_char_req.h
    src/packets/cli_srv_select_req.cpp
    include/packets/cli_srv_select_req.h
    #  src/packets/cli_stop.cpp
    #  include/packets/cli_stop.h
    #  src/packets/cli_stopmoving.cpp
    #  include/packets/cli_stopmoving.h
    #  src/packets/cli_whisperchat.cpp
    #  include/packets/cli_whisperchat.h
    #  src/packets/cli_equipitem.cpp
    #  include/packets/cli_equipitem.h
    #  src/packets/cli_partyreq.cpp
    #  include/packets/cli_partyreq.h
    #  src/packets/cli_partyreply.cpp
    #  include/packets/cli_partyreply.h
    #  src/packets/cli_partychat.cpp
    #  include/packets/cli_partychat.h
    src/packets/cli_char_list_req.cpp
    include/packets/cli_char_list_req.h
    #  src/packets/cli_dropitem.cpp
    #  include/packets/cli_dropitem.h
    #  src/packets/cli_attack.cpp
    #  include/packets/cli_attack.h
    #  src/packets/cli_hpreq.cpp
    #  include/packets/cli_hpreq.h
    #  #------------------------------------------------
    src/packets/srv_accept_reply.cpp
    include/packets/srv_accept_reply.h
    #  src/packets/srv_changemapreply.cpp
    #  include/packets/srv_changemapreply.h
    src/packets/srv_create_char_reply.cpp
    include/packets/srv_create_char_reply.h
    src/packets/srv_delete_char_reply.cpp
    include/packets/srv_delete_char_reply.h
    #  src/packets/srv_initdata.cpp
    #  include/packets/srv_initdata.h
    src/packets/srv_inventory_data.cpp
    include/packets/srv_inventory_data.h
    src/packets/srv_join_server_reply.cpp
    include/packets/srv_join_server_reply.h
    #  src/packets/srv_logoutreply.cpp
    #  include/packets/srv_logoutreply.h
    #  src/packets/srv_mousecmd.cpp
    #  include/packets/srv_mousecmd.h
    #  src/packets/srv_normalchat.cpp
    #  include/packets/srv_normalchat.h
    #  src/packets/srv_playerchar.cpp
    #  include/packets/srv_playerchar.h
    #  src/packets/srv_removeobject.cpp
    #  include/packets/srv_removeobject.h
    src/packets/srv_screen_shot_time_reply.cpp
    include/packets/srv_screen_shot_time_reply.h
    #  src/packets/srv_selectcharreply.cpp
    #  include/packets/srv_selectcharreply.h
    #  src/packets/srv_globalvars.cpp
    #  include/packets/srv_globalvars.h
    src/packets/srv_srv_select_reply.cpp
    include/packets/srv_srv_select_reply.h
    #  src/packets/srv_stop.cpp
    #  include/packets/srv_stop.h
    #  src/packets/srv_stopmoving.cpp
    #  include/packets/srv_stopmoving.h
    src/packets/srv_switch_server.cpp
    include/packets/srv_switch_server.h
    #  src/packets/srv_whisperchat.cpp
    #  include/packets/srv_whisperchat.h
    #  src/packets/srv_equipitem.cpp
    #  include/packets/srv_equipitem.h
    #  src/packets/srv_setmoney.cpp
    #  include/packets/srv_setmoney.h
    #  src/packets/srv_setitem.cpp
    #  include/packets/srv_setitem.h
    #  src/packets/srv_partyreq.cpp
    #  include/packets/srv_partyreq.h
    #  src/packets/srv_partyreply.cpp
    #  include/packets/srv_partyreply.h
    #  src/packets/srv_partymember.cpp
    #  include/packets/srv_partymember.h
    #  src/packets/srv_partychat.cpp
    #  include/packets/srv_partychat.h
    src/packets/srv_login_reply.cpp
    include/packets/srv_login_reply.h
    src/packets/srv_channel_list_reply.cpp
    include/packets/srv_channel_list_reply.h
    src/packets/srv_quest_data.cpp
    include/packets/srv_quest_data.h
    #  src/packets/srv_dropitem.cpp
    #  include/packets/srv_dropitem.h
    #  src/packets/srv_mobchar.cpp
    #  include/packets/srv_mobchar.h
    #  src/packets/srv_npcchar.cpp
    #  include/packets/srv_npcchar.h
    #  src/packets/srv_adjustposition.cpp
    #  include/packets/srv_adjustposition.h
    #  src/packets/srv_teleportreply.cpp
    #  include/packets/srv_teleportreply.h
    #  src/packets/srv_setmoney.cpp
    #  include/packets/srv_setmoney.h
    #  src/packets/srv_attack.cpp
    #  include/packets/srv_attack.h
    #  src/packets/srv_hpreply.cpp
    #  include/packets/srv_hpreply.h
    #  src/packets/srv_sethpandmp.cpp
    #  include/packets/srv_sethpandmp.h
    #  src/packets/srv_damage.cpp
    #  include/packets/srv_damage.h
    #  src/packets/srv_billingmessage.cpp
    #  include/packets/srv_billingmessage.h
    #  #---------------------------------------------------------
    src/packets/isc_server_register.cpp
    include/packets/isc_server_register.h
    src/packets/isc_shutdown.cpp
    include/packets/isc_shutdown.h
    src/packets/srv_char_list_reply.cpp
    include/packets/srv_char_list_reply.h
    src/packets/isc_alive.cpp
    include/packets/isc_alive.h

)

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${SOURCES_PACKETS} ${SOURCES_HEADER})
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/packets>)
target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/components>)

if (MSVC)
  set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "/wd4503")
endif()

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
