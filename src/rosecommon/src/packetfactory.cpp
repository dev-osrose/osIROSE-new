#include "packetfactory.h"



#include "cli_accept_req.h"
#include "cli_channel_list_req.h"
#include "cli_char_list_req.h"
#include "cli_create_char_req.h"
#include "cli_delete_char_req.h"
#include "cli_join_server_req.h"
#include "cli_login_req.h"
#include "cli_select_char_req.h"
#include "cli_srv_select_req.h"
#include "isc_alive.h"
#include "isc_server_register.h"
#include "isc_shutdown.h"
#include "srv_accept_reply.h"
#include "srv_channel_list_reply.h"
#include "srv_char_list_reply.h"
#include "srv_create_char_reply.h"
#include "srv_delete_char_reply.h"
#include "srv_inventory_data.h"
#include "srv_join_server_reply.h"
#include "srv_login_reply.h"
#include "srv_quest_data.h"
#include "srv_screen_shot_time_reply.h"
#include "srv_srv_select_reply.h"
#include "srv_switch_server.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

#define REGISTER_SEND_PACKET(Type, Class) SendvPacketFactory::registerPacket<uint8_t*>(Type, &createPacket<Class>);

#define REGISTER_RECV_PACKET(Type, Class) RecvPacketFactory::registerPacket<uint8_t*>(Type, &createPacket<Class>);

void RoseCommon::register_recv_packets() {
    REGISTER_RECV_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANNEL_LIST_REQ, CliChannelListReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CHAR_LIST_REQ, CliCharListReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_CREATE_CHAR_REQ, CliCreateCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq);
    REGISTER_RECV_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq);
    REGISTER_RECV_PACKET(ePacketType::ISC_ALIVE, IscAlive);
    REGISTER_RECV_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister);
    REGISTER_RECV_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown);
}

void RoseCommon::register_send_packets() {
    REGISTER_SEND_PACKET(ePacketType::ISC_ALIVE, IscAlive);
    REGISTER_SEND_PACKET(ePacketType::ISC_SERVER_REGISTER, IscServerRegister);
    REGISTER_SEND_PACKET(ePacketType::ISC_SHUTDOWN, IscShutdown);
    REGISTER_SEND_PACKET(ePacketType::PAKSS_ACCEPT_REPLY, SrvAcceptReply);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelListReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_CHAR_LIST_REPLY, SrvCharListReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_CREATE_CHAR_REPLY, SrvCreateCharReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_DELETE_CHAR_REPLY, SrvDeleteCharReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_INVENTORY_DATA, SrvInventoryData);
    REGISTER_SEND_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvJoinServerReply);
    REGISTER_SEND_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvLoginReply);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_LOGIN_REPLY, SrvLoginReply);
    REGISTER_SEND_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData);
    REGISTER_SEND_PACKET(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, SrvScreenShotTimeReply);
    REGISTER_SEND_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply);
    REGISTER_SEND_PACKET(ePacketType::PAKCC_SWITCH_SERVER, SrvSwitchServer);
}
