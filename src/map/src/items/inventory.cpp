#include "items/inventory.h"
#include "entity_system.h"
#include "dataconsts.h"
#include "random.h"

#include "chat/whisper_chat.h"
#include "components/basic_info.h"
#include "components/inventory.h"
#include "components/item.h"
#include "components/lua.h"
#include "components/position.h"
#include "components/owner.h"
#include "itemdb.h"

#include "srv_equip_item.h"
#include "srv_equip_item_ride.h"
#include "srv_set_item.h"
#include "srv_set_money.h"
#include "srv_craft_enhance_reply.h"

#include <limits>

using namespace RoseCommon;
using namespace Items;

namespace {

inline bool is_spot_correct_riding(const EntitySystem& entitySystem, Entity entity, size_t spot) {
    const auto& item = entitySystem.get_component<ItemDef>(entity);
    const RidingItem pos = static_cast<RidingItem>(spot);
    if (pos >= MAX_ITEMS) {
        return true;
    }
    switch (item.subtype / 10 % 10) {
        case RidingSubType::FRAME:
            return pos == RidingItem::BODY;
        case RidingSubType::ENGINE_PART:
            return pos == RidingItem::ENGINE;
        case RidingSubType::WHEELS:
            return pos == RidingItem::LEGS;
        case RidingSubType::ABILITY_PART:
            return pos == RidingItem::OPTION;
        case RidingSubType::CART_WEAP:
            return pos == RidingItem::ARMS;
        default:
            return false;
    }
}

// only for items, not cart/castle gear
inline bool is_spot_correct(const EntitySystem& entitySystem, Entity entity, size_t spot) {
    const auto& item = entitySystem.get_component<ItemDef>(entity);
    const EquippedPosition pos = static_cast<EquippedPosition>(spot);
    if (pos >= EquippedPosition::MAX_EQUIP_ITEMS) {
        return true; // we don't care of the spot if we are not equipping anything
    }
    switch (item.type) {
        case ItemType::ITEM_GOGGLES:
            return pos == EquippedPosition::GOGGLES;
        case ItemType::ITEM_HELMET:
            return pos == EquippedPosition::HELMET;
        case ItemType::ITEM_ARMOR:
            return pos == EquippedPosition::ARMOR;
        case ItemType::ITEM_GAUNTLET:
            return pos == EquippedPosition::GAUNTLET;
        case ItemType::ITEM_BOOTS:
            return pos == EquippedPosition::BOOTS;
        case ItemType::ITEM_BACKPACK:
            return pos == EquippedPosition::BACKPACK;
        case ItemType::ITEM_RING:
            return pos == EquippedPosition::RING;
        case ItemType::ITEM_WEAPON_R:
            return pos == EquippedPosition::WEAPON_R;
        case ItemType::ITEM_WEAPON_L:
            return pos == EquippedPosition::WEAPON_L;
        case ItemType::ITEM_RIDING:
            return is_spot_correct_riding(entitySystem, entity, spot);
        default:
            return false;
    }
}

inline bool is_two_handed(uint16_t subtype) {
    return ((subtype >= ItemSubType::TWOH_SWORD && subtype <= ItemSubType::DUAL_WIELD) && (subtype != ItemSubType::WAND));
}

inline bool is_spot_equipped(size_t spot) {
    return spot < EquippedPosition::MAX_EQUIP_ITEMS;
}

inline size_t get_item_tab(ItemType type) {
    if (type < ItemType::ITEM_CONSUMABLE) return 0;
    else if (type == ItemType::ITEM_CONSUMABLE) return 1;
    else if (type == ItemType::ITEM_ETC || type == ItemType::ITEM_ETC2 || type == ItemType::ITEM_ETC_GEM) return 2;
    else return 3;
    }
}

size_t Items::get_first_available_spot(const EntitySystem& entitySystem, Entity entity, Entity item) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    ItemType type = ItemType::NONE;
    uint16_t id = 0;
    if (item != entt::null) {
        const auto& i = entitySystem.get_component<RoseCommon::ItemDef>(item);
        type = i.type;
        id = i.id;
    }
    size_t itemTab = get_item_tab(type);

    if (itemTab == 1 || itemTab ==2) {
        for(int i = 0; i < RoseCommon::TAB_SIZE; i++) {
            size_t res = EquippedPosition::MAX_EQUIP_ITEMS;
            res += (RoseCommon::TAB_SIZE * itemTab) + i;
            if(inv.items[res] != entt::null) {
                const auto& item = entitySystem.get_component<Component::Item>(inv.items[res]);
                const auto& itemDef = entitySystem.get_component<RoseCommon::ItemDef>(inv.items[res]);
                if (itemDef.type == type && itemDef.id == id && item.count < RoseCommon::MAX_STACK) {
                    return res;
                }
            }
        }
    }

    for (int i = 0; i < RoseCommon::TAB_SIZE; i++) {
        size_t res = EquippedPosition::MAX_EQUIP_ITEMS;
        res += (RoseCommon::TAB_SIZE * itemTab) + i;
        if (inv.items[res] == entt::null) return res;
    }
    return 0;
}

ReturnValue Items::add_item(EntitySystem& entitySystem, Entity entity, Entity item) {
    const size_t pos = get_first_available_spot(entitySystem, entity, item);

    if (pos == 0) {
        return ReturnValue::NO_SPACE;
    }
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    if (inv.items[pos] == entt::null) {
        inv.items[pos] = item;
    } else {
        // add the stack
        auto& i = entitySystem.get_component<Component::Item>(inv.items[pos]);
        auto& it = entitySystem.get_component<Component::Item>(item);
        if (i.count + it.count < RoseCommon::MAX_STACK) {
            // below max stack
            i.count += it.count;
            entitySystem.delete_entity(item);
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
    RoseCommon::Packet::SrvSetItem::IndexAndItem index; index.set_index(pos);
    index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(inv.items[pos]));
    auto packet = RoseCommon::Packet::SrvSetItem::create();
    packet.add_items(index);
    entitySystem.send_to(entity, packet);
    return ReturnValue::OK;
}

Entity Items::remove_item(EntitySystem& entitySystem, Entity entity, size_t pos, uint32_t quantity) {
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    Entity item = inv.items[pos];
    auto& i = entitySystem.get_component<Component::Item>(item);
    const auto& it = entitySystem.get_component<ItemDef>(item);
    if (i.count < quantity) {
        return entt::null;
    }
    if (i.count > quantity) {
        const auto type = it.type;
        const auto id = it.id;
        i.count -= quantity;
        Entity newItem = entitySystem.create_item(type, id, quantity);
        RoseCommon::Packet::SrvSetItem::IndexAndItem index;
        index.set_index(pos);
        index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(item));
        auto packet = RoseCommon::Packet::SrvSetItem::create();
        packet.add_items(index);
        entitySystem.send_to(entity, packet);
        return newItem;
    }
    if (is_spot_equipped(pos)) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(item);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(item)) {
                return entt::null;
            }
        }
    }
    inv.items[pos] = entt::null;
    RoseCommon::Packet::SrvSetItem::IndexAndItem index;
    index.set_index(pos);
    auto packet = RoseCommon::Packet::SrvSetItem::create();
    packet.add_items(index);
    entitySystem.send_to(entity, packet);
    return item;
}

void Items::swap_item(EntitySystem& entitySystem, Entity entity, size_t pos1, size_t pos2) {
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    std::swap(inv.items[pos1], inv.items[pos2]);
}



ReturnValue Items::equip_item(EntitySystem& entitySystem, Entity entity, size_t from, size_t to) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);

    if (from < decltype(inv.getInventory())::offset() || from >= decltype(inv.getInventory())::size()) {
        return ReturnValue::WRONG_INDEX;
    }
    if (to < decltype(inv.getEquipped())::offset() || (to >= decltype(inv.getEquipped())::size() && to < RidingItem::BODY) || to >= MAX_ITEMS) {
        return ReturnValue::WRONG_INDEX;
    }

    const Entity equipped = inv.items[to];
    const Entity to_equip = inv.items[from];
    const auto& item = entitySystem.get_component<RoseCommon::ItemDef>(to_equip);
    if (item.type == ItemType::ITEM_WEAPON_R && (is_two_handed(item.subtype) == true) && (inv.items[EquippedPosition::WEAPON_L] != entt::null))
    {
        if (unequip_item(entitySystem, entity, (size_t)EquippedPosition::WEAPON_L) == ReturnValue::NO_SPACE) return ReturnValue::NO_SPACE;
    }

    if (!is_spot_correct(entitySystem, to_equip, to)) {
        return ReturnValue::REQUIREMENTS_NOT_MET;
    }
    if (equipped != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(equipped);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(entity)) {
                //return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    if (to_equip != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(to_equip);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_equip(entity)) {
                //return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    swap_item(entitySystem, entity, from, to);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    {
        switch (item.type) {
            case ItemType::ITEM_RIDING:
            {
                const auto packet = RoseCommon::Packet::SrvEquipItemRide::create(basicInfo.id, static_cast<RoseCommon::Packet::SrvEquipItemRide::EquippedPositionRide>(to),
                    entitySystem.item_to_equipped<RoseCommon::Packet::SrvEquipItemRide>(inv.items[to]), static_cast<size_t>(800));
                entitySystem.send_map(packet);
                break;
            }
            default:
            {
                const auto packet = RoseCommon::Packet::SrvEquipItem::create(basicInfo.id, to,
                    entitySystem.item_to_equipped<RoseCommon::Packet::SrvEquipItem>(inv.items[to]));
                entitySystem.send_map(packet);
            }
        }
    }

    RoseCommon::Packet::SrvSetItem::IndexAndItem index; index.set_index(to);
    index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(to_equip));
    auto packet = RoseCommon::Packet::SrvSetItem::create();
    packet.add_items(index);
    index.set_index(from);
    index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(equipped));
    packet.add_items(index);
    entitySystem.send_to(entity, packet);
    return ReturnValue::OK;
}

void enhance_gemming(EntitySystem & entitySystem, Entity entity, const RoseCommon::Packet::CliCraftEnhanceReq::CraftEnhancementData data) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->debug("Item to get gemmed is {} gem slot is {}", data.get_gem_target(), data.get_gem_source());
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    auto& gem = entitySystem.get_component<Component::Item>(inv.items[data.get_gem_source()]);
    const auto& gemDef = entitySystem.get_component<RoseCommon::ItemDef>(inv.items[data.get_gem_source()]);
    auto& item = entitySystem.get_component<Component::Item>(inv.items[data.get_gem_target()]);
    auto result_packet = RoseCommon::Packet::SrvCraftEnhanceReply::create(RoseCommon::Packet::SrvCraftEnhanceReply::CraftEnhancementResult::GEM_SUCCESS);
    if(item.hasSocket) {
        if (item.gemOpt == 0) {
            if (gem.count > 0 && gemDef.type == ItemType::ITEM_ETC_GEM) {
                // TODO: Success rate random :>
                remove_item(entitySystem, entity, data.get_gem_source(), 1);
                item.gemOpt = gemDef.id;
                result_packet.set_result(RoseCommon::Packet::SrvCraftEnhanceReply::CraftEnhancementResult::GEM_SUCCESS);

                RoseCommon::Packet::SrvCraftEnhanceReply::IndexAndItem index;
                index.set_index(data.get_gem_target());
                index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvCraftEnhanceReply>(inv.items[data.get_gem_target()]));
                result_packet.add_items(index);
                index.set_index(data.get_gem_source());
                index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvCraftEnhanceReply>(inv.items[data.get_gem_source()]));
                result_packet.add_items(index);
                entitySystem.send_to(entity, result_packet);
            } else {
                logger->debug("No gem/Not a gem");
                Chat::send_whisper(entitySystem, entity, "Insufficient amount of gems/Not a gem");
            }
        } else {
            result_packet.set_result(RoseCommon::Packet::SrvCraftEnhanceReply::CraftEnhancementResult::GEM_SOCKET_FULL);
            logger->debug("Socket is full");
        }
    } else {
        result_packet.set_result(RoseCommon::Packet::SrvCraftEnhanceReply::CraftEnhancementResult::GEM_NO_SOCKET);
        logger->debug("No socket");
    }      
    entitySystem.send_to(entity, result_packet);
}


ReturnValue Items::unequip_item(EntitySystem& entitySystem, Entity entity, size_t from) {
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    const size_t to = get_first_available_spot(entitySystem, entity, inv.items[from]);

    if (to == 0) {
        return ReturnValue::NO_SPACE;
    }

    if (from < decltype(inv.getEquipped())::offset() || (from >= decltype(inv.getEquipped())::size() && from < RidingItem::BODY) || from >= MAX_ITEMS) {
        return ReturnValue::WRONG_INDEX;
    }

    const Entity equipped = inv.items[from];
    if (equipped != entt::null) {
        const auto& lua = entitySystem.get_component<Component::ItemLua>(equipped);
        if (const auto tmp = lua.api.lock(); tmp) {
            if (!tmp->on_unequip(entity)) {
                //return ReturnValue::REQUIREMENTS_NOT_MET;
            }
        }
    }
    swap_item(entitySystem, entity, from, to);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& item = entitySystem.get_component<RoseCommon::ItemDef>(equipped);
    {
        switch (item.type) {
            case ItemType::ITEM_RIDING:
            {
                const auto packet = RoseCommon::Packet::SrvEquipItemRide::create(basicInfo.id, static_cast<RoseCommon::Packet::SrvEquipItemRide::EquippedPositionRide>(from), {}, static_cast<size_t>(500));      
                entitySystem.send_nearby(entity, packet);
                break;
            }
            default:
            {
                const auto packet = RoseCommon::Packet::SrvEquipItem::create(basicInfo.id, from, {});
                entitySystem.send_nearby(entity, packet);
            }
        }
    }

    RoseCommon::Packet::SrvSetItem::IndexAndItem index;
    index.set_index(to);
    index.set_item(entitySystem.item_to_item<RoseCommon::Packet::SrvSetItem>(equipped));
    auto packet = RoseCommon::Packet::SrvSetItem::create();
    packet.add_items(index);
    index.set_index(from);
    index.set_item({});
    packet.add_items(index);
    entitySystem.send_to(entity, packet);
    return ReturnValue::OK;
}

void Items::drop_item(EntitySystem& entitySystem, Entity item, float x, float y, Entity owner) {
    Component::BasicInfo bi;
    bi.id = entitySystem.get_free_id();
    if (owner != entt::null) {
        const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(owner);
        bi.teamId = basicInfo.teamId;
        auto& Cowner = entitySystem.add_component<Component::Owner>(item);
        Cowner.owner = owner;
    } else {
        bi.teamId = bi.id;
    }
    entitySystem.add_component(item, std::move(bi));

    entitySystem.update_position(item, x, y);

    entitySystem.add_timer(1min, [item](EntitySystem& entitySystem) {
        if (!entitySystem.is_valid(item)) {
            return;
        }
        if (entitySystem.has_component<Component::Owner>(item)) {
            entitySystem.remove_component<Component::Owner>(item);
            auto& basic = entitySystem.get_component<Component::BasicInfo>(item);
            basic.teamId = basic.id;
        }
    });

    entitySystem.add_timer(2min, [item](EntitySystem& entitySystem) {
        if (!entitySystem.is_valid(item)) {
            return;
        }
        if (entitySystem.has_component<Component::Position>(item)) {
            entitySystem.delete_entity(item);
        }
    });
}

void Items::pickup_item(EntitySystem& entitySystem, Entity entity, Entity item) {
    const float x = entitySystem.get_component<Component::Position>(item).x;
    const float y = entitySystem.get_component<Component::Position>(item).y;
    const auto* owner = entitySystem.try_get_component<Component::Owner>(item);
    if (owner && owner->owner != entity) {
        return;
    }
    entitySystem.remove_component<Component::Position>(item);
    entitySystem.remove_component<Component::BasicInfo>(item);
    entitySystem.remove_component<Component::Owner>(item);
    if(entitySystem.has_component<Component::Item>(item)) {
        const auto& i = entitySystem.get_component<Component::Item>(item);
        if(i.is_zuly == true) {
            Items::add_zuly(entitySystem, entity, i.count);
            entitySystem.delete_entity(item);
        }
        else if (Items::add_item(entitySystem, entity, item) != ReturnValue::OK) {
            const Entity o = owner ? owner->owner : entt::null;
            Items::drop_item(entitySystem, item, x, y, o);
        }
    }
}

bool Items::add_zuly(EntitySystem& entitySystem, Entity entity, int64_t zuly) {
    auto& inv = entitySystem.get_component<Component::Inventory>(entity);
    if (zuly < 0 && inv.zuly + zuly < 0) {
        return false;
    } else if (zuly < 0) {
        inv.zuly += zuly;
    } else {
        inv.zuly = static_cast<int64_t>(std::min(static_cast<uint64_t>(inv.zuly) + static_cast<uint64_t>(zuly),
                                        static_cast<uint64_t>(std::numeric_limits<int64_t>::max())));
    }
    entitySystem.send_to(entity,
                         RoseCommon::Packet::SrvSetMoney::create(inv.zuly));
    return true;
}

void Items::equip_item_ride_packet(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliEquipItemRide& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("equip_item_ride_packet()");
    logger->trace("from {} to {}", packet.get_index(), packet.get_slot() + 135);
    const auto from = packet.get_index();
    const auto to = packet.get_slot() + 135;
    const auto res = from == 0 ?
        unequip_item(entitySystem, entity, to):
        equip_item(entitySystem, entity, from, to);
    (void) res;

}

void Items::craft_enhance_packet(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliCraftEnhanceReq& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->warn("craft_enhance_packet");
    //RoseCommon::Packet::CliCraftEnhanceReq::CraftEnhancementData data;
    RoseCommon::Packet::CliCraftEnhanceReq::CraftEnhancementType type;
    //data = packet.get_data();
    type = packet.get_enhancement();
    logger->warn("Enhance type is {}", type);
    
    switch (type) {
        case RoseCommon::Packet::CliCraftEnhanceReq::CraftEnhancementType::GEM:
            enhance_gemming(entitySystem, entity, packet.get_data());
    }

}


void Items::equip_item_packet(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliEquipItem& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("equip_item_packet()");
    logger->trace("from {} to {}", packet.get_slotFrom(), packet.get_slotTo());
    const auto from = packet.get_slotFrom();
    const auto to = packet.get_slotTo();
    const auto res = from == 0 ? // we want to unequip something, 0 being a "fake" no-item flag
        unequip_item(entitySystem, entity, to):
        equip_item(entitySystem, entity, from, to);
    (void) res;
}

void Items::drop_item_packet(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliDropItem& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("equip_item_packet()");
    logger->trace("drop {}x{}", packet.get_index(), packet.get_quantity());
    const auto index = packet.get_index();
    const auto quantity = packet.get_quantity();
    const auto& inv = entitySystem.get_component<Component::Inventory>(entity);

    if (index > inv.items.size()) {
        logger->warn("wrong index {} for item drop, client {}", index, entity);
        return;
    }
    Entity item = entt::null;
    if (index == 0) { // we drop zulies
        if (add_zuly(entitySystem, entity, -static_cast<int64_t>(packet.get_quantity()))) {
            item = entitySystem.create_zuly(packet.get_quantity());
        } else {
            return; // we don't have enough zuly to remove
        }
    } else {
        item = remove_item(entitySystem, entity, index, quantity);
    }
    const auto& pos = entitySystem.get_component<Component::Position>(entity);
    const auto [x, y] = Core::Random::getInstance().random_in_circle(pos.x, pos.y, DROP_RANGE);
    drop_item(entitySystem, item, x, y, entity);
}
