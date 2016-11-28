#include "cli_equipitem.h"

namespace RoseCommon {

CliEquipItem::CliEquipItem() : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM) {}

CliEquipItem::CliEquipItem(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_EQUIP_ITEM)
		throw std::runtime_error("Not the right packet!");
	*this >> slotTo_;
	*this >> slotFrom_;
}

CliEquipItem::CliEquipItem(uint8_t slotTo, uint8_t slotFrom) : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM), slotTo_(slotTo), slotFrom_(slotFrom) {}

uint8_t CliEquipItem::slotTo() const {
	return slotTo_;
}

uint8_t CliEquipItem::slotFrom() const {
	return slotFrom_;
}


void CliEquipItem::pack() {
	*this << slotTo_;
	*this << slotFrom_;
}

}