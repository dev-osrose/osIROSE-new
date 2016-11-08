#include "srv_inventorydata.h"

namespace RoseCommon {

SrvInventoryData::SrvInventoryData() : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA) {
}

SrvInventoryData::SrvInventoryData(Entity entity) : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), entity_(entity) {
}

Entity &SrvInventoryData::entity() {
	return entity_;
}

void SrvInventoryData::pack() {
	auto inventory = entity_.component<Inventory>();
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this << advancedInfo->zuly_;
	for (auto &it : inventory->items_) *this << static_cast<ISerialize&>(it);

}

}