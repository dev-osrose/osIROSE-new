#pragma once

#include "entity_system.h"
#include "components/item.h"
#include "itemdb.h" // ItemDef

namespace Serializers {

template <typename T>
auto item_to_equipped(const EntitySystem&, Entity entity) const;
template <typename T>
auto item_to_header(const EntitySystem&, Entity entity) const;
template <typename T>
auto item_to_data(const EntitySystem&, Entity entity) const;
template <typename T>
auto item_to_item(const EntitySystem&, Entity entity) const;
}


// implementation
template <typename T>
auto Serializers::item_to_equipped(const EntitySystem& es, Entity entity) const {
    if (entity == entt::null) {
        return typename T::EquippedItem{};
    }
    const auto& item = es.get_component<Component::Item>(entity);
    const auto& data = es.get_component<RoseCommon::ItemDef>(entity);

    typename T::EquippedItem itemPacket;
    itemPacket.set_id(data.id);
    itemPacket.set_gem_opt(item.gemOpt);
    itemPacket.set_socket(item.hasSocket);
    itemPacket.set_grade(item.refine);
    return itemPacket;
}
    
template <typename T>
auto Serializers::item_to_header(const EntitySystem& es, Entity entity) const {
    if (entity == entt::null) {
        return typename T::Header{};
    }
    const auto& item = es.get_component<Component::Item>(entity);
    const auto* data = es.try_get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Header header;
    header.set_isCreated(item.isCreated);
    if (data) {
        header.set_id(data->id);
        header.set_type(RoseCommon::to_underlying(data->type));
    } else {
        header.set_id(0);
        header.set_type(RoseCommon::to_underlying(RoseCommon::ItemType::ZULY));
    }
    return header;
}
    
template <typename T>
auto Serializers::item_to_data(const EntitySystem& es, Entity entity) const {
    if (entity == entt::null) {
        return typename T::Data{};
    }
    const auto& item = es.get_component<Component::Item>(entity);
    const auto* itemDef = es.try_get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Data data;
    if (!itemDef || itemDef->is_stackable) {
        data.set_count(item.count);
    } else {
        data.set_refine(item.refine);
        data.set_isAppraised(item.isAppraised);
        data.set_hasSocket(item.hasSocket);
        data.set_life(item.life);
        data.set_durability(item.durability);
        data.set_gem_opt(item.gemOpt);
    }
    return data;
}
    
template <typename T>
auto Serializers::item_to_item(const EntitySystem& es, Entity entity) const {
    typename T::Item item{};
    if (entity == entt::null) {
        return item;
    }
    item.set_header(item_to_header<T>(es, entity));
    item.set_data(item_to_data<T>(es, entity));
    return item;
}
