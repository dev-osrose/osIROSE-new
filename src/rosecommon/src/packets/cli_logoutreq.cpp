#include "cli_logoutreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliLogoutReq::CliLogoutReq() : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_LOGOUT_REQ, "Not the right packet: " << to_underlying(type()));
}


void CliLogoutReq::pack() {
}

}
