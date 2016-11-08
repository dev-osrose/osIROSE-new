#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

class CliStop : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_STOP, CliStop> {
	public:
		CliStop(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliStop() = default;

		float &x();
		float &y();

	private:
		float x_;
		float y_;
};

}