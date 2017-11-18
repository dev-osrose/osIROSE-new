#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"

#include "srv_mobchar.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_NPC_CHAR, SrvNpcChar)
class SrvNpcChar : public SrvMobChar {
	public:
		SrvNpcChar();
		SrvNpcChar(Entity entity);

		virtual ~SrvNpcChar() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;
};

}