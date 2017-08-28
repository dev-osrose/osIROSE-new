#include "cli_charlistreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliCharListReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_CHAR_LIST_REQ, &createPacket<CliCharListReq>);

CliCharListReq::CliCharListReq() : CRosePacket(ePacketType::PAKCS_CHAR_LIST_REQ) {}

CliCharListReq::CliCharListReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_CHAR_LIST_REQ, "Not the right packet: " << to_underlying(type()));
}


void CliCharListReq::pack() {
}

}
