#include "srv_equipitem.h"
#include "throwassert.h"

namespace RoseCommon {

SrvEquipItem::SrvEquipItem() : CRosePacket(ePacketType::PAKWC_EQUIP_ITEM) {}

SrvEquipItem::SrvEquipItem(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_EQUIP_ITEM, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_int16_t(slot_);
	reader.get_uint32_t(item_);
}

SrvEquipItem::SrvEquipItem(uint16_t id, int16_t slot, uint32_t item) : CRosePacket(ePacketType::PAKWC_EQUIP_ITEM), id_(id), slot_(slot), item_(item) {
}

uint16_t SrvEquipItem::id() const { return id_; }

SrvEquipItem& SrvEquipItem::set_id(uint16_t data) { id_ = data; return *this; }

int16_t SrvEquipItem::slot() const { return slot_; }

SrvEquipItem& SrvEquipItem::set_slot(int16_t data) { slot_ = data; return *this; }

uint32_t SrvEquipItem::item() const { return item_; }

SrvEquipItem& SrvEquipItem::set_item(uint32_t data) { item_ = data; return *this; }


void SrvEquipItem::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_int16_t(slot_);
	writer.set_uint32_t(item_);
}

uint16_t SrvEquipItem::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(slot_);
	size += sizeof(item_);
	return size;
}


SrvEquipItem SrvEquipItem::create(Entity entity, int16_t slot) {
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_inventory = entity.component<Inventory>();

	return SrvEquipItem(entity_basicinfo->id_, slot, entity_inventory->items_[slot].getVisible());
}

SrvEquipItem SrvEquipItem::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvEquipItem(reader);
}

}