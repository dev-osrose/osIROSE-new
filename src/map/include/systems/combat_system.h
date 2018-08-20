#pragma once

#include "system.h"
#include "hash.h"

#include <unordered_map>
#include <vector>
#include <tuple>

namespace RoseCommon {
class CliMouseCmd;
class CliStopMoving;
}  // namespace RoseCommon

namespace Systems {

class CombatSystem : public System {
 public:
  CombatSystem(SystemManager &manager);
  virtual ~CombatSystem() = default;

  virtual void update(EntityManager &es, std::chrono::milliseconds dt) override;

  void apply_damage(Entity entity, uint32_t damage);
  
 protected:
  void updateHP(Entity e);
};

}  // namespace Systems
