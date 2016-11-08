#pragma once

#include "packetfactory.h"

namespace RoseCommon {

class CliMouseCmd : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd> {
	public:
		CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliMouseCmd() = default;

		uint16_t &targetId();
		float &x();
		float &y();
		uint16_t &z();

	private:
		uint16_t targetId_;
		float x_;
		float y_;
		uint16_t z_;
};

}