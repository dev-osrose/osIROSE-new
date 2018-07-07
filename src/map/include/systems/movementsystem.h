#pragma once

#include "system.h"

namespace RoseCommon {
class CliMouseCmd;
class CliStopMoving;
class CliChangeMapReq;
}  // namespace RoseCommon

namespace Systems {

class MovementSystem : public System {
 public:
  MovementSystem(SystemManager &manager);
  virtual ~MovementSystem() = default;

  virtual void update(EntityManager &es, double dt);

  void move(Entity entity, float x, float y, uint16_t target = 0);

  void teleport(Entity entity, uint16_t map_id, float x, float y);

  void stop(Entity entity, float x, float y);

  bool nearby(Entity a, Entity b) const;

  void processMove(CMapClient &client, Entity entity, const RoseCommon::CliMouseCmd &packet);
  void stopMoving(CMapClient &client, Entity entity, const RoseCommon::CliStopMoving &packet);

 protected:
  void updatePosition(Entity e);
  Entity is_on_warpgate(Entity e);

  static constexpr double POSITION_CHEATING = 100.;
};

}  // namespace Systems
