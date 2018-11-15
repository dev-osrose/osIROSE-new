#include "cli_screenshottimereq.h"
#include "throwassert.h"

namespace RoseCommon {

CliScreenShotTimeReq::CliScreenShotTimeReq() : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ) {}

CliScreenShotTimeReq::CliScreenShotTimeReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(count_);
}

CliScreenShotTimeReq::CliScreenShotTimeReq(uint16_t count) : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ), count_(count) {
}

uint16_t CliScreenShotTimeReq::count() const { return count_; }

CliScreenShotTimeReq& CliScreenShotTimeReq::set_count(uint16_t data) { count_ = data; return *this; }


void CliScreenShotTimeReq::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(count_);
}

uint16_t CliScreenShotTimeReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(count_);
	return size;
}


CliScreenShotTimeReq CliScreenShotTimeReq::create(uint16_t count) {


	return CliScreenShotTimeReq(count);
}

CliScreenShotTimeReq CliScreenShotTimeReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliScreenShotTimeReq(reader);
}

}