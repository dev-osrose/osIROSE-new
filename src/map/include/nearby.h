#pragma once

#include "hash.h"
#include "dataconsts.h"

#include <unordered_map>
#include <vector>
#include <tuple>

class Nearby {
  public:
    Nearby();
    
    void add_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity);
    void remove_entity(RoseCommon::Registry& registry, RoseCommon::Entity entity);
    
    bool is_nearby(RoseCommon::Entity first, RoseCommon::Entity second) const;
    
    // returns a sorted vector, it's useful for std::set_difference
    std::vector<RoseCommon::Entity> get_nearby(RoseCommon::Entity entity) const;
    
    void update_position(RoseCommon::Entity entity, float old_x, float old_y, float x, float y);
    
  private:
    using key_t = std::tuple<uint16_t, uint16_t>;
    
    std::unordered_map<key_t, std::vector<Entity>, Core::tuple_hash> grid;
  
    const EntitySystem& entitySystem;
};
