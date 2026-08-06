#include "gtest/gtest.h"

#include "combat/combat.h"

#include <cstdint>
#include <limits>

// get_exp_to_level is the total experience required to reach a given level. It
// is five piecewise polynomials stitched together at 15/50/100/139 with a clamp
// at 210. The stitching is what a typo would break, and a non-monotonic curve
// would let a player lose levels by gaining experience.

TEST(ExpCurve, KnownValuesInEachBand) {
  // Recomputed from the formulas, so a change to any coefficient is caught.
  EXPECT_EQ(static_cast<int64_t>((1 + 3) * 1 * (1 + 10) * 0.7), Combat::get_exp_to_level(1));
  EXPECT_EQ(static_cast<int64_t>((15 + 3) * 15 * (15 + 10) * 0.7), Combat::get_exp_to_level(15));
  EXPECT_EQ(static_cast<int64_t>((30 - 5) * (30 + 2) * (30 + 2) * 2.2), Combat::get_exp_to_level(30));
  EXPECT_EQ(static_cast<int64_t>((75 - 5) * (75 + 2) * (75 - 38) * 9), Combat::get_exp_to_level(75));
  EXPECT_EQ(static_cast<int64_t>((120 + 27) * (120 + 34) * (120 + 220)), Combat::get_exp_to_level(120));
  EXPECT_EQ(static_cast<int64_t>((180 - 15) * (180 + 7) * (180 - 126) * 41), Combat::get_exp_to_level(180));
}

TEST(ExpCurve, BandBoundariesUseTheExpectedFormula) {
  // Each pair straddles a boundary: the lower value must still use the lower
  // band's formula and the upper value the next one.
  EXPECT_EQ(static_cast<int64_t>((15 + 3) * 15 * (15 + 10) * 0.7), Combat::get_exp_to_level(15));
  EXPECT_EQ(static_cast<int64_t>((16 - 5) * (16 + 2) * (16 + 2) * 2.2), Combat::get_exp_to_level(16));

  EXPECT_EQ(static_cast<int64_t>((50 - 5) * (50 + 2) * (50 + 2) * 2.2), Combat::get_exp_to_level(50));
  EXPECT_EQ(static_cast<int64_t>((51 - 5) * (51 + 2) * (51 - 38) * 9), Combat::get_exp_to_level(51));

  EXPECT_EQ(static_cast<int64_t>((100 - 5) * (100 + 2) * (100 - 38) * 9), Combat::get_exp_to_level(100));
  EXPECT_EQ(static_cast<int64_t>((101 + 27) * (101 + 34) * (101 + 220)), Combat::get_exp_to_level(101));

  EXPECT_EQ(static_cast<int64_t>((139 + 27) * (139 + 34) * (139 + 220)), Combat::get_exp_to_level(139));
  EXPECT_EQ(static_cast<int64_t>((140 - 15) * (140 + 7) * (140 - 126) * 41), Combat::get_exp_to_level(140));
}

// The property that actually matters: experience requirements never go
// backwards. A wrong coefficient at a seam shows up here even if the
// hand-computed values above were all copied from the same mistake.
TEST(ExpCurve, IsStrictlyIncreasingUpToTheCap) {
  int64_t previous = Combat::get_exp_to_level(1);
  for (int level = 2; level <= 210; ++level) {
    const int64_t current = Combat::get_exp_to_level(level);
    EXPECT_GT(current, previous) << "exp requirement dropped at level " << level;
    previous = current;
  }
}

TEST(ExpCurve, IsPositiveForEveryRealLevel) {
  for (int level = 1; level <= 210; ++level) {
    EXPECT_GT(Combat::get_exp_to_level(level), 0) << "at level " << level;
  }
}

TEST(ExpCurve, ClampsAt210) {
  const int64_t cap = Combat::get_exp_to_level(210);
  EXPECT_EQ(cap, Combat::get_exp_to_level(211));
  EXPECT_EQ(cap, Combat::get_exp_to_level(1000));
  EXPECT_EQ(cap, Combat::get_exp_to_level(std::numeric_limits<int>::max()));
}

// Documents current behavior at inputs the callers should never produce. There
// is a clamp at the top (level > 210) but nothing at the bottom, so the
// level <= 15 polynomial is evaluated as written. Its three roots at 0, -3 and
// -10 make the result change sign twice, which is worth pinning precisely
// because "negative level, negative answer" is the intuitive guess and it is
// wrong.
TEST(ExpCurve, HasNoLowerBoundGuard) {
  EXPECT_EQ(0, Combat::get_exp_to_level(0)) << "root of the (level) factor";
  EXPECT_LT(Combat::get_exp_to_level(-1), 0);
  EXPECT_LT(Combat::get_exp_to_level(-2), 0);

  // Between the roots at -3 and -10 the product turns positive again: a
  // negative level asks for a *positive* amount of experience.
  EXPECT_GT(Combat::get_exp_to_level(-5), 0);
  EXPECT_GT(Combat::get_exp_to_level(-9), 0);

  EXPECT_EQ(0, Combat::get_exp_to_level(-10)) << "root of the (level + 10) factor";
  EXPECT_LT(Combat::get_exp_to_level(-20), 0);
}
