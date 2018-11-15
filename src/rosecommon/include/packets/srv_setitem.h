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

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		std::vector<SetItem::Item> items_;
};

}