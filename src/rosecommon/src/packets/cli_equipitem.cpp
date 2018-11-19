#include "cli_equipitem.h"
#include "throwassert.h"

namespace RoseCommon {

CliEquipItem::CliEquipItem() : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM) {}

CliEquipItem::CliEquipItem(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_EQUIP_ITEM, "Not the right packet: " << to_underlying(get_type()));
	reader.get_int16_t(slotTo_);
	reader.get_int16_t(slotFrom_);
}

CliEquipItem::CliEquipItem(int16_t slotTo, int16_t slotFrom) : CRosePacket(ePacketType::PAKCS_EQUIP_ITEM), slotTo_(slotTo), slotFrom_(slotFrom) {
}

int16_t CliEquipItem::slotTo() const { return slotTo_; }

CliEquipItem& CliEquipItem::set_slotTo(int16_t data) { slotTo_ = data; return *this; }

int16_t CliEquipItem::slotFrom() const { return slotFrom_; }

CliEquipItem& CliEquipItem::set_slotFrom(int16_t data) { slotFrom_ = data; return *this; }


void CliEquipItem::pack(CRoseBasePolicy& writer) const {
	writer.set_int16_t(slotTo_);
	writer.set_int16_t(slotFrom_);
}

CliEquipItem CliEquipItem::create(int16_t slotTo, int16_t slotFrom) {


	return CliEquipItem(slotTo, slotFrom);
}

CliEquipItem CliEquipItem::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliEquipItem(reader);
}
std::unique_ptr<CliEquipItem> CliEquipItem::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliEquipItem>(reader);
}

}