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
struct find_recv_class<ePacketType::PAKCS_LOGIN_REQ> { CliLoginReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_CHANNEL_LIST_REQ> { CliChannelReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_SRV_SELECT_REQ> { CliServerSelectReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_JOIN_SERVER_REQ> { CliJoinServerReq type; };

template <ePacketType T>
struct find_send_class {};

template <>
struct find_send_class<ePacketType::PAKSS_ACCEPT_REPLY> { SrvAcceptClient type; };

template <>
struct find_send_class<ePacketType::PAKLC_LOGIN_REPLY> { SrvLoginReply type; };

template <>
struct find_send_class<ePacketType::PAKLC_CHANNEL_LIST_REPLY> { SrvChannelReply type; };

template <>
struct find_send_class<ePacketType::PAKLC_SRV_SELECT_REPLY> { SrvServerSelectReply type; };

template <>
struct find_send_class<ePacketType::PAKSC_JOIN_SERVER_REPLY> { SrvJoinServerReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_CHAR_LIST_REPLY> { SrvCharacterListReply type; };

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
template <>
struct find_recv_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };

template <>
struct find_send_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };
}

#endif /* !_FINDCLASS_H_ */
