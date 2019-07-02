
link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT RoseCommon)
project(${PROJECT})

if (NOT MSVC)
  add_compiler_flags(-Werror)
endif()

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
  src/party_base.cpp
)

set(IDL_TYPE_FILES
  packets/equipped_item.xml
  packets/equipped_position.xml
  packets/item.xml
)

set(IDL_FILES
  packets/cli_accept_req.xml
  packets/cli_alive.xml
  packets/cli_attack.xml
  packets/cli_change_char_req.xml
  packets/cli_change_map_req.xml
  packets/cli_channel_list_req.xml
  packets/cli_char_list_req.xml
  packets/cli_create_char_req.xml
  packets/cli_delete_char_req.xml
  packets/cli_hp_req.xml
  packets/cli_drop_item.xml
  packets/cli_equip_item.xml
  packets/cli_join_server_req.xml
  packets/cli_login_req.xml
  packets/cli_logout_req.xml
  packets/cli_mouse_cmd.xml
  packets/cli_normal_chat.xml
  packets/cli_party_chat.xml
  packets/cli_party_reply.xml
  packets/cli_party_req.xml
  packets/cli_revive_req.xml
  packets/cli_select_char_req.xml
  packets/cli_shout_chat.xml
  packets/cli_srv_select_req.xml
  packets/cli_stop_moving.xml
  packets/cli_teleport_req.xml
  packets/cli_whisper_chat.xml
  packets/isc_alive.xml
  packets/isc_server_register.xml
  packets/isc_transfer.xml
  packets/isc_transfer_char.xml
  packets/isc_shutdown.xml
  packets/srv_accept_reply.xml
  packets/srv_announce_chat.xml
  packets/srv_attack.xml
  packets/srv_billing_message.xml
  packets/srv_chan_char_reply.xml
  packets/srv_change_map_reply.xml
  packets/srv_channel_list_reply.xml
  packets/srv_char_list_reply.xml
  packets/srv_create_char_reply.xml
  packets/srv_damage.xml
  packets/srv_delete_char_reply.xml
  packets/srv_hp_reply.xml
  packets/srv_drop_item.xml
  packets/srv_equip_item.xml
  packets/srv_inventory_data.xml
  packets/srv_join_server_reply.xml
  packets/srv_login_reply.xml
  packets/srv_logout_reply.xml
  packets/srv_mob_char.xml
  packets/srv_mouse_cmd.xml
  packets/srv_normal_chat.xml
  packets/srv_npc_char.xml
  packets/srv_party_chat.xml
  packets/srv_party_member.xml
  packets/srv_party_reply.xml
  packets/srv_party_req.xml
  packets/srv_player_char.xml
  packets/srv_quest_data.xml
  packets/srv_remove_object.xml
  packets/srv_screen_shot_time_reply.xml
  packets/srv_select_char_reply.xml
  packets/srv_set_item.xml
  packets/srv_set_money.xml
  packets/srv_shout_chat.xml
  packets/srv_set_hp_and_mp.xml
  packets/srv_srv_select_reply.xml
  packets/srv_switch_server.xml
  packets/srv_teleport_reply.xml
  packets/srv_whisper_chat.xml
)

set(args
  TARGET RosePackets
  IDLFILES ${IDL_FILES}
  IDLROOT ${CMAKE_CURRENT_SOURCE_DIR}/packets
  SRC_OUTPATH ${CMAKE_GEN_SRC_DIR}/packets
  HDR_OUTPATH ${CMAKE_GEN_SRC_DIR}/packets
)

generate_packets(PACKET_SOURCES PACKET_HEADERS ${args})

add_library(${PROJECT} STATIC ${SOURCES_BASE} ${PACKET_SOURCES} ${PACKET_HEADERS} ${SOURCES_HEADER})
target_include_directories(${PROJECT} 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_GEN_SRC_DIR}/packets>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/components>
)

add_dependencies(${PROJECT} Core)
add_dependencies(${PROJECT} RosePackets_generated)
target_link_libraries(${PROJECT} 
  PUBLIC 
    Core
)

if (COMMAND cotire)
  cotire(${PROJECT})
endif()

SET_PROPERTY(TARGET ${PROJECT}                PROPERTY FOLDER "Servers")
