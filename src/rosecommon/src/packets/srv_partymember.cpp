#include "srv_partymember.h"
#include "throwassert.h"

namespace RoseCommon {

SrvPartyMember::SrvPartyMember() : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER) {}

SrvPartyMember::SrvPartyMember(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_PARTY_MEMBER, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(rules_);
	reader.get_iserialize(party_);
}

SrvPartyMember::SrvPartyMember(uint8_t rules, PartyMember::Party party) : CRosePacket(ePacketType::PAKWC_PARTY_MEMBER), rules_(rules), party_(party) {
}

uint8_t SrvPartyMember::rules() const { return rules_; }

SrvPartyMember& SrvPartyMember::set_rules(uint8_t data) { rules_ = data; return *this; }

PartyMember::Party SrvPartyMember::party() const { return party_; }

SrvPartyMember& SrvPartyMember::set_party(PartyMember::Party data) { party_ = data; return *this; }


void SrvPartyMember::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(rules_);
	writer.set_iserialize(party_);
}

uint16_t SrvPartyMember::get_size() const {
	uint16_t size = 0;
	size += sizeof(rules_);
	size += party_.get_size();
	return size;
}


SrvPartyMember SrvPartyMember::create(uint8_t rules, PartyMember::Party party) {


	return SrvPartyMember(rules, party);
}

SrvPartyMember SrvPartyMember::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvPartyMember(reader);
}

}