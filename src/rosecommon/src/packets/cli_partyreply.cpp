#include "cli_partyreply.h"
#include "throwassert.h"

namespace RoseCommon {

CliPartyReply::CliPartyReply() : CRosePacket(ePacketType::PAKCS_PARTY_REPLY) {}

CliPartyReply::CliPartyReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(type() != ePacketType::PAKCS_PARTY_REPLY, "Not the right packet: " << to_underlying(type()));
	*this >> reply_;
	*this >> idXorTag_;
}

CliPartyReply::CliPartyReply(CliPartyReply::Reply reply, uint32_t idXorTag) : CRosePacket(ePacketType::PAKCS_PARTY_REPLY), reply_(reply), idXorTag_(idXorTag) {}

CliPartyReply::Reply CliPartyReply::reply() const {
	return reply_;
}

uint32_t CliPartyReply::idXorTag() const {
	return idXorTag_;
}


void CliPartyReply::pack() {
	*this << to_underlying(reply_);
	*this << idXorTag_;
}

}
