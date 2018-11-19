#include "cli_partyreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliPartyReq::CliPartyReq() : CRosePacket(ePacketType::PAKCS_PARTY_REQ) {}

CliPartyReq::CliPartyReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_PARTY_REQ, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); request_ = static_cast<PartyReq::Request>(_tmp); }
	reader.get_uint32_t(idXorTag_);
}

CliPartyReq::CliPartyReq(PartyReq::Request request, uint32_t idXorTag) : CRosePacket(ePacketType::PAKCS_PARTY_REQ), request_(request), idXorTag_(idXorTag) {
}

PartyReq::Request CliPartyReq::request() const { return request_; }

CliPartyReq& CliPartyReq::set_request(PartyReq::Request data) { request_ = data; return *this; }

uint32_t CliPartyReq::idXorTag() const { return idXorTag_; }

CliPartyReq& CliPartyReq::set_idXorTag(uint32_t data) { idXorTag_ = data; return *this; }


void CliPartyReq::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(request_);
	writer.set_uint32_t(idXorTag_);
}

CliPartyReq CliPartyReq::create(PartyReq::Request request, uint32_t idXorTag) {


	return CliPartyReq(request, idXorTag);
}

CliPartyReq CliPartyReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliPartyReq(reader);
}
std::unique_ptr<CliPartyReq> CliPartyReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliPartyReq>(reader);
}

}