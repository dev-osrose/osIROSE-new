#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_STOP, SrvStop)
REGISTER_SEND_PACKET(ePacketType::PAKWC_STOP, SrvStop)
class SrvStop : public CRosePacket {
	public:
		SrvStop();
		SrvStop(CRoseReader reader);
	private:
		SrvStop(uint16_t id, float x, float y, uint16_t z);
	public:

		virtual ~SrvStop() = default;

		uint16_t id() const;
		SrvStop& set_id(uint16_t);
		float x() const;
		SrvStop& set_x(float);
		float y() const;
		SrvStop& set_y(float);
		uint16_t z() const;
		SrvStop& set_z(uint16_t);

		static SrvStop create(Entity entity);
		static SrvStop create(uint8_t *buffer);
		static std::unique_ptr<SrvStop> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t id_;
		float x_;
		float y_;
		uint16_t z_;
};

}