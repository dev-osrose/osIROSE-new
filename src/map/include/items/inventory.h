#pragma once

#include "dataconsts.h"
#include <optional>

#include "cli_equip_item.h"
#include "cli_drop_item.h"
#include "cli_equip_projectile.h"

class EntitySystem;

namespace Items {
enum class ReturnValue {
    OK,
    NO_SPACE,
    REQUIREMENTS_NOT_MET,
    WRONG_INDEX,
};

// returns the first available spot in the inventory or 0 if there is no more space
size_t get_first_available_spot(const EntitySystem& entitySystem, RoseCommon::Entity entity, RoseCommon::Entity item = entt::null);
uint8_t has_bullets(const EntitySystem& entitySystem, RoseCommon::Entity entity);
bool is_bullet_weapon(const EntitySystem& entitySystem, RoseCommon::Entity entity);
ReturnValue add_item(EntitySystem&, RoseCommon::Entity entity, RoseCommon::Entity item);
// will be entt::null if there is no item at that position
RoseCommon::Entity remove_item(EntitySystem&, RoseCommon::Entity entity, size_t pos, uint32_t quantity);
void swap_item(EntitySystem&, RoseCommon::Entity entity, size_t pos1, size_t pos2);
void set_projectile(EntitySystem&, RoseCommon::Entity entity, const RoseCommon::Packet::CliEquipProjectile&);
ReturnValue equip_item(EntitySystem&, RoseCommon::Entity, size_t from, size_t to);
// the unequipped item will be put in the first available spot
ReturnValue unequip_item(EntitySystem&, RoseCommon::Entity, size_t from);
void drop_item(EntitySystem&, RoseCommon::Entity, float x, float y, RoseCommon::Entity owner = entt::null);
void pickup_item(EntitySystem&, RoseCommon::Entity entity, RoseCommon::Entity item);

// you can add a negative amount (effectively removing zulies). Returns false if there isn't enough zuly in the inventory to remove
bool add_zuly(EntitySystem&, RoseCommon::Entity, int64_t zuly);

void equip_item_packet(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliEquipItem&);
void drop_item_packet(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliDropItem&);
}
