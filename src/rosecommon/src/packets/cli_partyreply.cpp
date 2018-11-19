#include "cli_partyreply.h"
#include "throwassert.h"

namespace RoseCommon {

CliPartyReply::CliPartyReply() : CRosePacket(ePacketType::PAKCS_PARTY_REPLY) {}

CliPartyReply::CliPartyReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_PARTY_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); reply_ = static_cast<PartyReply::Reply>(_tmp); }
	reader.get_uint32_t(idXorTag_);
}

CliPartyReply::CliPartyReply(PartyReply::Reply reply, uint32_t idXorTag) : CRosePacket(ePacketType::PAKCS_PARTY_REPLY), reply_(reply), idXorTag_(idXorTag) {
}

PartyReply::Reply CliPartyReply::reply() const { return reply_; }

CliPartyReply& CliPartyReply::set_reply(PartyReply::Reply data) { reply_ = data; return *this; }

uint32_t CliPartyReply::idXorTag() const { return idXorTag_; }

CliPartyReply& CliPartyReply::set_idXorTag(uint32_t data) { idXorTag_ = data; return *this; }


void CliPartyReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(reply_);
	writer.set_uint32_t(idXorTag_);
}

CliPartyReply CliPartyReply::create(PartyReply::Reply reply, uint32_t idXorTag) {


	return CliPartyReply(reply, idXorTag);
}

CliPartyReply CliPartyReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliPartyReply(reader);
}
std::unique_ptr<CliPartyReply> CliPartyReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliPartyReply>(reader);
}

}