#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_MOB_CHAR, SrvMobChar)
class SrvMobChar : public CRosePacket {
	public:
		SrvMobChar(ePacketType type = ePacketType::PAKWC_MOB_CHAR);
		SrvMobChar(Entity entity, ePacketType type = ePacketType::PAKWC_MOB_CHAR);

		virtual ~SrvMobChar() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;
		Entity entity_;
};

}