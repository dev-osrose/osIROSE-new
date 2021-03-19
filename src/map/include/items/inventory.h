#pragma once

#include "entity.h"
#include <optional>

#include "cli_equip_item.h"
#include "cli_drop_item.h"

class EntitySystem;

namespace Items {
enum class ReturnValue {
    OK,
    NO_SPACE,
    REQUIREMENTS_NOT_MET,
    WRONG_INDEX,
};

// returns the first available spot in the inventory or 0 if there is no more space
size_t get_first_available_spot(const EntitySystem& entitySystem, Entity entity, Entity item = entt::null);
ReturnValue add_item(EntitySystem&, Entity entity, Entity item);
// will be entt::null if there is no item at that position
Entity remove_item(EntitySystem&, Entity entity, size_t pos, uint32_t quantity);
void swap_item(EntitySystem&, Entity entity, size_t pos1, size_t pos2);
ReturnValue equip_item(EntitySystem&, Entity, size_t from, size_t to);
// the unequipped item will be put in the first available spot
ReturnValue unequip_item(EntitySystem&, Entity, size_t from);
void drop_item(EntitySystem&, Entity, float x, float y, Entity owner = entt::null);
void pickup_item(EntitySystem&, Entity entity, Entity item);

// you can add a negative amount (effectively removing zulies). Returns false if there isn't enough zuly in the inventory to remove
bool add_zuly(EntitySystem&, Entity, int64_t zuly);

void equip_item_packet(EntitySystem&, Entity, const RoseCommon::Packet::CliEquipItem&);
void drop_item_packet(EntitySystem&, Entity, const RoseCommon::Packet::CliDropItem&);
}
