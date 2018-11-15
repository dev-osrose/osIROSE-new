#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_STOP, CliStop)
REGISTER_SEND_PACKET(ePacketType::PAKCS_STOP, CliStop)
class CliStop : public CRosePacket {
	public:
		CliStop();
		CliStop(CRoseReader reader);
	private:
		CliStop(float x, float y);
	public:

		virtual ~CliStop() = default;

		float x() const;
		CliStop& set_x(float);
		float y() const;
		CliStop& set_y(float);

		static CliStop create(float x, float y);
		static CliStop create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		float x_;
		float y_;
};

}