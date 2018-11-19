#include "cli_logoutreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliLogoutReq::CliLogoutReq() : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

CliLogoutReq::CliLogoutReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_LOGOUT_REQ, "Not the right packet: " << to_underlying(get_type()));
}


void CliLogoutReq::pack(CRoseBasePolicy&) const {
}

CliLogoutReq CliLogoutReq::create() {


	return CliLogoutReq();
}

CliLogoutReq CliLogoutReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliLogoutReq(reader);
}
std::unique_ptr<CliLogoutReq> CliLogoutReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliLogoutReq>(reader);
}

}