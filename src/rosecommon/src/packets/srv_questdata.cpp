#include "srv_questdata.h"
#include "throwassert.h"

namespace RoseCommon {

SrvQuestData::SrvQuestData() : CRosePacket(ePacketType::PAKWC_QUEST_DATA) {}

SrvQuestData::SrvQuestData(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKWC_QUEST_DATA, "Not the right packet: " << to_underlying(type()));
}

SrvQuestData::SrvQuestData(Entity entity) : CRosePacket(ePacketType::PAKWC_QUEST_DATA), entity_(entity) {}

Entity SrvQuestData::entity() const {
	return entity_;
}


void SrvQuestData::pack() {
  const auto *quests = entity_.component<Quests>();
  const auto *wishlist = entity_.component<Wishlist>();

  *this << *quests;
  *this << *wishlist;
}

}
