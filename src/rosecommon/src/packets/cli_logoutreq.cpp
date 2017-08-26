#include "cli_logoutreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliLogoutReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_LOGOUT_REQ, &createPacket<CliLogoutReq>);

CliLogoutReq::CliLogoutReq() : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_LOGOUT_REQ, "Not the right packet: " << to_underlying(type()));
}


void CliLogoutReq::pack() {
}

}
