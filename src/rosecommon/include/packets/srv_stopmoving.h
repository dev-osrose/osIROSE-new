#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_STOP_MOVING, SrvStopMoving)
class SrvStopMoving : public CRosePacket {
	public:
		SrvStopMoving();
		SrvStopMoving(Entity entity);

		virtual ~SrvStopMoving() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}