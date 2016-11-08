#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

class CliLogoutReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_LOGOUT_REQ, CliLogoutReq> {
	public:
		CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliLogoutReq() = default;

};

}