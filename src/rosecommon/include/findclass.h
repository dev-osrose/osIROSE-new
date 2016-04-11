
#ifndef _FINDCLASS_H_
#define _FINDCLASS_H_

#include "epackettype.h"
#include "packetclasses.h"

namespace RoseCommon {

template <uint16_t T>
struct find_class {};

template <>
struct find_class<ePacketType::PAKCS_LOGIN_REQ> { CliLoginReq type; };

template <>
struct find_class<ePacketType::PAKLC_LOGIN_REPLY> { SrvLoginReply type; };
}

#endif /* !_FINDCLASS_H_ */
