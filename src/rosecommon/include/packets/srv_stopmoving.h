#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_STOP_MOVING, SrvStopMoving)
REGISTER_SEND_PACKET(ePacketType::PAKWC_STOP_MOVING, SrvStopMoving)
class SrvStopMoving : public CRosePacket {
	public:
		SrvStopMoving();
		SrvStopMoving(CRoseReader reader);
	private:
		SrvStopMoving(uint16_t id, float x, float y, uint16_t z);
	public:

		virtual ~SrvStopMoving() = default;

		uint16_t id() const;
		SrvStopMoving& set_id(uint16_t);
		float x() const;
		SrvStopMoving& set_x(float);
		float y() const;
		SrvStopMoving& set_y(float);
		uint16_t z() const;
		SrvStopMoving& set_z(uint16_t);

		static SrvStopMoving create(Entity entity);
		static SrvStopMoving create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		float x_;
		float y_;
		uint16_t z_;
};

}