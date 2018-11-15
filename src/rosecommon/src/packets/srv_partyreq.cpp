#include "srv_partyreq.h"
#include "throwassert.h"

namespace RoseCommon {

SrvPartyReq::SrvPartyReq() : CRosePacket(ePacketType::PAKWC_PARTY_REQ) {}

SrvPartyReq::SrvPartyReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_PARTY_REQ, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); request_ = static_cast<PartyReq::Request>(_tmp); }
	reader.get_uint32_t(tag_);
	reader.get_string(name_);
}

SrvPartyReq::SrvPartyReq(PartyReq::Request request, uint32_t tag, const std::string& name) : CRosePacket(ePacketType::PAKWC_PARTY_REQ), request_(request), tag_(tag), name_(name) {
}

PartyReq::Request SrvPartyReq::request() const { return request_; }

SrvPartyReq& SrvPartyReq::set_request(PartyReq::Request data) { request_ = data; return *this; }

uint32_t SrvPartyReq::tag() const { return tag_; }

SrvPartyReq& SrvPartyReq::set_tag(uint32_t data) { tag_ = data; return *this; }

const std::string& SrvPartyReq::name() const { return name_; }

SrvPartyReq& SrvPartyReq::set_name(const std::string& data) { name_ = data; return *this; }


void SrvPartyReq::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(request_);
	writer.set_uint32_t(tag_);
	writer.set_string(name_);
}

uint16_t SrvPartyReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(request_);
	size += sizeof(tag_);
	size += sizeof(char) * (name_.size() + 1);
	return size;
}


SrvPartyReq SrvPartyReq::create(PartyReq::Request request, Entity entity) {
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvPartyReq(request, entity_basicinfo->tag_, entity_basicinfo->name_);
}

SrvPartyReq SrvPartyReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvPartyReq(reader);
}

}