#pragma once

#include "array_wrapper.h"
#include "entity.h"
#include "dataconsts.h"
#include <array>
#include <entt.hpp>

#include <type_traits>

namespace Component {
  struct Inventory {
    Inventory() {
      items.fill(entt::null);
    }
    int64_t zuly;
    std::array<Entity, RoseCommon::MAX_ITEMS> items;

    template <typename T, size_t N, size_t L>
    using base_wrapper = Core::array_wrapper<T, N, L>;
    template <size_t N, size_t L>
    using wrapper = base_wrapper<std::array<Entity, RoseCommon::MAX_ITEMS>, N, L>;
    template <size_t N, size_t L>
    using const_wrapper = base_wrapper<std::add_const_t<std::array<Entity, RoseCommon::MAX_ITEMS>>, N, L>;

    inline Entity& goggles() {
      return items[RoseCommon::EquippedPosition::GOGGLES];
    }

    inline Entity& helmet() {
      return items[RoseCommon::EquippedPosition::HELMET];
    }

    inline Entity& armor() {
      return items[RoseCommon::EquippedPosition::ARMOR];
    }

    inline Entity& backpack() {
      return items[RoseCommon::EquippedPosition::BACKPACK];
    }

    inline Entity& gauntlet() {
      return items[RoseCommon::EquippedPosition::GAUNTLET];
    }

    inline Entity& boots() {
      return items[RoseCommon::EquippedPosition::BOOTS];
    }

    inline Entity& weapon_r() {
      return items[RoseCommon::EquippedPosition::WEAPON_R];
    }

    inline Entity& weapon_l() {
      return items[RoseCommon::EquippedPosition::WEAPON_L];
    }

    inline Entity& necklace() {
      return items[RoseCommon::EquippedPosition::NECKLACE];
    }

    inline Entity& ring() {
      return items[RoseCommon::EquippedPosition::RING];
    }

    inline Entity& earing() {
      return items[RoseCommon::EquippedPosition::EARING];
    }

    constexpr wrapper<1, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> getEquipped() {
      return { items };
    }

    constexpr wrapper<1, RoseCommon::MAX_VISIBLE_ITEMS> getVisible() {
      return { items };
    }

    constexpr wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY, RoseCommon::MAX_BULLET_TYPES> getBullets() {
      return { items };
    }

    constexpr wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS, RoseCommon::MAX_INVENTORY> getInventory() {
      return { items };
    }

    constexpr wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY + RoseCommon::MAX_BULLET_TYPES, RoseCommon::MAX_RIDING_ITEMS> getRidingItems() {
      return { items };
    }

    constexpr const_wrapper<1, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> getEquipped() const {
      return { items };
    }

    constexpr const_wrapper<1, RoseCommon::MAX_VISIBLE_ITEMS> getVisible() const {
      return { items };
    }

    constexpr const_wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS, RoseCommon::MAX_INVENTORY> getInventory() const {
      return { items };
    }

    constexpr const_wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY, RoseCommon::MAX_BULLET_TYPES> getBullets() const {
      return { items };
    }

    constexpr const_wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY + RoseCommon::MAX_BULLET_TYPES, RoseCommon::MAX_RIDING_ITEMS> getRidingItems() const {
      return { items };
    }
  };
}
