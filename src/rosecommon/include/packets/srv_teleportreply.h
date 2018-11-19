#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_TELEPORT_REPLY, SrvTeleportReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_TELEPORT_REPLY, SrvTeleportReply)
class SrvTeleportReply : public CRosePacket {
	public:
		SrvTeleportReply();
		SrvTeleportReply(CRoseReader reader);
	private:
		SrvTeleportReply(uint16_t id, uint16_t map, float x, float y, uint8_t moveMode);
	public:

		virtual ~SrvTeleportReply() = default;

		uint16_t id() const;
		SrvTeleportReply& set_id(uint16_t);
		uint16_t map() const;
		SrvTeleportReply& set_map(uint16_t);
		float x() const;
		SrvTeleportReply& set_x(float);
		float y() const;
		SrvTeleportReply& set_y(float);
		uint8_t moveMode() const;
		SrvTeleportReply& set_moveMode(uint8_t);
		uint8_t rideMode() const;
		SrvTeleportReply& set_rideMode(uint8_t);

		static SrvTeleportReply create(Entity entity);
		static SrvTeleportReply create(uint8_t *buffer);
		static std::unique_ptr<SrvTeleportReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t id_;
		uint16_t map_;
		float x_;
		float y_;
		uint8_t moveMode_;
		uint8_t rideMode_= 0;
};

}