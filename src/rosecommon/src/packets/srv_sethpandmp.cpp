#include "srv_sethpandmp.h"

namespace RoseCommon {

SrvSetHpAndMp::SrvSetHpAndMp() : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP) {}

SrvSetHpAndMp::SrvSetHpAndMp(Entity entity) : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP), entity_(entity) {}

Entity SrvSetHpAndMp::entity() const {
  return entity_;
}

void SrvSetHpAndMp::pack() {
  auto basicInfo = entity_.component<BasicInfo>();
  auto stats = entity_.component<Stats>();
  auto advanced = entity_.component<AdvancedInfo>();

  *this << basicInfo->id_;
  
  // If the value is 0, the client won't do anything
  
  if(stats) {
    if(advanced->hp_ == stats->maxHp_)
      *this << (uint16_t)0;
    else
      *this << (uint16_t)advanced->hp_;
      
    if(advanced->hp_ == stats->maxMp_)
      *this << (uint16_t)0;
    else
      *this << (uint16_t)advanced->mp_;
  } else {
    *this << (uint16_t)0;
    *this << (uint16_t)0;
  }
}

}