#include "cli_screenshottimereq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliScreenShotTimeReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, &createPacket<CliScreenShotTimeReq>);

CliScreenShotTimeReq::CliScreenShotTimeReq() : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ) {}

CliScreenShotTimeReq::CliScreenShotTimeReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> count_;
}

CliScreenShotTimeReq::CliScreenShotTimeReq(uint16_t count) : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ), count_(count) {}

uint16_t CliScreenShotTimeReq::count() const {
	return count_;
}


void CliScreenShotTimeReq::pack() {
	*this << count_;
}

}
