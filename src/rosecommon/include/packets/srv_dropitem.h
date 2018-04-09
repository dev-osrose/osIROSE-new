#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_DROP_ITEM, SrvDropItem)
class SrvDropItem : public CRosePacket {
	public:
		SrvDropItem();
		SrvDropItem(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvDropItem(Entity entity);

		virtual ~SrvDropItem() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}