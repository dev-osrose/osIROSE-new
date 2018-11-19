#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_EQUIP_ITEM, CliEquipItem)
REGISTER_SEND_PACKET(ePacketType::PAKCS_EQUIP_ITEM, CliEquipItem)
class CliEquipItem : public CRosePacket {
	public:
		CliEquipItem();
		CliEquipItem(CRoseReader reader);
	private:
		CliEquipItem(int16_t slotTo, int16_t slotFrom);
	public:

		virtual ~CliEquipItem() = default;

		int16_t slotTo() const;
		CliEquipItem& set_slotTo(int16_t);
		int16_t slotFrom() const;
		CliEquipItem& set_slotFrom(int16_t);

		static CliEquipItem create(int16_t slotTo, int16_t slotFrom);
		static CliEquipItem create(uint8_t *buffer);
		static std::unique_ptr<CliEquipItem> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		int16_t slotTo_;
		int16_t slotFrom_;
};

}