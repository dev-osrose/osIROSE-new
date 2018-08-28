#include "srv_damage.h"

namespace RoseCommon {

SrvDamage::SrvDamage() : CRosePacket(ePacketType::PAKWC_DAMAGE) {}

SrvDamage::SrvDamage(Entity attacker, Entity defender, uint32_t damage) : CRosePacket(ePacketType::PAKWC_DAMAGE), attacker_(attacker), defender_(defender), damage_(damage) {}

Entity SrvDamage::attacker() const {
  return attacker_;
}

Entity SrvDamage::defender() const {
  return defender_;
}

void SrvDamage::pack() {
  auto attackerInfo = attacker_.component<BasicInfo>();
  auto defenderInfo = defender_.component<BasicInfo>();

  *this << attackerInfo->id_;
  *this << defenderInfo->id_;
  *this << damage_;
}

}