#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_ADJUST_POSITION, SrvAdjustPosition)
class SrvAdjustPosition : public CRosePacket {
	public:
		SrvAdjustPosition();
		SrvAdjustPosition(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvAdjustPosition(Entity entity);

		virtual ~SrvAdjustPosition() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}