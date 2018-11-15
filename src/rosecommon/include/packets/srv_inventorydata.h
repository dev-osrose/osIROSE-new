#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <vector>
#include <item.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_INVENTORY_DATA, SrvInventoryData)
REGISTER_SEND_PACKET(ePacketType::PAKWC_INVENTORY_DATA, SrvInventoryData)
class SrvInventoryData : public CRosePacket {
	public:
		SrvInventoryData();
		SrvInventoryData(CRoseReader reader);
	private:
		SrvInventoryData(int64_t zuly);
	public:

		virtual ~SrvInventoryData() = default;

		int64_t zuly() const;
		SrvInventoryData& set_zuly(int64_t);
		const RoseCommon::Item* items() const;
		const RoseCommon::Item& items(size_t index) const;
		SrvInventoryData& set_items(const RoseCommon::Item[Inventory::maxItems]);
		SrvInventoryData& set_items(const RoseCommon::Item&, size_t index);

		static SrvInventoryData create(Entity entity);
		static SrvInventoryData create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		int64_t zuly_;
		RoseCommon::Item items_[Inventory::maxItems];
};

}