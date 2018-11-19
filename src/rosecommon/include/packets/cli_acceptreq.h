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
		static std::unique_ptr<CliAcceptReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;
};

}