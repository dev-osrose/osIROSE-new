#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REQ, CliPartyReq)
class CliPartyReq : public CRosePacket {
	public:
		CliPartyReq();
		CliPartyReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliPartyReq(uint8_t request, uint32_t idXorTag);

		virtual ~CliPartyReq() = default;

		uint8_t request() const;
		uint32_t idXorTag() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t request_;
		uint32_t idXorTag_;
};

}