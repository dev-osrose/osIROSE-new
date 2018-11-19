#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, SrvScreenShotTimeReply)
REGISTER_SEND_PACKET(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY, SrvScreenShotTimeReply)
class SrvScreenShotTimeReply : public CRosePacket {
	public:
		SrvScreenShotTimeReply();
		SrvScreenShotTimeReply(CRoseReader reader);
	private:
		SrvScreenShotTimeReply(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min);
	public:

		virtual ~SrvScreenShotTimeReply() = default;

		uint16_t year() const;
		SrvScreenShotTimeReply& set_year(uint16_t);
		uint8_t month() const;
		SrvScreenShotTimeReply& set_month(uint8_t);
		uint8_t day() const;
		SrvScreenShotTimeReply& set_day(uint8_t);
		uint8_t hour() const;
		SrvScreenShotTimeReply& set_hour(uint8_t);
		uint8_t min() const;
		SrvScreenShotTimeReply& set_min(uint8_t);

		static SrvScreenShotTimeReply create(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min);
		static SrvScreenShotTimeReply create(uint8_t *buffer);
		static std::unique_ptr<SrvScreenShotTimeReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t year_;
		uint8_t month_;
		uint8_t day_;
		uint8_t hour_;
		uint8_t min_;
};

}