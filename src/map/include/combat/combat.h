#pragma once

#include <tuple>
#include <utility>
#include "entity.h"
#include "cli_attack.h"
#include "cli_hp_req.h"
#include "cli_revive_req.h"

class EntitySystem;

namespace Combat {
  void attack(EntitySystem&, Entity, const RoseCommon::Packet::CliAttack&);
  void hp_request(EntitySystem&, Entity, const RoseCommon::Packet::CliHpReq&);
  void revive(EntitySystem&, Entity, const RoseCommon::Packet::CliReviveReq&);

  void update(EntitySystem&, Entity, uint32_t dt);
  void updateStats(EntitySystem&, Entity);
  void check_for_level_up(EntitySystem& entitySystem, Entity entity, uint16_t sourceId);

  std::pair<float, float> get_range_position(const EntitySystem& entitySystem, Entity character, Entity target, float range = 100);
  float get_range_to(const EntitySystem& entitySystem, Entity character, Entity target);

  Entity get_closest_spawn(EntitySystem& entitySystem, Entity player);
  Entity get_saved_spawn(EntitySystem& entitySystem, Entity player);
  Entity get_start_spawn(EntitySystem& entitySystem);

  int64_t get_exp_to_level(int level);

  std::tuple<uint16_t, float, float> get_spawn_point(EntitySystem& entitySystem, Entity entity, int type = 1);

  void drop_loot(EntitySystem& entitySystem, Entity entity, Entity owner);
}
