#include "srv_equipitem.h"

namespace RoseCommon {

SrvEquipItem::SrvEquipItem() : CRosePacket(ePacketType::PAKWC_EQUIP_ITEM) {}

SrvEquipItem::SrvEquipItem(uint16_t id, uint8_t slot, const Item &item) : CRosePacket(ePacketType::PAKWC_EQUIP_ITEM), id_(id), slot_(slot), item_(item) {}

uint16_t SrvEquipItem::id() const {
	return id_;
}

uint8_t SrvEquipItem::slot() const {
	return slot_;
}

Item &SrvEquipItem::item() {
	return item_;
}

const Item &SrvEquipItem::item() const {
	return item_;
}


void SrvEquipItem::pack() {
	*this << id_;
	*this << slot_;
	*this << item_.getVisible();
}

}
