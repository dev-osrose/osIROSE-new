#include "cli_partyreply.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliPartyReply::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_PARTY_REPLY, &createPacket<CliPartyReply>);

CliPartyReply::CliPartyReply() : CRosePacket(ePacketType::PAKCS_PARTY_REPLY) {}

CliPartyReply::CliPartyReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_PARTY_REPLY, "Not the right packet: " << to_underlying(type()));
	*this >> reply_;
	*this >> idXorTag_;
}

CliPartyReply::CliPartyReply(PartyReply::PartyReply reply, uint32_t idXorTag) : CRosePacket(ePacketType::PAKCS_PARTY_REPLY), reply_(reply), idXorTag_(idXorTag) {}

PartyReply::PartyReply CliPartyReply::reply() const {
	return reply_;
}

uint32_t CliPartyReply::idXorTag() const {
	return idXorTag_;
}


void CliPartyReply::pack() {
	*this << reply_;
	*this << idXorTag_;
}

}
