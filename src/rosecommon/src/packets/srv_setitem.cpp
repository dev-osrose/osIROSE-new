#include "srv_setitem.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSetItem::SrvSetItem() : CRosePacket(ePacketType::PAKWC_SET_ITEM) {}

SrvSetItem::SrvSetItem(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_SET_ITEM, "Not the right packet: " << to_underlying(get_type()));
	{
		uint8_t index = 0;
		reader.get_uint8_t(index);
		for (uint8_t i = 0; i < index; ++i) {
			SetItem::Item tmp;
			reader.get_iserialize(tmp);
			items_.push_back(tmp);
		}
	}
}

const std::vector<SetItem::Item>& SrvSetItem::items() const { return items_; }

SrvSetItem& SrvSetItem::set_items(const std::vector<SetItem::Item>& data) { items_ = data; return *this; }

SrvSetItem& SrvSetItem::add_item(const SetItem::Item& data) { items_.push_back(data); return *this; }


void SrvSetItem::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(items_.size());
	for (const auto& elem : items_) writer.set_iserialize(elem);
}

SrvSetItem SrvSetItem::create() {


	return SrvSetItem();
}

SrvSetItem SrvSetItem::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSetItem(reader);
}
std::unique_ptr<SrvSetItem> SrvSetItem::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvSetItem>(reader);
}

}