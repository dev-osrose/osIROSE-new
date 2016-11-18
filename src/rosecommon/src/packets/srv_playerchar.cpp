#include "srv_playerchar.h"

namespace RoseCommon {

SrvPlayerChar::SrvPlayerChar() : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR) {}

SrvPlayerChar::SrvPlayerChar(Entity entity) : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR), entity_(entity) {}

Entity SrvPlayerChar::entity() const {
	return entity_;
}


void SrvPlayerChar::pack() {
	auto equippedItems = entity_.component<EquippedItems>();
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
    for (auto &it : equippedItems->items_) {
        it.partialSerialize(*this);
    }
    for (auto &it : bulletItems->items_) {
        it.bulletPartialSerialize(*this);
    }
	*this << characterInfo->job_;
	*this << basicInfo->level_;
    for (auto &it : ridingItems->items_) {
        it.partialSerialize(*this);
    }
	*this << position->z_;
	*this << characterInfo->subFlag_;
	*this << basicInfo->name_;
	*this << basicInfo->name_;

}

}
