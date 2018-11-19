#include "srv_screenshottimereply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvScreenShotTimeReply::SrvScreenShotTimeReply() : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY) {}

SrvScreenShotTimeReply::SrvScreenShotTimeReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(year_);
	reader.get_uint8_t(month_);
	reader.get_uint8_t(day_);
	reader.get_uint8_t(hour_);
	reader.get_uint8_t(min_);
}

SrvScreenShotTimeReply::SrvScreenShotTimeReply(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min) : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY), year_(year), month_(month), day_(day), hour_(hour), min_(min) {
}

uint16_t SrvScreenShotTimeReply::year() const { return year_; }

SrvScreenShotTimeReply& SrvScreenShotTimeReply::set_year(uint16_t data) { year_ = data; return *this; }

uint8_t SrvScreenShotTimeReply::month() const { return month_; }

SrvScreenShotTimeReply& SrvScreenShotTimeReply::set_month(uint8_t data) { month_ = data; return *this; }

uint8_t SrvScreenShotTimeReply::day() const { return day_; }

SrvScreenShotTimeReply& SrvScreenShotTimeReply::set_day(uint8_t data) { day_ = data; return *this; }

uint8_t SrvScreenShotTimeReply::hour() const { return hour_; }

SrvScreenShotTimeReply& SrvScreenShotTimeReply::set_hour(uint8_t data) { hour_ = data; return *this; }

uint8_t SrvScreenShotTimeReply::min() const { return min_; }

SrvScreenShotTimeReply& SrvScreenShotTimeReply::set_min(uint8_t data) { min_ = data; return *this; }


void SrvScreenShotTimeReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(year_);
	writer.set_uint8_t(month_);
	writer.set_uint8_t(day_);
	writer.set_uint8_t(hour_);
	writer.set_uint8_t(min_);
}

SrvScreenShotTimeReply SrvScreenShotTimeReply::create(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min) {


	return SrvScreenShotTimeReply(year, month, day, hour, min);
}

SrvScreenShotTimeReply SrvScreenShotTimeReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvScreenShotTimeReply(reader);
}
std::unique_ptr<SrvScreenShotTimeReply> SrvScreenShotTimeReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvScreenShotTimeReply>(reader);
}

}