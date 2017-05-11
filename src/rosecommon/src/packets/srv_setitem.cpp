#include "srv_setitem.h"

namespace RoseCommon {

SrvSetItem::SrvSetItem() : CRosePacket(ePacketType::PAKWC_SET_ITEM) {}

SrvSetItem::SrvSetItem(Entity entity, const std::vector<uint8_t> &list) : CRosePacket(ePacketType::PAKWC_SET_ITEM), entity_(entity), list_(list) {}

Entity SrvSetItem::entity() const {
	return entity_;
}

const std::vector<uint8_t> &SrvSetItem::list() const {
	return list_;
}


void SrvSetItem::pack() {
	auto inventory = entity_.component<Inventory>();

    *this << static_cast<uint8_t>(list_.size());
    for (auto &id : list_) {
        *this << id;
        *this << inventory->items_[id].getHeader();
        *this << inventory->items_[id].getData();
    }
}

}
