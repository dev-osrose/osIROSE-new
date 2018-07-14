#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_TELEPORT_REPLY, SrvTeleportReply)
class SrvTeleportReply : public CRosePacket {
	public:
		SrvTeleportReply();
		SrvTeleportReply(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvTeleportReply(Entity entity);

		virtual ~SrvTeleportReply() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}
