#include "cli_equipitem.h"
#include "throwassert.h"

namespace RoseCommon {

CliEquipItem::CliEquipItem() : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM) {}

CliEquipItem::CliEquipItem(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() != ePacketType::PAKCS_EQUIP_ITEM, "Not the right packet: " << to_underlying(type()));
    *this >> slotTo_;
	*this >> slotFrom_;
}

CliEquipItem::CliEquipItem(int16_t slotTo, int16_t slotFrom) : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM), slotTo_(slotTo), slotFrom_(slotFrom) {}

int16_t CliEquipItem::slotTo() const {
	return slotTo_;
}

int16_t CliEquipItem::slotFrom() const {
	return slotFrom_;
}


void CliEquipItem::pack() {
	*this << slotTo_;
	*this << slotFrom_;
}

}
