#include "cli_attack.h"
#include "throwassert.h"

namespace RoseCommon {

CliAttack::CliAttack() : CRosePacket(ePacketType::PAKCS_ATTACK) {}

CliAttack::CliAttack(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_ATTACK, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(targetId_);
}

CliAttack::CliAttack(uint16_t targetId) : CRosePacket(ePacketType::PAKCS_ATTACK), targetId_(targetId) {
}

uint16_t CliAttack::targetId() const { return targetId_; }

CliAttack& CliAttack::set_targetId(uint16_t data) { targetId_ = data; return *this; }


void CliAttack::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(targetId_);
}

uint16_t CliAttack::get_size() const {
	uint16_t size = 0;
	size += sizeof(targetId_);
	return size;
}


CliAttack CliAttack::create(uint16_t targetId) {


	return CliAttack(targetId);
}

CliAttack CliAttack::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliAttack(reader);
}

}