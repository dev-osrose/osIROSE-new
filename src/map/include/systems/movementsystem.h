#pragma once

#include "system.h"

namespace RoseCommon {
class CliMouseCmd;
class CliStopMoving;
}  // namespace RoseCommon

namespace Systems {

class MovementSystem : public System {
 public:
  MovementSystem(SystemManager &manager);
  virtual ~MovementSystem() = default;

  virtual void update(EntityManager &es, double dt);

  void move(Entity entity, float x, float y, uint16_t target = 0);

  void stop(Entity entity, float x, float y);

  void processMove(CMapClient &client, Entity entity, const RoseCommon::CliMouseCmd &packet);
  void stopMoving(CMapClient &client, Entity entity, const RoseCommon::CliStopMoving &packet);
};

}  // namespace Systems
