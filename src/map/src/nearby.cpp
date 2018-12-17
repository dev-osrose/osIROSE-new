#include "nearby.h"
#include "components/position.h"
#include "entity_system.h"

#include <algorithm>

namespace {
constexpr std::tuple<uint16_t, uint16_t> get_grid_position(float x, float y) {
    uint16_t gx = x / 1000.f;
    uint16_t gy = y / 1000.f;
    return {gx, gy};
}

std::tuple<uint16_t, uint16_t> get_grid_position(const Registry& registry, Entity e) {
    const auto* pos = registry.try_get_component<Component::Position>();
    if (!pos) return {0, 0};
    return get_grid_position(pos->x, pos->y);
}
}

void Nearby::add_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity) {
  if (entity == entt::null) return;
  grid[get_grid_position(registry, entity)].push_back(entity);
}

void remove_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity) {
  if (entity == entt::unll) return;
  auto& list = grid[get_grid_position(registry, entity)];
  std::remove(list.begin(), list.end(), entity);
}
    
bool is_nearby(RoseCommon::Entity first, RoseCommon::Entity second) const {
  if (first == entt::null || second == entt::null) return false;
    auto pos_first = get_grid_position(entitySystem, first);
    auto pos_second = get_grid_position(entitySystem, second);
    if (std::abs(std::get<0>(pos_first) - std::get<0>(pos_second)) <= 10
        && std::abs(std::get<1>(pos_first) - std::get<1>(pos_second)) <= 10)
        return true;
    return false;
}
    
std::vector<RoseCommon::Entity> get_nearby(RoseCommon::Entity entity) const {
  std::vector<RoseCommon::Entity> res;
  // TODO: populate res and sort it by entity
  return res;
}
    
void update_position(RoseCommon::Entity entity, float old_x, float old_y, float x, float y) {
  if (old_x && old_y) {
    auto &list = grid[get_grid_position(old_x, old_y)];
    std::remove(list.begin(), list.end(), entity);
  }
  grid[get_grid_position(x, y)].push_back(entity);
}
