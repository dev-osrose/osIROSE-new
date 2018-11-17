#include "srv_questdata.h"
#include "throwassert.h"

namespace RoseCommon {

SrvQuestData::SrvQuestData() : CRosePacket(ePacketType::PAKWC_QUEST_DATA) {}

SrvQuestData::SrvQuestData(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_QUEST_DATA, "Not the right packet: " << to_underlying(get_type()));
	reader.get_iserialize(quests_);
	reader.get_iserialize(wishlist_);
}

SrvQuestData::SrvQuestData(const Quests& quests, const Wishlist& wishlist) : CRosePacket(ePacketType::PAKWC_QUEST_DATA), quests_(quests), wishlist_(wishlist) {
}

const Quests& SrvQuestData::quests() const { return quests_; }

SrvQuestData& SrvQuestData::set_quests(const Quests& data) { quests_ = data; return *this; }

const Wishlist& SrvQuestData::wishlist() const { return wishlist_; }

SrvQuestData& SrvQuestData::set_wishlist(const Wishlist& data) { wishlist_ = data; return *this; }


void SrvQuestData::pack(CRoseWriter& writer) const {
	writer.set_iserialize(quests_);
	writer.set_iserialize(wishlist_);
}

uint16_t SrvQuestData::get_size() const {
	uint16_t size = 0;
	size += quests_.get_size();
	size += wishlist_.get_size();
	return size;
}


SrvQuestData SrvQuestData::create(Entity entity) {
	const auto entity_wishlist = entity.component<Wishlist>();
	const auto entity_quests = entity.component<Quests>();

	return SrvQuestData(*entity_quests, *entity_wishlist);
}

SrvQuestData SrvQuestData::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvQuestData(reader);
}

}