#include "packetfactory.h"



#include "cli_accept_req.h"
#include "cli_alive.h"
#include "cli_attack.h"
#include "cli_change_map_req.h"
#include "cli_channel_list_req.h"
#include "cli_char_list_req.h"
#include "cli_create_char_req.h"
#include "cli_delete_char_req.h"
#include "cli_drop_item.h"
#include "cli_equip_item.h"
#include "cli_hp_req.h"
#include "cli_join_server_req.h"
#include "cli_login_req.h"
#include "cli_mouse_cmd.h"
#include "cli_normal_chat.h"
#include "cli_revive_req.h"
#include "cli_party_chat.h"
#include "cli_party_reply.h"
#include "cli_party_req.h"
#include "cli_select_char_req.h"
#include "cli_shout_chat.h"
#include "cli_srv_select_req.h"
#include "cli_stop_moving.h"
#include "cli_teleport_req.h"
#include "cli_whisper_chat.h"
#include "isc_alive.h"
#include "isc_server_register.h"
#include "isc_shutdown.h"
#include "srv_accept_reply.h"
#include "srv_attack.h"
#include "srv_billing_message.h"
#include "srv_change_map_reply.h"
#include "srv_channel_list_reply.h"
#include "srv_char_list_reply.h"
#include "srv_create_char_reply.h"
#include "srv_damage.h"
#include "srv_delete_char_reply.h"
#include "srv_hp_reply.h"
#include "srv_inventory_data.h"
#include "srv_join_server_reply.h"
#include "srv_login_reply.h"
#include "srv_mob_char.h"
#include "srv_mouse_cmd.h"
#include "srv_normal_chat.h"
#include "srv_npc_char.h"
#include "srv_party_chat.h"
#include "srv_party_member.h"
#include "srv_party_reply.h"
#include "srv_party_req.h"
#include "srv_player_char.h"
#include "srv_quest_data.h"
#include "srv_remove_object.h"
#include "srv_screen_shot_time_reply.h"
#include "srv_select_char_reply.h"
#include "srv_set_item.h"
#include "srv_shout_chat.h"
#include "srv_set_hp_and_mp.h"
#include "srv_srv_select_reply.h"
#include "srv_switch_server.h"
#include "srv_teleport_reply.h"
#include "srv_whisper_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

#define REGISTER_SEND_PACKET(Type, Class) SendvPacketFactory::registerPacket<uint8_t*>(Type, std::function{&createPacket<Class>});

#define REGISTER_RECV_PACKET(Type, Class) RecvPacketFactory::registerPacket<const uint8_t*>(Type, std::function{&createPacket_const<Class>});

void RoseCommon::register_recv_packets() {
    REGISTER_RECV_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_ALIVE, CliAlive);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_ATTACK, CliAttack);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANNEL_LIST_REQ, CliChannelListReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CHAR_LIST_REQ, CliCharListReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CREATE_CHAR_REQ, CliCreateCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_HP_REQ, CliHpReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_DROP_ITEM, CliDropItem);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_EQUIP_ITEM, CliEquipItem);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_CHAT, CliPartyChat);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REQ, CliPartyReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_SHOUT_CHAT, CliShoutChat);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_STOP_MOVING, CliStopMoving);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_TELEPORT_REQ, CliTeleportReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_WHISPER_CHAT, CliWhisperChat);
    REGISTER_RECV_PACKET(ePacketType::ISC_ALIVE, IscAlive);
    REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister);
    REGISTER_RECV_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown);
}

void RoseCommon::register_send_packets() {
    REGISTER_SEND_PACKET(ePacketType::ISC_ALIVE, IscAlive);
    REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister);
    REGISTER_SEND_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown);
    REGISTER_SEND_PACKET(ePacketType::PAKSS_ACCEPT_REPLY, SrvAcceptReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_ATTACK, SrvAttack);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelListReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_CHANGE_MAP_REPLY, SrvChangeMapReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_CHAR_LIST_REPLY, SrvCharListReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_CREATE_CHAR_REPLY, SrvCreateCharReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_DAMAGE, SrvDamage);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_DELETE_CHAR_REPLY, SrvDeleteCharReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_HP_REPLY, SrvHpReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_INVENTORY_DATA, SrvInventoryData);
    REGISTER_SEND_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvJoinServerReply);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_MOB_CHAR, SrvMobChar);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_MOUSE_CMD, SrvMouseCmd);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_NORMAL_CHAT, SrvNormalChat);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_NPC_CHAR, SrvNpcChar);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_CHAT, SrvPartyChat);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_MEMBER, SrvPartyMember);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REPLY, SrvPartyReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REQ, SrvPartyReq);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_PLAYER_CHAR, SrvPlayerChar);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_REMOVE_OBJECT, SrvRemoveObject);
    REGISTER_SEND_PACKET(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, SrvScreenShotTimeReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_SELECT_CHAR_REPLY, SrvSelectCharReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_ITEM, SrvSetItem);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_SHOUT_CHAT, SrvShoutChat);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_HP_AND_MP, SrvSetHpAndMp);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_SWITCH_SERVER, SrvSwitchServer);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_TELEPORT_REPLY, SrvTeleportReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_WHISPER_CHAT, SrvWhisperChat);
}
