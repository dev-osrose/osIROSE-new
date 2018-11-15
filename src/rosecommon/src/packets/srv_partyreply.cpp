#include "srv_partyreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvPartyReply::SrvPartyReply() : CRosePacket(ePacketType::PAKWC_PARTY_REPLY) {}

SrvPartyReply::SrvPartyReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_PARTY_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); reply_ = static_cast<PartyReply::Reply>(_tmp); }
	reader.get_uint32_t(id_);
}

SrvPartyReply::SrvPartyReply(PartyReply::Reply reply, uint32_t id) : CRosePacket(ePacketType::PAKWC_PARTY_REPLY), reply_(reply), id_(id) {
}

PartyReply::Reply SrvPartyReply::reply() const { return reply_; }

SrvPartyReply& SrvPartyReply::set_reply(PartyReply::Reply data) { reply_ = data; return *this; }

uint32_t SrvPartyReply::id() const { return id_; }

SrvPartyReply& SrvPartyReply::set_id(uint32_t data) { id_ = data; return *this; }


void SrvPartyReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(reply_);
	writer.set_uint32_t(id_);
}

uint16_t SrvPartyReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(reply_);
	size += sizeof(id_);
	return size;
}


SrvPartyReply SrvPartyReply::create(PartyReply::Reply reply, uint32_t id) {


	return SrvPartyReply(reply, id);
}

SrvPartyReply SrvPartyReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvPartyReply(reader);
}

}