#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_EQUIP_ITEM, CliEquipItem)
class CliEquipItem : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliEquipItem();
		CliEquipItem(uint8_t buffer[MAX_PACKET_SIZE]);
		CliEquipItem(int16_t slotTo, int16_t slotFrom);

		virtual ~CliEquipItem() = default;

		int16_t slotTo() const;
		int16_t slotFrom() const;

	protected:
		virtual void pack() override;

	private:
		int16_t slotTo_;
		int16_t slotFrom_;
};

}