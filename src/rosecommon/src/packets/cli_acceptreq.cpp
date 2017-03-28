#include "cli_acceptreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliAcceptReq::CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

CliAcceptReq::CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() != ePacketType::PAKCS_ACCEPT_REQ, "Not the right packet: " << to_underlying(type()));
}


void CliAcceptReq::pack() {
}

}
