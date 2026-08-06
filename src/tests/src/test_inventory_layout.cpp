#include "gtest/gtest.h"

#include "components/inventory.h"

#include <set>

// Component::Inventory is one flat array of 140 slots that the client indexes
// directly, carved into regions by the get*() slice views. The slot numbers are
// wire format, so an off-by-one here silently moves someone's gear.
//
// Expected carve-up, derived from dataconsts.h / enums.h:
//
//   slot 0          unused
//   slots 1..11     equipment      (GOGGLES=1 .. EARING=11)
//   slots 12..131   inventory      (MAX_INVENTORY = 120)
//   slots 132..134  bullets        (MAX_BULLET_TYPES = 3)
//   slots 135..139  riding items   (BODY=135 .. ARMS=139)

using RoseCommon::EquippedPosition;
using RoseCommon::RidingItem;

namespace {
// The slice views expose their bounds statically, so we can compare regions
// without iterating.
template <typename W>
constexpr std::pair<size_t, size_t> bounds(const W&) {
  return {W::offset(), W::offset() + W::size()};
}
}  // namespace

TEST(InventoryLayout, ConstantsMatchTheWireFormat) {
  EXPECT_EQ(140u, RoseCommon::MAX_ITEMS);
  EXPECT_EQ(120u, RoseCommon::MAX_INVENTORY);
  EXPECT_EQ(12u, EquippedPosition::MAX_EQUIP_ITEMS);
  EXPECT_EQ(3u, RoseCommon::BulletType::MAX_BULLET_TYPES);
  EXPECT_EQ(5u, RidingItem::MAX_RIDING_ITEMS);
  EXPECT_EQ(11u, EquippedPosition::EARING);  // last real equipment slot
}

TEST(InventoryLayout, DefaultConstructedIsAllNull) {
  Component::Inventory inv;
  for (const auto& slot : inv.items) {
    // Compared as a bool: gtest cannot print entt::null_t.
    EXPECT_TRUE(slot == entt::null);
  }
  EXPECT_EQ(0, inv.zuly);
}

TEST(InventoryLayout, NamedAccessorsAreAllDistinctSlots) {
  Component::Inventory inv;

  // Write a unique value through each accessor, then confirm none of them
  // aliased by counting the distinct values that landed in the array.
  Entity* const accessors[] = {
      &inv.goggles(), &inv.helmet(),   &inv.armor(),    &inv.backpack(),
      &inv.gauntlet(), &inv.boots(),   &inv.weapon_r(), &inv.weapon_l(),
      &inv.necklace(), &inv.ring(),    &inv.earing(),
      &inv.riding_body(), &inv.riding_engine(), &inv.riding_legs(),
      &inv.riding_option(), &inv.riding_arms(),
  };

  std::set<Entity*> distinct(std::begin(accessors), std::end(accessors));
  EXPECT_EQ(std::size(accessors), distinct.size())
      << "two named accessors resolve to the same slot";
}

TEST(InventoryLayout, EquipmentAccessorsLandInTheEquipmentRegion) {
  Component::Inventory inv;
  const Entity* base = inv.items.data();

  EXPECT_EQ(1, &inv.goggles() - base);
  EXPECT_EQ(2, &inv.helmet() - base);
  EXPECT_EQ(7, &inv.weapon_r() - base);
  EXPECT_EQ(8, &inv.weapon_l() - base);
  EXPECT_EQ(11, &inv.earing() - base);
}

TEST(InventoryLayout, RidingAccessorsLandInTheRidingRegion) {
  Component::Inventory inv;
  const Entity* base = inv.items.data();

  EXPECT_EQ(135, &inv.riding_body() - base);
  EXPECT_EQ(136, &inv.riding_engine() - base);
  EXPECT_EQ(137, &inv.riding_legs() - base);
  EXPECT_EQ(138, &inv.riding_option() - base);
  EXPECT_EQ(139, &inv.riding_arms() - base);
}

TEST(InventoryLayout, EverySliceStaysInsideTheArray) {
  Component::Inventory inv;

  for (auto [begin, end] : {bounds(inv.getEquipped()), bounds(inv.getVisible()),
                            bounds(inv.getInventory()), bounds(inv.getBullets()),
                            bounds(inv.getRidingItems())}) {
    EXPECT_LE(end, RoseCommon::MAX_ITEMS) << "slice runs past the end of items[]";
    EXPECT_LT(begin, end) << "empty or inverted slice";
  }
}

TEST(InventoryLayout, InventoryBulletsAndRidingArePackedAndDisjoint) {
  Component::Inventory inv;

  const auto inventory = bounds(inv.getInventory());
  const auto bullets = bounds(inv.getBullets());
  const auto riding = bounds(inv.getRidingItems());

  EXPECT_EQ(std::make_pair(size_t{12}, size_t{132}), inventory);
  EXPECT_EQ(std::make_pair(size_t{132}, size_t{135}), bullets);
  EXPECT_EQ(std::make_pair(size_t{135}, size_t{140}), riding);

  // Back to back with no gaps and no overlap.
  EXPECT_EQ(inventory.second, bullets.first);
  EXPECT_EQ(bullets.second, riding.first);
  EXPECT_EQ(riding.second, RoseCommon::MAX_ITEMS);

  EXPECT_EQ(RoseCommon::FIRST_BULLET_SLOT, bullets.first);
  EXPECT_EQ(RidingItem::BODY, riding.first);
}

// getVisible is deliberately a prefix of getEquipped: the first 8 equipment
// slots are the ones other players can see.
TEST(InventoryLayout, VisibleIsAPrefixOfEquipped) {
  Component::Inventory inv;

  const auto equipped = bounds(inv.getEquipped());
  const auto visible = bounds(inv.getVisible());

  EXPECT_EQ(equipped.first, visible.first);
  EXPECT_LE(visible.second, equipped.second);
  EXPECT_EQ(RoseCommon::MAX_VISIBLE_ITEMS, inv.getVisible().size());
}

// BUG, pinned as current behavior rather than fixed here.
//
// getEquipped() is wrapper<1, MAX_EQUIP_ITEMS>, i.e. offset 1 length 12, so it
// spans slots 1..12. But MAX_EQUIP_ITEMS is the enum's count sentinel and the
// last real equipment slot is EARING == 11; slot 12 is the *first inventory
// slot*, which is exactly where getInventory() begins.
//
// So iterating getEquipped() walks one slot into the inventory. The length
// should be MAX_EQUIP_ITEMS - 1. Changing it is a wire-format-visible fix and
// is deliberately out of scope for this test pass — this test documents the
// overlap so the fix has something to flip.
TEST(InventoryLayout, EquippedCurrentlyOverlapsInventoryByOneSlot) {
  Component::Inventory inv;

  const auto equipped = bounds(inv.getEquipped());
  const auto inventory = bounds(inv.getInventory());

  EXPECT_EQ(std::make_pair(size_t{1}, size_t{13}), equipped);
  EXPECT_EQ(size_t{12}, inventory.first);

  // The overlap: equipped's last slot is inventory's first slot.
  EXPECT_GT(equipped.second, inventory.first);
  EXPECT_EQ(1u, equipped.second - inventory.first) << "overlap changed size";

  // Concretely: writing through the last equipped slot is observable as the
  // first inventory slot.
  auto equipped_view = inv.getEquipped();
  auto it = Core::begin(equipped_view);
  std::advance(it, EquippedPosition::MAX_EQUIP_ITEMS - 1);  // last equipped slot
  *it = static_cast<Entity>(42);

  EXPECT_EQ(static_cast<Entity>(42), *Core::begin(inv.getInventory()));
  EXPECT_EQ(static_cast<Entity>(42), inv.items[12]);

  // ...and that slot, index 12, is past the last real equipment slot,
  // EARING == 11.
  const size_t last_equipped_slot = equipped.second - 1;
  EXPECT_EQ(size_t{12}, last_equipped_slot);
  EXPECT_GT(last_equipped_slot, size_t{EquippedPosition::EARING});
}
