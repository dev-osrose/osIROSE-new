#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_STOP, SrvStop)
class SrvStop : public CRosePacket {
	public:
		SrvStop();
		SrvStop(Entity entity);

		virtual ~SrvStop() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}