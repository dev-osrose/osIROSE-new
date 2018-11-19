#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_MOUSE_CMD, SrvMouseCmd)
REGISTER_SEND_PACKET(ePacketType::PAKWC_MOUSE_CMD, SrvMouseCmd)
class SrvMouseCmd : public CRosePacket {
	public:
		SrvMouseCmd();
		SrvMouseCmd(CRoseReader reader);
	private:
		SrvMouseCmd(uint16_t id, uint16_t targetId, uint16_t dist, float x, float y, uint16_t z);
	public:

		virtual ~SrvMouseCmd() = default;

		uint16_t id() const;
		SrvMouseCmd& set_id(uint16_t);
		uint16_t targetId() const;
		SrvMouseCmd& set_targetId(uint16_t);
		uint16_t dist() const;
		SrvMouseCmd& set_dist(uint16_t);
		float x() const;
		SrvMouseCmd& set_x(float);
		float y() const;
		SrvMouseCmd& set_y(float);
		uint16_t z() const;
		SrvMouseCmd& set_z(uint16_t);

		static SrvMouseCmd create(Entity entity);
		static SrvMouseCmd create(uint8_t *buffer);
		static std::unique_ptr<SrvMouseCmd> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t id_;
		uint16_t targetId_;
		uint16_t dist_;
		float x_;
		float y_;
		uint16_t z_;
};

}