#include "cli_dropitem.h"
#include "throwassert.h"

namespace RoseCommon {

CliDropItem::CliDropItem() : CRosePacket(ePacketType::PAKCS_DROP_ITEM) {}

CliDropItem::CliDropItem(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_DROP_ITEM, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(item_);
	reader.get_uint32_t(quantity_);
}

CliDropItem::CliDropItem(uint8_t item, uint32_t quantity) : CRosePacket(ePacketType::PAKCS_DROP_ITEM), item_(item), quantity_(quantity) {
}

uint8_t CliDropItem::item() const { return item_; }

CliDropItem& CliDropItem::set_item(uint8_t data) { item_ = data; return *this; }

uint32_t CliDropItem::quantity() const { return quantity_; }

CliDropItem& CliDropItem::set_quantity(uint32_t data) { quantity_ = data; return *this; }


void CliDropItem::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(item_);
	writer.set_uint32_t(quantity_);
}

uint16_t CliDropItem::get_size() const {
	uint16_t size = 0;
	size += sizeof(item_);
	size += sizeof(quantity_);
	return size;
}


CliDropItem CliDropItem::create(uint8_t item, uint32_t quantity) {


	return CliDropItem(item, quantity);
}

CliDropItem CliDropItem::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliDropItem(reader);
}

}