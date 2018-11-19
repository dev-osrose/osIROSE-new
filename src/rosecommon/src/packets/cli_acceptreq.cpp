#include "cli_acceptreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliAcceptReq::CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

CliAcceptReq::CliAcceptReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_ACCEPT_REQ, "Not the right packet: " << to_underlying(get_type()));
}


void CliAcceptReq::pack(CRoseBasePolicy&) const {
}

CliAcceptReq CliAcceptReq::create() {


	return CliAcceptReq();
}

CliAcceptReq CliAcceptReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliAcceptReq(reader);
}
std::unique_ptr<CliAcceptReq> CliAcceptReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliAcceptReq>(reader);
}

}