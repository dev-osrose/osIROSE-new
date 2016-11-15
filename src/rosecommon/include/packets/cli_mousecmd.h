#pragma once

#include "packetfactory.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd)
class CliMouseCmd : public CRosePacket {
	public:
		CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliMouseCmd() = default;

		uint16_t &targetId();
        const uint16_t &targetId() const;
		float &x();
        const float &x() const;
		float &y();
        const float &y() const;
		uint16_t &z();
        const uint16_t &z() const;

	private:
		uint16_t targetId_;
		float x_;
		float y_;
		uint16_t z_;
};

}
