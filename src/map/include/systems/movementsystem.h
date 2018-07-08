#pragma once

#include "system.h"
#include "hash.h"

#include <unordered_map>
#include <vector>
#include <tuple>

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

  using key_t = std::tuple<uint16_t, uint16_t>;

  std::unordered_map<key_t, std::vector<Entity>, Core::tuple_hash> grid;
};

}  // namespace Systems
