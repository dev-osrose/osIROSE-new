#include "cli_charlistreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliCharListReq::CliCharListReq() : CRosePacket(ePacketType::PAKCS_CHAR_LIST_REQ) {}

CliCharListReq::CliCharListReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_CHAR_LIST_REQ, "Not the right packet: " << to_underlying(get_type()));
}


void CliCharListReq::pack(CRoseBasePolicy&) const {
}

CliCharListReq CliCharListReq::create() {


	return CliCharListReq();
}

CliCharListReq CliCharListReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliCharListReq(reader);
}
std::unique_ptr<CliCharListReq> CliCharListReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliCharListReq>(reader);
}

}