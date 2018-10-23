#include "srv_damage.h"

namespace RoseCommon {

SrvDamage::SrvDamage() : CRosePacket(ePacketType::PAKWC_DAMAGE) {}

SrvDamage::SrvDamage(Entity attacker, Entity defender, uint32_t damage, uint32_t action) : CRosePacket(ePacketType::PAKWC_DAMAGE), attacker_(attacker), defender_(defender), damage_(damage), action_(action) {}

Entity SrvDamage::attacker() const {
  return attacker_;
}

Entity SrvDamage::defender() const {
  return defender_;
}

uint32_t SrvDamage::damage() const {
  return damage_;
}

uint32_t SrvDamage::action() const {
  return action_;
}

void SrvDamage::pack() {
  auto attackerInfo = attacker_.component<BasicInfo>();
  auto defenderInfo = defender_.component<BasicInfo>();

  *this << attackerInfo->id_;
  *this << defenderInfo->id_;
  *this << damage_;
  *this << action_;
}

}