#include "cli_mousecmd.h"
#include "throwassert.h"

namespace RoseCommon {

CliMouseCmd::CliMouseCmd() : CRosePacket(ePacketType::PAKCS_MOUSE_CMD) {}

CliMouseCmd::CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_MOUSE_CMD, "Not the right packet: " << to_underlying(type()));
    *this >> targetId_;
	*this >> x_;
	*this >> y_;
	*this >> z_;
}

CliMouseCmd::CliMouseCmd(uint16_t targetId, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKCS_MOUSE_CMD), targetId_(targetId), x_(x), y_(y), z_(z) {}

uint16_t CliMouseCmd::targetId() const {
	return targetId_;
}

float CliMouseCmd::x() const {
	return x_;
}

float CliMouseCmd::y() const {
	return y_;
}

uint16_t CliMouseCmd::z() const {
	return z_;
}


void CliMouseCmd::pack() {
	*this << targetId_;
	*this << x_;
	*this << y_;
	*this << z_;
}

}
