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

std::tuple<uint16_t, uint16_t> get_grid_position(const RoseCommon::Registry& registry, RoseCommon::Entity e) {
    const auto* pos = registry.try_get<Component::Position>(e);
    if (!pos) return {0, 0};
    return get_grid_position(pos->x, pos->y);
}

std::tuple<uint16_t, uint16_t> get_grid_position(const EntitySystem& entitySystem, RoseCommon::Entity e) {
    const auto* pos = entitySystem.try_get_component<Component::Position>(e);
    if (!pos) return {0, 0};
    return get_grid_position(pos->x, pos->y);
}
}

void Nearby::add_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity) {
  if (entity == entt::null || !registry.valid(entity)) return;
  grid[get_grid_position(registry, entity)].push_back(entity);
}

void Nearby::remove_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity) {
  if (entity == entt::null || !registry.valid(entity)) return;
  auto& list = grid[get_grid_position(registry, entity)];
  list.erase(std::remove(list.begin(), list.end(), entity), list.end());
}
    
bool Nearby::is_nearby(const EntitySystem& entitySystem, RoseCommon::Entity first, RoseCommon::Entity second) const {
  if (first == entt::null || second == entt::null) return false;
    auto pos_first = get_grid_position(entitySystem, first);
    auto pos_second = get_grid_position(entitySystem, second);
    if (std::abs(std::get<0>(pos_first) - std::get<0>(pos_second)) <= 10
        && std::abs(std::get<1>(pos_first) - std::get<1>(pos_second)) <= 10)
        return true;
    return false;
}
    
std::vector<RoseCommon::Entity> Nearby::get_nearby(const EntitySystem& entitySystem, RoseCommon::Entity entity) const {
  std::vector<RoseCommon::Entity> res;
  // TODO: populate res and sort it by entity
  auto pos = get_grid_position(entitySystem, entity);
  for (uint16_t x = std::max(0, std::get<0>(pos) - 10); x < std::get<0>(pos) + 10; ++x) {
      for (uint16_t y = std::max(0, std::get<1>(pos) - 10); y < std::get<1>(pos) + 10; ++y) {
          if (const auto it = grid.find({x, y}); it != grid.cend()) {
              res.insert(res.end(), it->second.cbegin(), it->second.cend());
          }
      }
  }
  std::sort(res.begin(), res.end());
  return res;
}
    
void Nearby::update_position(RoseCommon::Entity entity, float old_x, float old_y, float x, float y) {
  if (old_x && old_y) {
    auto &list = grid[get_grid_position(old_x, old_y)];
    list.erase(std::remove(list.begin(), list.end(), entity), list.end());
  }
  grid[get_grid_position(x, y)].push_back(entity);
}
