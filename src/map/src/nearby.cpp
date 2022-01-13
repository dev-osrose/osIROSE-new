#include "nearby.h"
#include "entity_system.h"

#include <algorithm>
#include <set>

namespace {
constexpr std::tuple<uint16_t, uint16_t> get_grid_position(float x, float y) {
    uint16_t gx = x / 1000.f;
    uint16_t gy = y / 1000.f;
    return {gx, gy};
}

std::tuple<uint16_t, uint16_t> get_grid_position(const Component::Position& pos) {
    return get_grid_position(pos.x, pos.y);
}

std::tuple<uint16_t, uint16_t> get_grid_position(const EntitySystem& entitySystem, Entity e) {
    const auto* pos = entitySystem.try_get_component<Component::Position>(e);
    if (!pos) return {0, 0};
    return get_grid_position(pos->x, pos->y);
}
}

void Nearby::add_entity(Registry& registry, Entity entity) {
  if (entity == entt::null || !registry.valid(entity)) return;
  if (const auto* pos = registry.try_get<Component::Position>(entity)) {
    grid[get_grid_position(*pos)].insert(entity);
  }
}

void Nearby::remove_entity(Registry& registry, Entity entity) {
  if (entity == entt::null || !registry.valid(entity)) return;
  if (const auto* pos = registry.try_get<Component::Position>(entity)) {
      auto& list = grid[get_grid_position(*pos)];
      list.erase(entity);
  }
}
    
bool Nearby::is_nearby(const EntitySystem& entitySystem, Entity first, Entity second) const {
  if (first == entt::null || second == entt::null) return false;
    auto pos_first = get_grid_position(entitySystem, first);
    auto pos_second = get_grid_position(entitySystem, second);
    if (std::abs(std::get<0>(pos_first) - std::get<0>(pos_second)) <= 10
        && std::abs(std::get<1>(pos_first) - std::get<1>(pos_second)) <= 10)
        return true;
    return false;
}
    
std::vector<Entity> Nearby::get_nearby(const EntitySystem& entitySystem, Entity entity) const {
  std::set<Entity> tmp, result;
  auto pos = get_grid_position(entitySystem, entity);
  for (uint16_t x = std::max(0, std::get<0>(pos) - 10); x < std::get<0>(pos) + 10; ++x) {
      for (uint16_t y = std::max(0, std::get<1>(pos) - 10); y < std::get<1>(pos) + 10; ++y) {
          if (const auto it = grid.find({x, y}); it != grid.cend()) {
              tmp.insert(it->second.cbegin(), it->second.cend());
          }
      }
  }

  for (auto en : tmp) {
    if (entitySystem.is_valid(en)) {
      result.insert(en);
    } else {
      //TODO: Remove invalid entity from the list
    }
  }
  result.erase(entity);
  return {result.cbegin(), result.cend()};
}
    
void Nearby::update_position(Entity entity, float old_x, float old_y, float x, float y) {
  if (old_x && old_y) {
    auto &list = grid[get_grid_position(old_x, old_y)];
    list.erase(entity);
  }
  grid[get_grid_position(x, y)].insert(entity);
}
