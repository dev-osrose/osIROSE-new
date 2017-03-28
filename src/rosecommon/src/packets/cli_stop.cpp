#include "cli_stop.h"
#include "throwassert.h"

namespace RoseCommon {

CliStop::CliStop() : CRosePacket(ePacketType::PAKCS_STOP) {}

CliStop::CliStop(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() != ePacketType::PAKCS_STOP, "Not the right packet: " << to_underlying(type()));
    *this >> x_;
	*this >> y_;
}

CliStop::CliStop(float x, float y) : CRosePacket(ePacketType::PAKCS_STOP), x_(x), y_(y) {}

float CliStop::x() const {
	return x_;
}

float CliStop::y() const {
	return y_;
}


void CliStop::pack() {
	*this << x_;
	*this << y_;
}

}
