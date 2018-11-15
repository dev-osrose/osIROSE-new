#include "srv_inventorydata.h"
#include "throwassert.h"

namespace RoseCommon {

SrvInventoryData::SrvInventoryData() : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA) {}

SrvInventoryData::SrvInventoryData(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_INVENTORY_DATA, "Not the right packet: " << to_underlying(get_type()));
	reader.get_int64_t(zuly_);
	for (size_t index = 0; index < Inventory::maxItems; ++index) reader.get_iserialize(items_[index]);
}

SrvInventoryData::SrvInventoryData(int64_t zuly) : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), zuly_(zuly) {
}

int64_t SrvInventoryData::zuly() const { return zuly_; }

SrvInventoryData& SrvInventoryData::set_zuly(int64_t data) { zuly_ = data; return *this; }

const RoseCommon::Item* SrvInventoryData::items() const { return items_; }

const RoseCommon::Item& SrvInventoryData::items(size_t index) const { return items_[index]; }

SrvInventoryData& SrvInventoryData::set_items(const RoseCommon::Item* data) { for (size_t index = 0; index < Inventory::maxItems; ++index) items_[index] = data[index]; return *this; }

SrvInventoryData& SrvInventoryData::set_items(const RoseCommon::Item& data, size_t index) { items_[index] = data; return *this; }


void SrvInventoryData::pack(CRoseWriter& writer) const {
	writer.set_int64_t(zuly_);
	for (size_t index = 0; index < Inventory::maxItems; ++index) writer.set_iserialize(items_[index]);
}

uint16_t SrvInventoryData::get_size() const {
	uint16_t size = 0;
	size += sizeof(zuly_);
	size += sizeof(items_);
	return size;
}


SrvInventoryData SrvInventoryData::create(Entity entity) {
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();

	return SrvInventoryData(entity_advancedinfo->zuly_);
}

SrvInventoryData SrvInventoryData::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvInventoryData(reader);
}

}