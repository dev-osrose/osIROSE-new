#include "srv_inventorydata.h"

namespace RoseCommon {

SrvInventoryData::SrvInventoryData() : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA) {}

SrvInventoryData::SrvInventoryData(Entity entity) : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), entity_(entity) {}

Entity SrvInventoryData::entity() const {
	return entity_;
}


void SrvInventoryData::pack() {
	auto advancedInfo = entity_.component<AdvancedInfo>();
	auto inventory = entity_.component<Inventory>();

	*this << advancedInfo->zuly_;
  bool faceSkipped = false;
	for (auto &it : inventory->items_) {
    if (!faceSkipped) {
      faceSkipped = true;
      continue;
    }
		*this << it.getHeader() << it.getData();
	}

}

}
