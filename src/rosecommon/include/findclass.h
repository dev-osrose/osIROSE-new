
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

//template <>
//struct find_recv_class<ePacketType::PAKCS_SRV_SELECT_REQ> { CliLoginReq type; };

template <ePacketType T>
struct find_send_class {};

template <>
struct find_send_class<ePacketType::PAKLC_LOGIN_REPLY> { SrvLoginReply type; };

template <>
struct find_send_class<ePacketType::PAKLC_CHANNEL_LIST_REPLY> { SrvChannelReply type; };
}

#endif /* !_FINDCLASS_H_ */
