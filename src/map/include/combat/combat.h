#pragma once

#include "dataconsts.h"
#include "cli_attack.h"
#include "cli_hp_req.h"
#include "cli_revive_req.h"

class EntitySystem;

namespace Combat {
  void attack(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliAttack&);
  void hp_request(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliHpReq&);
  void revive(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliReviveReq&);
  
  void update(EntitySystem&, RoseCommon::Entity);
}
