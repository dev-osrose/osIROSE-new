#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGOUT_REQ, CliLogoutReq)
class CliLogoutReq : public CRosePacket {
	public:
		CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliLogoutReq() = default;

};

}
