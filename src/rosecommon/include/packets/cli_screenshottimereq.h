#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, CliScreenShotTimeReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, CliScreenShotTimeReq)
class CliScreenShotTimeReq : public CRosePacket {
	public:
		CliScreenShotTimeReq();
		CliScreenShotTimeReq(CRoseReader reader);
	private:
		CliScreenShotTimeReq(uint16_t count);
	public:

		virtual ~CliScreenShotTimeReq() = default;

		uint16_t count() const;
		CliScreenShotTimeReq& set_count(uint16_t);

		static CliScreenShotTimeReq create(uint16_t count);
		static CliScreenShotTimeReq create(uint8_t *buffer);
		static std::unique_ptr<CliScreenShotTimeReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t count_;
};

}