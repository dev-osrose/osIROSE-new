#pragma once

#include "hash.h"
#include "entity.h"
#include "components/position.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <tuple>

class EntitySystem;

class Nearby {
  public:
    void add_entity(Registry& registry, Entity entity);
    void remove_entity(Registry& registry, Entity entity);
    
    bool is_nearby(const EntitySystem& entitySystem, Entity first, Entity second) const;
    
    // returns a sorted vector, it's useful for std::set_difference
    std::vector<Entity> get_nearby(const EntitySystem& entitySystem, Entity entity) const;
    
    void update_position(Entity entity, float old_x, float old_y, float x, float y);
    
  private:
    using key_t = std::tuple<uint16_t, uint16_t>;
    
    std::unordered_map<key_t, std::unordered_set<Entity>, Core::tuple_hash> grid;
};
