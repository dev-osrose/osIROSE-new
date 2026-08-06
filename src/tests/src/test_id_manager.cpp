#include "gtest/gtest.h"

#include "id_manager.h"

#include <set>

// IdManager hands out the entity ids the map server puts on the wire. Ids must
// be unique among live entities, and must come back into circulation when an
// entity leaves, or a long-running map eventually exhausts the 16 bit space.

TEST(IdManager, StartsAtOne) {
  IdManager ids;
  EXPECT_EQ(1, ids.get_free_id());
}

TEST(IdManager, AllocatesSequentially) {
  IdManager ids;
  for (uint16_t expected = 1; expected <= 100; ++expected) {
    EXPECT_EQ(expected, ids.get_free_id());
  }
}

TEST(IdManager, FreshIdsAreUnique) {
  IdManager ids;
  std::set<uint16_t> seen;
  for (int i = 0; i < 1000; ++i) {
    EXPECT_TRUE(seen.insert(ids.get_free_id()).second);
  }
  EXPECT_EQ(1000u, seen.size());
}

TEST(IdManager, ReleasedIdIsHandedOutAgain) {
  IdManager ids;
  const uint16_t first = ids.get_free_id();
  ids.get_free_id();
  ids.get_free_id();

  ids.release_id(first);
  EXPECT_EQ(first, ids.get_free_id());
}

TEST(IdManager, ReleasedIdsArePreferredOverFreshOnes) {
  IdManager ids;
  for (int i = 0; i < 10; ++i) ids.get_free_id();  // 1..10, next fresh would be 11

  ids.release_id(4);
  EXPECT_EQ(4, ids.get_free_id());
  // Pool is empty again, so we go back to fresh ids.
  EXPECT_EQ(11, ids.get_free_id());
}

TEST(IdManager, DrainsTheFreePoolBeforeAllocatingFresh) {
  IdManager ids;
  for (int i = 0; i < 5; ++i) ids.get_free_id();  // 1..5

  ids.release_id(2);
  ids.release_id(3);

  std::set<uint16_t> recycled{ids.get_free_id(), ids.get_free_id()};
  EXPECT_EQ((std::set<uint16_t>{2, 3}), recycled);
  EXPECT_EQ(6, ids.get_free_id());
}

TEST(IdManager, DoubleReleaseIsIdempotent) {
  IdManager ids;
  for (int i = 0; i < 3; ++i) ids.get_free_id();  // 1..3

  ids.release_id(2);
  ids.release_id(2);

  // The free pool is a set, so 2 comes back exactly once and is not
  // handed to two different entities.
  EXPECT_EQ(2, ids.get_free_id());
  EXPECT_EQ(4, ids.get_free_id());
}

// Documents current behavior rather than endorsing it: release_id performs no
// ownership check, so releasing an id that was never allocated injects it into
// circulation. A caller that releases the same entity twice through different
// code paths can hand one id to two entities.
TEST(IdManager, ReleasingANeverAllocatedIdInjectsIt) {
  IdManager ids;
  ids.release_id(9999);
  EXPECT_EQ(9999, ids.get_free_id());
  EXPECT_EQ(1, ids.get_free_id());
}

// _max_id is a uint16_t incremented without a bounds check. Past 65535 it wraps
// to 0 and starts colliding with live ids. Pinned so the day someone adds
// exhaustion handling, this test is what tells them the old behavior is gone.
TEST(IdManager, FreshIdsWrapAroundAt16Bits) {
  IdManager ids;
  for (int i = 1; i < 65536; ++i) ids.get_free_id();  // consumes 1..65535

  EXPECT_EQ(0, ids.get_free_id());  // wrapped
  EXPECT_EQ(1, ids.get_free_id());  // and now collides with the very first id
}
