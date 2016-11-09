#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_STOP_MOVING, CliStopMoving)
class CliStopMoving : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_STOP_MOVING, CliStopMoving> {
	public:
		CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliStopMoving() = default;

		float &x();
		float &y();
		int16_t &z();

	private:
		float x_;
		float y_;
		int16_t z_;
};

}
