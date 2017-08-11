#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <partybase.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply)
class CliPartyReply : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliPartyReply();
		CliPartyReply(uint8_t buffer[MAX_PACKET_SIZE]);
		CliPartyReply(PartyReply::PartyReply reply, uint32_t idXorTag);

		virtual ~CliPartyReply() = default;

		PartyReply::PartyReply reply() const;
		uint32_t idXorTag() const;

	protected:
		virtual void pack() override;

	private:
		PartyReply::PartyReply reply_;
		uint32_t idXorTag_;
};

}