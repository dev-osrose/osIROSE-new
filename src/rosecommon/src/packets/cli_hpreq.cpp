#include "cli_hpreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliHpReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_HP_REQ, &createPacket<CliHpReq>);

CliHpReq::CliHpReq() : CRosePacket(ePacketType::PAKCS_HP_REQ) {}

CliHpReq::CliHpReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_HP_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> targetId_;
}

CliHpReq::CliHpReq(uint16_t targetId) : CRosePacket(ePacketType::PAKCS_HP_REQ), targetId_(targetId) {}

uint16_t CliHpReq::targetId() const {
	return targetId_;
}


void CliHpReq::pack() {
	*this << targetId_;
}

}
