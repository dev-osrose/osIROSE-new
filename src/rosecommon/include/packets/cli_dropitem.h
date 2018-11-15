#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_DROP_ITEM, CliDropItem)
REGISTER_SEND_PACKET(ePacketType::PAKCS_DROP_ITEM, CliDropItem)
class CliDropItem : public CRosePacket {
	public:
		CliDropItem();
		CliDropItem(CRoseReader reader);
	private:
		CliDropItem(uint8_t item, uint32_t quantity);
	public:

		virtual ~CliDropItem() = default;

		uint8_t item() const;
		CliDropItem& set_item(uint8_t);
		uint32_t quantity() const;
		CliDropItem& set_quantity(uint32_t);

		static CliDropItem create(uint8_t item, uint32_t quantity);
		static CliDropItem create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint8_t item_;
		uint32_t quantity_;
};

}