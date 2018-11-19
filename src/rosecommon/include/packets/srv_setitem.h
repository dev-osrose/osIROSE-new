#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>
#include <vector>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_SET_ITEM, SrvSetItem)
REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_ITEM, SrvSetItem)
class SrvSetItem : public CRosePacket {
	public:
		SrvSetItem();
		SrvSetItem(CRoseReader reader);
	public:

		virtual ~SrvSetItem() = default;

		const std::vector<SetItem::Item>& items() const;
		SrvSetItem& set_items(const std::vector<SetItem::Item>&);
		SrvSetItem& add_item(const SetItem::Item&);

		static SrvSetItem create();
		static SrvSetItem create(uint8_t *buffer);
		static std::unique_ptr<SrvSetItem> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		std::vector<SetItem::Item> items_;
};

}