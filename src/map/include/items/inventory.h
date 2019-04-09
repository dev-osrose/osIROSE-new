#pragma once

#include "dataconsts.h"
#include <optional>

#include "cli_equip_item.h"

class EntitySystem;

namespace Items {
enum class ReturnValue {
    OK,
    NO_SPACE,
    REQUIREMENTS_NOT_MET,
};

// returns the first available spot in the inventory or 0 if there is no more space
size_t get_first_available_spot(const EntitySystem& entitySystem, RoseCommon::Entity entity);
ReturnValue add_item(EntitySystem&, RoseCommon::Entity entity, RoseCommon::Entity item);
// will be entt::null if there is no item at that position
RoseCommon::Entity remove_item(EntitySystem&, RoseCommon::Entity entity, size_t pos);
void swap_item(EntitySystem&, RoseCommon::Entity entity, size_t pos1, size_t pos2);
ReturnValue equip_item(EntitySystem&, RoseCommon::Entity, size_t from, size_t to);
// the unequipped item will be put in the first available spot
ReturnValue unequip_item(EntitySystem&, RoseCommon::Entity, size_t from);


// TODO: drop, pickup
void equip_item_packet(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliEquipItem&);
}
