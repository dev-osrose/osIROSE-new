#include "items/inventory.h"
#include "entity_system.h"

#include "components/basicInfo.h"
#include "components/inventory.h"
#include "components/lua.h"

#include "srv_equip_item.h"

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

ReturnValue Items::equip_item(EntitySystem& entitySystem, RoseCommon::Entity entity, size_t from, size_t to) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);

    const RoseCommon::Entity equipped = inv.getEquipped()[to];
    if (equipped != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(equipped);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(entity)) {
                return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    const RoseCommon::Entity to_equip = inv.getInventory()[from];
    if (from != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(to_equip);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(entity)) {
                return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    swap_item(entitySystem, entity, from, to);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto packet = RoseCommon::Packet::SrvEquipItem::create(basicInfo.id, to,
            entitySystem.item_to_equipped<RoseCommon::Packet::SrvEquipItem>(inv.getInventory()[to]));
    entitySystem.send_nearby(entity, packet);
    return ReturnValue::OK;
}

ReturnValue Items::unequip_item(EntitySystem& entitySystem, RoseCommon::Entity entity, size_t from) {
    const size_t to = get_first_available_spot(entitySystem, entity);
    if (to == 0) {
        return ReturnValue::NO_SPACE;
    }

    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);

    const RoseCommon::Entity equipped = inv.getEquipped()[from];
    if (equipped != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(equipped);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(entity)) {
                return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    swap_item(entitySystem, entity, from, to);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto packet = RoseCommon::Packet::SrvEquipItem::create(basicInfo.id, to,
            entitySystem.item_to_equipped<RoseCommon::Packet::SrvEquipItem>(inv.getInventory()[from]));
    entitySystem.send_nearby(entity, packet);
    return ReturnValue::OK;
}

void Items::equip_item_packet(EntitySystem& entitySystem, RoseCommon::Entity entity, const RoseCommon::Packet::CliEquipItem& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("equip_item_packet");
    logger->trace("from {} to {}", packet.get_slotFrom(), packet.get_slotTo());
    const auto from = packet.get_slotFrom() - decltype(std::declval<Component::Inventory>().getInventory())::offset();
    const auto to = packet.get_slotTo() == 0 ? 0 : packet.get_slotTo() - decltype(std::declval<Component::Inventory>().getEquipped())::offset();
    logger->trace("translated from {} to {}", from, to);
    if (to == 0) { // we want to unequip something, 0 being a "fake" no-item flag
        unequip_item(entitySystem, entity, from);
    } else {
        equip_item(entitySystem, entity, from, to);
    }
}
