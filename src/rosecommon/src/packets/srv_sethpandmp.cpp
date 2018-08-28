#include "srv_sethpandmp.h"

namespace RoseCommon {

SrvSetHpAndMp::SrvSetHpAndMp() : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP) {}

SrvSetHpAndMp::SrvSetHpAndMp(Entity entity) : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP), entity_(entity) {}

Entity SrvSetHpAndMp::entity() const {
  return entity_;
}

void SrvSetHpAndMp::pack() {
  auto basicInfo = entity_.component<BasicInfo>();
  auto advanced = entity_.component<AdvancedInfo>();

  *this << basicInfo->id_;
  *this << advanced->hp_;
  *this << advanced->mp_;
}

}