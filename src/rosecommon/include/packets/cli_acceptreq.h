#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq)
class CliAcceptReq : public CRosePacket {
	public:
		CliAcceptReq();
		CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliAcceptReq() = default;


	protected:
		virtual void pack() override;
};

}