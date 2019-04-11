#include "items/inventory.h"
#include "entity_system.h"
#include "dataconsts.h"

#include "components/basic_info.h"
#include "components/inventory.h"
#include "components/item.h"
#include "components/lua.h"
#include "itemdb.h"

#include "srv_equip_item.h"
#include "srv_set_item.h"

using namespace RoseCommon;
using namespace Items;

size_t Items::get_first_available_spot(const EntitySystem& entitySystem, RoseCommon::Entity entity, RoseCommon::Entity item) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    size_t res = 0;
    bool stackable = false;
    uint8_t type = 0;
    uint16_t id = 0;
    if (item != entt::null) {
        const auto& i = entitySystem.get_component<RoseCommon::ItemDef>(item);
        stackable = i.is_stackable;
        type = i.type;
        id = i.id;
    }

    for (const auto item : inv.getInventory()) {
        if (item == entt::null) {
            return res;
        } else if (stackable) {
            const auto& i = entitySystem.get_component<RoseCommon::ItemDef>(item);
            const auto& it = entitySystem.get_component<Component::Item>(item);
            if (i.type == type && i.id == id && it.count < RoseCommon::MAX_STACK) {
                return res;
            }
        }
        ++res;
    }
    return 0;
}

ReturnValue Items::add_item(EntitySystem& entitySystem, RoseCommon::Entity entity, RoseCommon::Entity item) {
    const size_t pos = get_first_available_spot(entitySystem, entity, item);
    if (pos == 0) {
        return ReturnValue::NO_SPACE;
    }
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    if (inv.getInventory()[pos] == entt::null) {
        inv.getInventory()[pos] = item;
    } else {
        // add the stack
        auto& i = entitySystem.get_component<Component::Item>(inv.getInventory()[pos]);
        auto& it = entitySystem.get_component<Component::Item>(item);
        if (i.count + it.count < RoseCommon::MAX_STACK) {
            // below max stack
            i.count += it.count;
        } else {
            // split the stack in two or more
            const uint32_t stack_tmp1 = i.count;
            const uint32_t stack_tmp2 = it.count;
            it.count -= RoseCommon::MAX_STACK - i.count;
            i.count = RoseCommon::MAX_STACK;
            if (add_item(entitySystem, entity, item) == ReturnValue::NO_SPACE) {
                it.count = stack_tmp2;
                i.count = stack_tmp1;
                return ReturnValue::NO_SPACE;
            }
        }
    }
    RoseCommon::Packet::SrvSetItem::IndexAndItem index;
    index.set_index(pos);
    index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(item));
    auto packet = RoseCommon::Packet::SrvSetItem::create();
    packet.add_items(index);
    entitySystem.send_to(entity, packet);
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
    std::swap(inv.items[pos1], inv.items[pos2]);
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
    logger->trace("equip_item_packet()");
    logger->trace("from {} to {}", packet.get_slotFrom(), packet.get_slotTo());
    const auto from = packet.get_slotFrom();
    const auto to = packet.get_slotTo();
    if (to == 0) { // we want to unequip something, 0 being a "fake" no-item flag
        unequip_item(entitySystem, entity, from);
    } else {
        equip_item(entitySystem, entity, from, to);
    }
}
