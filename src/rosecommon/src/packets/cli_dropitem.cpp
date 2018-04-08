#include "cli_dropitem.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliDropItem::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_DROP_ITEM, &createPacket<CliDropItem>);

CliDropItem::CliDropItem() : CRosePacket(ePacketType::PAKCS_DROP_ITEM) {}

CliDropItem::CliDropItem(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_DROP_ITEM, "Not the right packet: " << to_underlying(type()));
	*this >> item_;
	*this >> quantity_;
}

CliDropItem::CliDropItem(uint8_t item, uint32_t quantity) : CRosePacket(ePacketType::PAKCS_DROP_ITEM), item_(item), quantity_(quantity) {}

uint8_t CliDropItem::item() const {
	return item_;
}

uint32_t CliDropItem::quantity() const {
	return quantity_;
}


void CliDropItem::pack() {
	*this << item_;
	*this << quantity_;
}

}