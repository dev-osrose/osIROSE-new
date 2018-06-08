#include "srv_npcchar.h"

namespace RoseCommon {

SrvNpcChar::SrvNpcChar() : SrvMobChar(ePacketType::PAKWC_NPC_CHAR) {}

SrvNpcChar::SrvNpcChar(Entity entity) : SrvMobChar(entity, ePacketType::PAKWC_NPC_CHAR) {}

Entity SrvNpcChar::entity() const {
	return entity_;
}

void SrvNpcChar::pack() {
  
  // Since the npcs are the same as mobs, just call mob's pack then add NPC's data on top of it
	SrvMobChar::pack();

    auto pos = entity_.component<Position>();
	
	// NPC only fields
	*this << pos->angle_;
	*this << uint16_t(0); // Event status
}

}
