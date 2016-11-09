#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, CliScreenShotTimeReq)
class CliScreenShotTimeReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ, CliScreenShotTimeReq> {
	public:
		CliScreenShotTimeReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliScreenShotTimeReq() = default;

		uint16_t &count();

	private:
		uint16_t count_;
};

}
