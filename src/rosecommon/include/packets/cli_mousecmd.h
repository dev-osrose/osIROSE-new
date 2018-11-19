#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd)
REGISTER_SEND_PACKET(ePacketType::PAKCS_MOUSE_CMD, CliMouseCmd)
class CliMouseCmd : public CRosePacket {
	public:
		CliMouseCmd();
		CliMouseCmd(CRoseReader reader);
	private:
		CliMouseCmd(uint16_t targetId, float x, float y, uint16_t z);
	public:

		virtual ~CliMouseCmd() = default;

		uint16_t targetId() const;
		CliMouseCmd& set_targetId(uint16_t);
		float x() const;
		CliMouseCmd& set_x(float);
		float y() const;
		CliMouseCmd& set_y(float);
		uint16_t z() const;
		CliMouseCmd& set_z(uint16_t);

		static CliMouseCmd create(uint16_t targetId, float x, float y, uint16_t z);
		static CliMouseCmd create(uint8_t *buffer);
		static std::unique_ptr<CliMouseCmd> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t targetId_;
		float x_;
		float y_;
		uint16_t z_;
};

}