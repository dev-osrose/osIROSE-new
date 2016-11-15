#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_STOP_MOVING, CliStopMoving)
class CliStopMoving : public CRosePacket {
	public:
		CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliStopMoving() = default;

		float &x();
        const float &x() const;
		float &y();
        const float &y() const;
		int16_t &z();
        const int16_t &z() const;

	private:
		float x_;
		float y_;
		int16_t z_;
};

}
