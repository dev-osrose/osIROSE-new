#pragma once

#include "system.h"

namespace Systems {

class UpdateSystem : public System {
 public:
  UpdateSystem(SystemManager &manager) : System(manager) {}
  virtual ~UpdateSystem() = default;

  virtual void update(EntityManager &es, std::chrono::milliseconds) override;

  static void calculateSpeed(Entity entity);

  static void calculateAtkSpeed(Entity entity);

  static void calculateCommand(Entity entity);
};

}  // namespace Systems
