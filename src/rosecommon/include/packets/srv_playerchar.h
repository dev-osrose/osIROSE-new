#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PLAYER_CHAR, SrvPlayerChar)
class SrvPlayerChar : public CRosePacket {
	public:
		SrvPlayerChar();
		SrvPlayerChar(Entity entity);

		virtual ~SrvPlayerChar() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}