#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_HP_REPLY, SrvHpReply)
class SrvHpReply : public CRosePacket {
	public:
		SrvHpReply();
		SrvHpReply(Entity entity);

		virtual ~SrvHpReply() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}