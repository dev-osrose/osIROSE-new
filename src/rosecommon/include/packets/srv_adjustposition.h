#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_ADJUST_POSITION, SrvAdjustPosition)
REGISTER_SEND_PACKET(ePacketType::PAKWC_ADJUST_POSITION, SrvAdjustPosition)
class SrvAdjustPosition : public CRosePacket {
	public:
		SrvAdjustPosition();
		SrvAdjustPosition(CRoseReader reader);
	private:
		SrvAdjustPosition(uint16_t id, float x, float y, float z);
	public:

		virtual ~SrvAdjustPosition() = default;

		uint16_t id() const;
		SrvAdjustPosition& set_id(uint16_t);
		float x() const;
		SrvAdjustPosition& set_x(float);
		float y() const;
		SrvAdjustPosition& set_y(float);
		float z() const;
		SrvAdjustPosition& set_z(float);

		static SrvAdjustPosition create(Entity entity);
		static SrvAdjustPosition create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		float x_;
		float y_;
		float z_;
};

}