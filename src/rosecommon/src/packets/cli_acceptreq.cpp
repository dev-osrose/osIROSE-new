#include "cli_acceptreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliAcceptReq::CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

CliAcceptReq::CliAcceptReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_ACCEPT_REQ, "Not the right packet: " << to_underlying(get_type()));
}


void CliAcceptReq::pack(CRoseWriter&) const {
}

uint16_t CliAcceptReq::get_size() const {
	uint16_t size = 0;
	return size;
}


CliAcceptReq CliAcceptReq::create() {


	return CliAcceptReq();
}

CliAcceptReq CliAcceptReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliAcceptReq(reader);
}

}