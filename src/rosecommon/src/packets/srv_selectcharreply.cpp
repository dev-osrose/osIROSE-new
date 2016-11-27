#include "srv_selectcharreply.h"

namespace RoseCommon {

SrvSelectCharReply::SrvSelectCharReply() : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY) {}

SrvSelectCharReply::SrvSelectCharReply(Entity entity) : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY), entity_(entity) {}

Entity SrvSelectCharReply::entity() const {
	return entity_;
}

void SrvSelectCharReply::pack() {
	auto basicInfo = entity_.component<BasicInfo>();
	auto position = entity_.component<Position>();
	auto statusEffects = entity_.component<StatusEffects>();
	auto characterInfo = entity_.component<CharacterInfo>();
	auto characterGraphics = entity_.component<CharacterGraphics>();
	auto stats = entity_.component<Stats>();
	auto skills = entity_.component<Skills>();
	auto hotbar = entity_.component<Hotbar>();
	auto inventory = entity_.component<Inventory>();
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this << characterGraphics->race_;
	*this << position->map_;
	*this << position->x_;
	*this << position->y_;
	*this << position->spawn_;
    for (auto &it : inventory->getEquipped()) {
        *this << it.getVisible();
    }
	*this << characterInfo->stone_;
	*this << characterGraphics->face_;
	*this << characterGraphics->hair_;
	*this << characterInfo->job_;
	*this << characterInfo->factionId_;
	*this << characterInfo->factionRank_;
	*this << characterInfo->fame_;
	*this << stats->str_;
	*this << stats->dex_;
	*this << stats->int_;
	*this << stats->con_;
	*this << stats->charm_;
	*this << stats->sense_;
	*this << advancedInfo->hp_;
	*this << advancedInfo->mp_;
	*this << basicInfo->xp_;
	*this << basicInfo->level_;
	*this << characterInfo->statPoints_;
	*this << characterInfo->skillPoints_;
	*this << stats->bodySize_;
	*this << stats->headSize_;
	*this << characterInfo->penaltyXp_;
	*this << characterInfo->factionFame_;
	*this << characterInfo->factionPoints_;
	*this << characterInfo->guildId_;
	*this << characterInfo->guildContribution_;
	*this << characterInfo->guildRank_;
	*this << characterInfo->pkFlag_;
	*this << characterInfo->stamina_;
	for (auto &it : statusEffects->effects_) {
        *this << it.expiredSeconds_ << it.value_ << it.unkown_;
	}
	*this << characterInfo->patHp_;
	*this << characterInfo->patCooldownTime_;
	for (auto &it : skills->skills_) {
        *this << it.id_;
	}
	for (auto &it : hotbar->items_) {
        *this << it.item_;
	}
	*this << basicInfo->tag_;
	*this << basicInfo->name_;

}

}
