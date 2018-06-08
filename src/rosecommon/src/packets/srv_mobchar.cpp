#include "srv_mobchar.h"

namespace RoseCommon {

SrvMobChar::SrvMobChar(ePacketType type) : CRosePacket(type) {}

SrvMobChar::SrvMobChar(Entity entity, ePacketType type) : CRosePacket(type), entity_(entity) {}

Entity SrvMobChar::entity() const {
	return entity_;
}

void SrvMobChar::pack() {
	auto advancedInfo = entity_.component<AdvancedInfo>();
	auto basicInfo = entity_.component<BasicInfo>();
	auto characterInfo = entity_.component<CharacterInfo>();
	auto position = entity_.component<Position>();
	auto destination = entity_.component<Destination>();
    auto npc = entity_.component<Npc>();
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
	
	// If the ID below is less then 0, we hide the MOB/NPC
	// MOB & NPC
	*this << npc.npc_id_;
	*this << npc.quest_id_;
}

}
