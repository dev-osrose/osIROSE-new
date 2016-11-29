#include "srv_playerchar.h"

namespace RoseCommon {

SrvPlayerChar::SrvPlayerChar() : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR) {}

SrvPlayerChar::SrvPlayerChar(Entity entity) : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR), entity_(entity) {}

Entity SrvPlayerChar::entity() const {
	return entity_;
}

void SrvPlayerChar::pack() {
	auto inventory = entity_.component<Inventory>();
	auto ridingItems = entity_.component<RidingItems>();
	auto advancedInfo = entity_.component<AdvancedInfo>();
	auto basicInfo = entity_.component<BasicInfo>();
	auto bulletItems = entity_.component<BulletItems>();
	auto characterGraphics = entity_.component<CharacterGraphics>();
	auto characterInfo = entity_.component<CharacterInfo>();
	auto position = entity_.component<Position>();
	auto destination = entity_.component<Destination>();
    float destX = position->x_, destY = position->y_;
    if (destination) {
        destX = destination->x_;
        destY = destination->y_;
    }

	*this << basicInfo->id_;
	*this << position->x_;
	*this << position->y_;
    *this << destX;
    *this << destY;
	*this << basicInfo->command_;
	*this << basicInfo->targetId_;
	*this << advancedInfo->moveMode_;
	*this << advancedInfo->hp_;
	*this << basicInfo->teamId_;
	*this << characterInfo->statusFlag_;
	*this << characterGraphics->race_;
	*this << advancedInfo->runSpeed_;
	*this << advancedInfo->atkSpeed_;
	*this << advancedInfo->weightRate_;
	*this << (uint32_t)characterGraphics->face_;
	*this << (uint32_t)characterGraphics->hair_;
    for (auto &it : inventory->getEquipped()) {
        *this << it.getVisible();
    }
    for (auto &it : bulletItems->items_) {
        *this << it.getHeader();
    }
	*this << characterInfo->job_;
	*this << basicInfo->level_;
    for (auto &it : ridingItems->items_) {
        *this << it.getVisible();
    }
	*this << position->z_;
	*this << characterInfo->subFlag_;
	*this << basicInfo->name_;
	*this << basicInfo->name_;

}

}
