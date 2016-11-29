#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply)
class CliPartyReply : public CRosePacket {
	public:
		CliPartyReply();
		CliPartyReply(uint8_t buffer[MAX_PACKET_SIZE]);
		CliPartyReply(uint8_t reply, uint32_t idXorTag);

		virtual ~CliPartyReply() = default;

		uint8_t reply() const;
		uint32_t idXorTag() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t reply_;
		uint32_t idXorTag_;
};

}