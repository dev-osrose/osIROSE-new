#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_ACCEPT_REQ, CliAcceptReq)
class CliAcceptReq : public CRosePacket {
	public:
		CliAcceptReq();
		CliAcceptReq(CRoseReader reader);
	public:

		virtual ~CliAcceptReq() = default;


		static CliAcceptReq create();
		static CliAcceptReq create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;
};

}