#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd)
class CliMouseCmd : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliMouseCmd();
		CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]);
		CliMouseCmd(uint16_t targetId, float x, float y, uint16_t z);

		virtual ~CliMouseCmd() = default;

		uint16_t targetId() const;
		float x() const;
		float y() const;
		uint16_t z() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t targetId_;
		float x_;
		float y_;
		uint16_t z_;
};

}