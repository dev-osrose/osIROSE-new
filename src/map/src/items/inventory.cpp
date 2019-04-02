#include "items/inventory.h"
#include "entity_system.h"

#include "components/inventory.h"

using namespace RoseCommon;
using namespace Items;

size_t Items::get_first_available_spot(const EntitySystem& entitySystem, RoseCommon::Entity entity) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    size_t res = 0;
    for (const auto item : inv.getInventory()) {
        if (item == entt::null) {
            return res;
        }
        ++res;
    }
    return 0;
}

ReturnValue Items::add_item(EntitySystem& entitySystem, RoseCommon::Entity entity, RoseCommon::Entity item) {
    const size_t pos = get_first_available_spot(entitySystem, entity);
    if (pos == 0) {
        return ReturnValue::NO_SPACE;
    }
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    inv.getInventory()[pos] = item;
    return ReturnValue::OK;
}

RoseCommon::Entity Items::remove_item(EntitySystem& entitySystem, RoseCommon::Entity entity, size_t pos) {
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    auto inventory = inv.getInventory();
    RoseCommon::Entity item = inventory[pos];
    inventory[pos] = entt::null;
    return item;
}

void Items::swap_item(EntitySystem& entitySystem, RoseCommon::Entity entity, size_t pos1, size_t pos2) {
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    auto inventory = inv.getInventory();
    std::swap(inventory[pos1], inventory[pos2]);
}

ReturnValue Items::equip_item(EntitySystem&, RoseCommon::Entity entity, size_t from, size_t to) {
    return ReturnValue::OK;
}

ReturnValue Items::unequip_item(EntitySystem&, RoseCommon::Entity entity, size_t from) {
    return ReturnValue::OK;
}