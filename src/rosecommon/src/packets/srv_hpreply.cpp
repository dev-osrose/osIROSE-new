#include "srv_hpreply.h"

namespace RoseCommon {

SrvHpReply::SrvHpReply() : CRosePacket(ePacketType::PAKWC_HP_REPLY) {}

SrvHpReply::SrvHpReply(Entity entity) : CRosePacket(ePacketType::PAKWC_HP_REPLY), entity_(entity) {}

Entity SrvHpReply::entity() const {
  return entity_;
}

void SrvHpReply::pack() {
  auto basicInfo = entity_.component<BasicInfo>();
  auto advanced = entity_.component<AdvancedInfo>();

  *this << basicInfo->id_;
  *this << advanced->hp_;
  
}

}