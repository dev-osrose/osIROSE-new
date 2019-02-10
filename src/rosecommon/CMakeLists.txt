
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
  include/party_base.h
  include/components/basic_info.h
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
  include/components/party.h
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
    src/packets/cli_change_map_req.cpp
    include/packets/cli_change_map_req.h
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
    src/packets/cli_mouse_cmd.cpp
    include/packets/cli_mouse_cmd.h
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
    src/packets/cli_stop_moving.cpp
    include/packets/cli_stop_moving.h
    src/packets/cli_whisper_chat.cpp
    include/packets/cli_whisper_chat.h
    #  src/packets/cli_equipitem.cpp
    #  include/packets/cli_equipitem.h
    src/packets/cli_party_req.cpp
    include/packets/cli_party_req.h
    src/packets/cli_party_reply.cpp
    include/packets/cli_party_reply.h
    src/packets/cli_party_chat.cpp
    include/packets/cli_party_chat.h
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
    src/packets/srv_change_map_reply.cpp
    include/packets/srv_change_map_reply.h
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
    src/packets/srv_mouse_cmd.cpp
    include/packets/srv_mouse_cmd.h
    src/packets/srv_normal_chat.cpp
    include/packets/srv_normal_chat.h
    src/packets/srv_player_char.cpp
    include/packets/srv_player_char.h
    src/packets/srv_remove_object.cpp
    include/packets/srv_remove_object.h
    src/packets/srv_screen_shot_time_reply.cpp
    include/packets/srv_screen_shot_time_reply.h
    src/packets/srv_select_char_reply.cpp
    include/packets/srv_select_char_reply.h
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
    src/packets/srv_whisper_chat.cpp
    include/packets/srv_whisper_chat.h
    #  src/packets/srv_equipitem.cpp
    #  include/packets/srv_equipitem.h
    #  src/packets/srv_setmoney.cpp
    #  include/packets/srv_setmoney.h
    #  src/packets/srv_setitem.cpp
    #  include/packets/srv_setitem.h
    src/packets/srv_party_req.cpp
    include/packets/srv_party_req.h
    src/packets/srv_party_reply.cpp
    include/packets/srv_party_reply.h
    src/packets/srv_party_member.cpp
    include/packets/srv_party_member.h
    src/packets/srv_party_chat.cpp
    include/packets/srv_party_chat.h
    src/packets/srv_login_reply.cpp
    include/packets/srv_login_reply.h
    src/packets/srv_channel_list_reply.cpp
    include/packets/srv_channel_list_reply.h
    src/packets/srv_quest_data.cpp
    include/packets/srv_quest_data.h
    #  src/packets/srv_dropitem.cpp
    #  include/packets/srv_dropitem.h
    src/packets/srv_mob_char.cpp
    include/packets/srv_mob_char.h
    src/packets/srv_npc_char.cpp
    include/packets/srv_npc_char.h
    #  src/packets/srv_adjustposition.cpp
    #  include/packets/srv_adjustposition.h
    src/packets/srv_teleport_reply.cpp
    include/packets/srv_teleport_reply.h
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
    src/packets/srv_billing_message.cpp
    include/packets/srv_billing_message.h
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
