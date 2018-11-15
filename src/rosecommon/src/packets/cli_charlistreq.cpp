#include "cli_charlistreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliCharListReq::CliCharListReq() : CRosePacket(ePacketType::PAKCS_CHAR_LIST_REQ) {}

CliCharListReq::CliCharListReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_CHAR_LIST_REQ, "Not the right packet: " << to_underlying(get_type()));
}


void CliCharListReq::pack(CRoseWriter&) const {
}

uint16_t CliCharListReq::get_size() const {
	uint16_t size = 0;
	return size;
}


CliCharListReq CliCharListReq::create() {


	return CliCharListReq();
}

CliCharListReq CliCharListReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliCharListReq(reader);
}

}