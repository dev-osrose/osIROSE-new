#include "cli_attack.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliAttack::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_ATTACK, &createPacket<CliAttack>);

CliAttack::CliAttack() : CRosePacket(ePacketType::PAKCS_ATTACK) {}

CliAttack::CliAttack(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_ATTACK, "Not the right packet: " << to_underlying(type()));
	*this >> targetId_;
}

CliAttack::CliAttack(uint16_t targetId) : CRosePacket(ePacketType::PAKCS_ATTACK), targetId_(targetId) {}

uint16_t CliAttack::targetId() const {
	return targetId_;
}


void CliAttack::pack() {
	*this << targetId_;
}

}
