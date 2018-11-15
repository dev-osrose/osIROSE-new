#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_EQUIP_ITEM, SrvEquipItem)
REGISTER_SEND_PACKET(ePacketType::PAKWC_EQUIP_ITEM, SrvEquipItem)
class SrvEquipItem : public CRosePacket {
	public:
		SrvEquipItem();
		SrvEquipItem(CRoseReader reader);
	private:
		SrvEquipItem(uint16_t id, int16_t slot, uint32_t item);
	public:

		virtual ~SrvEquipItem() = default;

		uint16_t id() const;
		SrvEquipItem& set_id(uint16_t);
		int16_t slot() const;
		SrvEquipItem& set_slot(int16_t);
		uint32_t item() const;
		SrvEquipItem& set_item(uint32_t);

		static SrvEquipItem create(Entity entity, int16_t slot);
		static SrvEquipItem create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t id_;
		int16_t slot_;
		uint32_t item_;
};

}