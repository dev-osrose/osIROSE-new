#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_EQUIP_ITEM, SrvEquipItem)
class SrvEquipItem : public CRosePacket {
	public:
		SrvEquipItem();
		SrvEquipItem(Entity entity, int16_t slot);

		virtual ~SrvEquipItem() = default;

		Entity entity() const;
		int16_t slot() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
		int16_t slot_;
};

}
