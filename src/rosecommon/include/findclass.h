// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file findclass.h
 *
 * \author L3nn0x
 * \date april 2016
 *
 * This header contains two structs to properly deduce the subclass type of a packet from the ePacketType value
 */
#ifndef _FINDCLASS_H_
#define _FINDCLASS_H_

#include "epackettype.h"
#include "packetclasses.h"

namespace RoseCommon {

template <ePacketType T>
struct find_recv_class {};

template <>
struct find_recv_class<ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ> { CliScreenShotReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_JOIN_SERVER_REQ> { CliJoinServerReq type; };

#ifdef _LOGINPACKETS_H_
template <>
struct find_recv_class<ePacketType::PAKCS_LOGIN_REQ> { CliLoginReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_CHANNEL_LIST_REQ> { CliChannelReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_SRV_SELECT_REQ> { CliServerSelectReq type; };
#endif

#ifdef _CHARPACKETS_H_
template <>
struct find_recv_class<ePacketType::PAKCS_CREATE_CHAR_REQ> { CliCreateCharReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_DELETE_CHAR_REQ> { CliDeleteCharReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_SELECT_CHAR_REQ> { CliSelectCharReq type; };
#endif

#ifdef _MAPPACKETS_H_
//MAP PACKETS
template <>
struct find_recv_class<ePacketType::PAKCS_LOGOUT_REQ> { CliLogoutReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_STOP_MOVING> { CliStopMoving type; };

template <>
struct find_recv_class<ePacketType::PAKCS_MOUSE_CMD> { CliMouseCmd type; };

template <>
struct find_recv_class<ePacketType::PAKCS_CHANGE_MAP_REQ> { CliChangeMapReq type; };

template <>
struct find_recv_class<ePacketType::PAKWC_GLOBAL_VARS> { SrvServerData type; };

template <>
struct find_recv_class<ePacketType::PAKCS_REVIVE_REQ> { CliReviveReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_NORMAL_CHAT> { CliChat type; };
#endif

//SEND PACKETS
template <ePacketType T>
struct find_send_class {};

template <>
struct find_send_class<ePacketType::PAKSS_ACCEPT_REPLY> { SrvAcceptClient type; };

template <>
struct find_send_class<ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY> { SrvScreenShotReply type; };

// CHAR/MAP SERVER
template <>
struct find_send_class<ePacketType::PAKSC_JOIN_SERVER_REPLY> { SrvJoinServerReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_SWITCH_SERVER> { SrvSwitchServerReply type; };

#ifdef _LOGINPACKETS_H_
template <>
struct find_send_class<ePacketType::PAKLC_LOGIN_REPLY> { SrvLoginReply type; };

template <>
struct find_send_class<ePacketType::PAKLC_CHANNEL_LIST_REPLY> { SrvChannelReply type; };

template <>
struct find_send_class<ePacketType::PAKLC_SRV_SELECT_REPLY> { SrvServerSelectReply type; };
#endif

#ifdef _CHARPACKETS_H_
// CHAR SERVER
template <>
struct find_send_class<ePacketType::PAKCC_CHAR_LIST_REPLY> { SrvCharacterListReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_CREATE_CHAR_REPLY> { SrvCreateCharReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_DELETE_CHAR_REPLY> { SrvDeleteCharReply type; };
#endif

#ifdef _MAPPACKETS_H_
// MAP SERVER
template <>
struct find_send_class<ePacketType::PAKWC_SELECT_CHAR_REPLY> { SrvSelectCharReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_MOUSE_CMD> { SrvMouseCmd type; };

template <>
struct find_send_class<ePacketType::PAKWC_INVENTORY_DATA> { SrvInventoryData type; };

template <>
struct find_send_class<ePacketType::PAKWC_QUEST_DATA> { SrvQuestData type; };

template <>
struct find_send_class<ePacketType::PAKWC_BILLING_MESSAGE> { SrvBillingMsg type; };

template <>
struct find_send_class<ePacketType::PAKWC_LOGOUT_REPLY> { SrvLogoutReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_INIT_DATA> { SrvInitDataReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_CHANGE_MAP_REPLY> { SrvChangeMapReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_REVIVE_REPLY> { SrvReviveReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_NORMAL_CHAT> { SrvChat type; };
#endif

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
template <>
struct find_recv_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };

template <>
struct find_send_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };

template <>
struct find_recv_class<ePacketType::ISC_SHUTDOWN> { IscServerShutdown type; };

template <>
struct find_send_class<ePacketType::ISC_SHUTDOWN> { IscServerShutdown type; };
}

#endif /* !_FINDCLASS_H_ */
