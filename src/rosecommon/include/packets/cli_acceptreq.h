#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq)
class CliAcceptReq : public CRosePacket {
	public:
		CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]);
        CliAcceptReq();

		virtual ~CliAcceptReq() = default;

};

}
