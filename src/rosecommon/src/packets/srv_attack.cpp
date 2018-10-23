#include "srv_attack.h"

namespace RoseCommon {

SrvAttack::SrvAttack() : CRosePacket(ePacketType::PAKWC_ATTACK) {}

SrvAttack::SrvAttack(Entity attacker, Entity defender) : CRosePacket(ePacketType::PAKWC_ATTACK), attacker_(attacker), defender_(defender) {}

Entity SrvAttack::attacker() const {
  return attacker_;
}

Entity SrvAttack::defender() const {
  return defender_;
}

void SrvAttack::pack() {
  auto attackerInfo = attacker_.component<BasicInfo>();
  auto defenderInfo = defender_.component<BasicInfo>();
  
  auto destination = attacker_.component<Destination>();

  *this << attackerInfo->id_;
  *this << defenderInfo->id_;
 
  *this << destination->dist_;
  *this << destination->x_;
  *this << destination->y_;
  *this << destination->z_;
}

}