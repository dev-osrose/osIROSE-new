#include "srv_screenshottimereply.h"

namespace RoseCommon {

SrvScreenShotTimeReply::SrvScreenShotTimeReply() : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY) {
}

SrvScreenShotTimeReply::SrvScreenShotTimeReply(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min) : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY), year_(year), month_(month), day_(day), hour_(hour), min_(min) {
}

uint16_t &SrvScreenShotTimeReply::year() {
	return year_;
}

uint8_t &SrvScreenShotTimeReply::month() {
	return month_;
}

uint8_t &SrvScreenShotTimeReply::day() {
	return day_;
}

uint8_t &SrvScreenShotTimeReply::hour() {
	return hour_;
}

uint8_t &SrvScreenShotTimeReply::min() {
	return min_;
}

void SrvScreenShotTimeReply::pack() {
	*this << year_;
	*this << month_;
	*this << day_;
	*this << hour_;
	*this << min_;
}

}