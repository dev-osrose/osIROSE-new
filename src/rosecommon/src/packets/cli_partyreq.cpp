#include "cli_partyreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliPartyReq::CliPartyReq() : CRosePacket(ePacketType::PAKCS_PARTY_REQ) {}

CliPartyReq::CliPartyReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(type() != ePacketType::PAKCS_PARTY_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> request_;
	*this >> idXorTag_;
}

CliPartyReq::CliPartyReq(CliPartyReq::Request request, uint32_t idXorTag) : CRosePacket(ePacketType::PAKCS_PARTY_REQ), request_(request), idXorTag_(idXorTag) {}

CliPartyReq::Request CliPartyReq::request() const {
	return request_;
}

uint32_t CliPartyReq::idXorTag() const {
	return idXorTag_;
}


void CliPartyReq::pack() {
	*this << to_underlying(request_);
	*this << idXorTag_;
}

}
