#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_EQUIP_ITEM, CliEquipItem)
class CliEquipItem : public CRosePacket {
	public:
		CliEquipItem();
		CliEquipItem(uint8_t buffer[MAX_PACKET_SIZE]);
		CliEquipItem(uint8_t slotTo, uint8_t slotFrom);

		virtual ~CliEquipItem() = default;

		uint8_t slotTo() const;
		uint8_t slotFrom() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t slotTo_;
		uint8_t slotFrom_;
};

}