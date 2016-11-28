#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <item.h>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_EQUIP_ITEM, SrvEquipItem)
class SrvEquipItem : public CRosePacket {
	public:
		SrvEquipItem();
		SrvEquipItem(uint16_t id, uint8_t slot, const Item &item);

		virtual ~SrvEquipItem() = default;

		uint16_t id() const;
		uint8_t slot() const;
		Item &item();
		const Item &item() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t id_;
		uint8_t slot_;
		Item item_;
};

}