#include "gtest/gtest.h"

#include "lua_api.h"
#include "lua_db.h"

#include <sol/sol.hpp>

#include <algorithm>
#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// The map server reads every NPC/mob stat and runs every item and NPC hook
// through these two classes. Both are thin wrappers over sol2, and both are
// reachable in a test with nothing but a sol::state -- no EntitySystem, no
// database, no network.
//
// The two halves behave very differently on bad input, and that difference is
// the point of this file:
//
//   * LuaApi::safe_lua_call degrades gracefully -- a missing or erroring Lua
//     function is logged and yields a value-initialised result.
//   * LuaData::get_data does not -- it is an unguarded static_cast on a
//     sol::proxy, so a bad key takes the whole process down through sol2's
//     panic handler. With 573 script files feeding it, that is worth pinning.

namespace {

// Every getter paired with the key it is supposed to read. A copy-paste slip in
// lua_db.h -- two getters reading the same key -- only shows up if each key
// holds a distinct value, which is what the fixture below arranges.
using Getter = int (LuaData::*)();

const std::vector<std::pair<const char*, Getter>>& getters() {
  static const std::vector<std::pair<const char*, Getter>> kGetters = {
      {"walk_speed", &LuaData::get_walk_speed},
      {"run_speed", &LuaData::get_run_speed},
      {"scale", &LuaData::get_scale},
      {"r_weapon", &LuaData::get_r_weapon},
      {"l_weapon", &LuaData::get_l_weapon},
      {"level", &LuaData::get_level},
      {"hp", &LuaData::get_hp},
      {"attack", &LuaData::get_attack},
      {"hit", &LuaData::get_hit},
      {"def", &LuaData::get_def},
      {"res", &LuaData::get_res},
      {"avoid", &LuaData::get_avoid},
      {"attack_spd", &LuaData::get_attack_spd},
      {"is_magic_damage", &LuaData::get_is_magic_damage},
      {"ai_type", &LuaData::get_ai_type},
      {"give_exp", &LuaData::get_give_exp},
      {"drop_type", &LuaData::get_drop_type},
      {"drop_money", &LuaData::get_drop_money},
      {"drop_item", &LuaData::get_drop_item},
      {"union_number", &LuaData::get_union_number},
      {"need_summon_count", &LuaData::get_need_summon_count},
      {"sell_tab0", &LuaData::get_sell_tab0},
      {"sell_tab1", &LuaData::get_sell_tab1},
      {"sell_tab2", &LuaData::get_sell_tab2},
      {"sell_tab3", &LuaData::get_sell_tab3},
      {"can_target", &LuaData::get_can_target},
      {"attack_range", &LuaData::get_attack_range},
      {"npc_type", &LuaData::get_npc_type},
      {"hit_material_type", &LuaData::get_hit_material_type},
      {"face_icon", &LuaData::get_face_icon},
      {"summon_mob_type", &LuaData::get_summon_mob_type},
      {"quest_type", &LuaData::get_quest_type},
      {"height", &LuaData::get_height},
  };
  return kGetters;
}

// Distinct per key, and none of them 0, so a getter that silently reads a
// missing key cannot accidentally match.
int expected_value(size_t index) { return 1000 + static_cast<int>(index); }

sol::table make_full_table(sol::state& lua) {
  sol::table t = lua.create_table();
  for (size_t i = 0; i < getters().size(); ++i) {
    t[getters()[i].first] = expected_value(i);
  }
  return t;
}

// safe_lua_call is protected, so reaching it directly needs a subclass. The
// real subclasses (ItemLuaApi, NpcLuaApi) are exercised separately below.
class ExposedLuaApi : public LuaApi {
 public:
  using LuaApi::LuaApi;

  template <typename T, typename... Args>
  T call(const std::string& name, Args&&... args) {
    return safe_lua_call<T>(name, std::forward<Args>(args)...);
  }
};

// One sol::state per test, with an environment that falls back to the globals
// so scripts can still reach error(), tostring() and friends.
class LuaFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    lua.open_libraries(sol::lib::base);
    env = sol::environment(lua, sol::create, lua.globals());
  }

  void run(const std::string& source) { lua.script(source, env); }

  sol::state lua;
  sol::environment env;
};

}  // namespace

// ---------------------------------------------------------------------------
// LuaData
// ---------------------------------------------------------------------------

TEST_F(LuaFixture, EveryGetterReadsItsOwnKey) {
  LuaData data(make_full_table(lua));

  for (size_t i = 0; i < getters().size(); ++i) {
    const auto& [key, getter] = getters()[i];
    EXPECT_EQ(expected_value(i), (data.*getter)())
        << "getter for \"" << key << "\" read the wrong key";
  }
}

TEST_F(LuaFixture, GettersCoverEveryKeyExactlyOnce) {
  // C++ has no reflection, so nothing can prove the table above lists every
  // getter in lua_db.h -- a getter added there and not here is simply
  // untested. This count is the tripwire for the other direction: it fails if
  // someone edits the table without noticing it is meant to be exhaustive,
  // which is the moment to go re-check lua_db.h.
  EXPECT_EQ(33u, getters().size());

  std::vector<std::string> keys;
  for (const auto& [key, getter] : getters()) keys.emplace_back(key);
  std::sort(keys.begin(), keys.end());
  EXPECT_EQ(keys.end(), std::unique(keys.begin(), keys.end()))
      << "two getters are mapped to the same key";
}

TEST_F(LuaFixture, ReadsValuesFromAnActualScript) {
  // The production path is a table returned by a script file, not one built in
  // C++, so read one back through the interpreter too.
  run(R"(mob = { level = 42, hp = 1234, attack = 7, drop_money = 0 })");
  LuaData data(env["mob"].get<sol::table>());

  EXPECT_EQ(42, data.get_level());
  EXPECT_EQ(1234, data.get_hp());
  EXPECT_EQ(7, data.get_attack());
  EXPECT_EQ(0, data.get_drop_money()) << "a legitimate zero must survive";
}

TEST_F(LuaFixture, AcceptsOnlyIntegerSubtypeArithmetic) {
  // Lua 5.3+ distinguishes integer and float subtypes, and sol2 reads these
  // through lua_tointegerx, which rejects the float subtype outright. So the
  // arithmetic a script uses to compute a stat matters: `*`, `+` and `//` on
  // integers stay integers, but `/` and `^` always produce floats. See the
  // death test below.
  run(R"(mob = { level = 5 * 2, attack = 100 // 4, hp = 1000 + 234 })");
  LuaData data(env["mob"].get<sol::table>());

  EXPECT_EQ(10, data.get_level());
  EXPECT_EQ(25, data.get_attack());
  EXPECT_EQ(1234, data.get_hp());
}

// BUG, pinned as current behavior rather than fixed here.
//
// get_data is `static_cast<T>(data[name])` on a sol proxy, with no check that
// the key exists. A missing key trips sol2's panic handler, which prints to
// stderr and throws sol::error. Nothing in the map server catches that, so one
// typo'd key in any of the 573 files under scripts/ takes the process down.
//
// Catching it does not help either, which is the part worth knowing: the
// lua_State is left corrupt, and a caller that swallows the sol::error and
// carries on dies later in the allocator ("free(): invalid size") with no
// remaining connection to the script that caused it.
//
// The fix would be a sol::optional read with a default, but that changes the
// value every existing script sees for keys it omits, so it is out of scope for
// this test pass. This test is what such a fix has to flip.
TEST(LuaDataDeathTest, MissingKeyAbortsTheProcess) {
  EXPECT_DEATH(
      {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        sol::table t = lua.create_table();
        t["level"] = 5;
        LuaData data(t);
        // "level" is present; "hp" is not.
        (void)data.get_hp();
      },
      "received nil");
}

// Same unguarded static_cast, second way to trip it, and a much easier one to
// hit than it looks: sol2 rejects Lua's *float subtype*, not just fractional
// values. 10.0 is as fatal as 10.5, which means `hp = base / 2` kills the
// server even when base is even. Only integer-subtype arithmetic survives.
TEST(LuaDataDeathTest, FloatStatAbortsEvenWithNoFractionalPart) {
  EXPECT_DEATH(
      {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        sol::table t = lua.create_table();
        t["hp"] = 10.0;  // not 10.5 -- exactly ten, as a float
        LuaData data(t);
        (void)data.get_hp();
      },
      "fits exactly an integer");
}

TEST(LuaDataDeathTest, FloatDivisionInAScriptAbortsTheProcess) {
  // The same thing as a script would actually write it.
  EXPECT_DEATH(
      {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        sol::environment env(lua, sol::create, lua.globals());
        lua.script(R"(mob = { hp = 100 / 4 })", env);
        LuaData data(env["mob"].get<sol::table>());
        (void)data.get_hp();
      },
      "fits exactly an integer");
}

// And a third: a stat written as a string. Lua would happily coerce it, sol2
// will not.
TEST(LuaDataDeathTest, StringStatAbortsTheProcess) {
  EXPECT_DEATH(
      {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        sol::table t = lua.create_table();
        t["hp"] = "100";
        LuaData data(t);
        (void)data.get_hp();
      },
      "expected number");
}

// ---------------------------------------------------------------------------
// LuaApi::safe_lua_call
// ---------------------------------------------------------------------------

TEST_F(LuaFixture, ReturnsTheValueFromLua) {
  run(R"(function GetNumber() return 5 end)");
  ExposedLuaApi api(env);

  EXPECT_EQ(5, api.call<int>("GetNumber"));
}

TEST_F(LuaFixture, ConvertsLuaTruthinessToBool) {
  run(R"(
    function Yes() return true end
    function No() return false end
  )");
  ExposedLuaApi api(env);

  EXPECT_TRUE(api.call<bool>("Yes"));
  EXPECT_FALSE(api.call<bool>("No"));
}

TEST_F(LuaFixture, ForwardsArgumentsToLua) {
  run(R"(function Add(a, b) return a + b end)");
  ExposedLuaApi api(env);

  EXPECT_EQ(30, api.call<int>("Add", 10, 20));
}

// The contrast with LuaData above: a Lua function that does not exist is a nil
// call, which protected_function turns into an invalid result rather than a
// panic. safe_lua_call logs it and returns a value-initialised T.
TEST_F(LuaFixture, MissingFunctionReturnsDefaultInsteadOfCrashing) {
  ExposedLuaApi api(env);

  EXPECT_EQ(0, api.call<int>("NoSuchFunction"));
  EXPECT_FALSE(api.call<bool>("NoSuchFunction"));
}

TEST_F(LuaFixture, LuaErrorIsSwallowedNotPropagated) {
  run(R"(function Boom() error("kaboom") end)");
  ExposedLuaApi api(env);

  int result = 1;
  EXPECT_NO_THROW(result = api.call<int>("Boom"));
  EXPECT_EQ(0, result);
}

TEST_F(LuaFixture, RuntimeErrorsAreCaughtToo) {
  // Not an explicit error() call -- an ordinary bug in a script.
  run(R"(function Bad() return nil + 1 end)");
  ExposedLuaApi api(env);

  EXPECT_EQ(0, api.call<int>("Bad"));
}

TEST_F(LuaFixture, VoidCallsSurviveMissingAndErroringFunctions) {
  run(R"(function Boom() error("kaboom") end)");
  ExposedLuaApi api(env);

  EXPECT_NO_THROW(api.call<void>("Boom"));
  EXPECT_NO_THROW(api.call<void>("AlsoMissing"));
}

TEST_F(LuaFixture, LifecycleHooksMapToTheExpectedLuaNames) {
  run(R"(
    function OnInit() return true end
    function OnCreate() return true end
    function OnDelete() return false end
  )");
  ExposedLuaApi api(env);

  EXPECT_TRUE(api.on_init());
  EXPECT_TRUE(api.on_create());
  EXPECT_FALSE(api.on_delete());
}

TEST_F(LuaFixture, LifecycleHooksAreOptional) {
  // Most scripts define only a couple of these, so the absent ones must be
  // harmless rather than fatal.
  ExposedLuaApi api(env);

  EXPECT_FALSE(api.on_init());
  EXPECT_FALSE(api.on_create());
  EXPECT_FALSE(api.on_delete());
}

TEST_F(LuaFixture, RegisteredFunctionIsCallableFromLua) {
  ExposedLuaApi api(env);

  int seen = 0;
  api.register_function<void, int>("record", std::function<void(int)>([&seen](int v) { seen = v; }));
  run(R"(record(99))");

  EXPECT_EQ(99, seen);
}

// ---------------------------------------------------------------------------
// ItemLuaApi / NpcLuaApi
// ---------------------------------------------------------------------------

TEST_F(LuaFixture, ItemHooksMapToTheExpectedLuaNames) {
  run(R"(
    called = {}
    local function mark(name) return function(entity) called[name] = entity; return true end end
    OnEquip   = mark("OnEquip")
    OnUnequip = mark("OnUnequip")
    OnDrop    = mark("OnDrop")
    OnPickup  = mark("OnPickup")
    OnUse     = mark("OnUse")
  )");
  ItemLuaApi api(env);
  const Entity entity = static_cast<Entity>(17);

  EXPECT_TRUE(api.on_equip(entity));
  EXPECT_TRUE(api.on_unequip(entity));
  EXPECT_TRUE(api.on_drop(entity));
  EXPECT_TRUE(api.on_pickup(entity));
  EXPECT_TRUE(api.on_use(entity));

  sol::table called = env["called"];
  for (const char* name : {"OnEquip", "OnUnequip", "OnDrop", "OnPickup", "OnUse"}) {
    // The entity round-trips through Lua as its underlying integer.
    EXPECT_EQ(17, called[name].get<int>()) << name << " got the wrong entity";
  }
}

TEST_F(LuaFixture, ItemStatGettersMapToTheExpectedLuaNames) {
  run(R"(
    function GetAttackSpd() return 115 end
    function GetMoveSpd() return 220 end
    function GetMagic() return 3 end
  )");
  ItemLuaApi api(env);

  EXPECT_EQ(115, api.get_attack_speed());
  EXPECT_EQ(220, api.get_move_speed());
  EXPECT_EQ(3, api.get_magic());
}

TEST_F(LuaFixture, ItemStatGettersDefaultToZeroWhenUndefined) {
  // Most item scripts define none of these; the values feed straight into stat
  // calculations, so the fallback has to be a benign zero.
  ItemLuaApi api(env);

  EXPECT_EQ(0, api.get_attack_speed());
  EXPECT_EQ(0, api.get_move_speed());
  EXPECT_EQ(0, api.get_magic());
}

// The full round trip an item script actually performs: C++ registers the
// callback, Lua calls it from inside a hook, C++ observes the arguments.
TEST_F(LuaFixture, BonusAttrCallbacksRoundTripThroughLua) {
  run(R"(
    function OnEquip(entity)
      addBonusAttr(entity, 1, 10)
      return true
    end
    function OnUnequip(entity)
      removeBonusAttr(entity, 1, 10)
      return true
    end
  )");
  ItemLuaApi api(env);

  std::vector<std::tuple<int, int, int>> added, removed;
  api.register_add_bonus_attr([&added](Entity e, int attr, int value) {
    added.emplace_back(static_cast<int>(e), attr, value);
  });
  api.register_remove_bonus_attr([&removed](Entity e, int attr, int value) {
    removed.emplace_back(static_cast<int>(e), attr, value);
  });

  const Entity entity = static_cast<Entity>(3);
  EXPECT_TRUE(api.on_equip(entity));
  EXPECT_TRUE(api.on_unequip(entity));

  ASSERT_EQ(1u, added.size());
  EXPECT_EQ(std::make_tuple(3, 1, 10), added[0]);
  ASSERT_EQ(1u, removed.size());
  EXPECT_EQ(std::make_tuple(3, 1, 10), removed[0]);
}

// A script that forgets to register its callbacks -- or a hook that fires
// before registration -- errors inside Lua rather than reaching C++. The hook
// still returns cleanly.
TEST_F(LuaFixture, UnregisteredBonusAttrCallbackFailsInsideLua) {
  run(R"(
    function OnEquip(entity)
      addBonusAttr(entity, 1, 10)
      return true
    end
  )");
  ItemLuaApi api(env);

  EXPECT_FALSE(api.on_equip(static_cast<Entity>(3)))
      << "the nil callback should have failed the whole hook";
}

TEST_F(LuaFixture, NpcHooksMapToTheExpectedLuaNames) {
  run(R"(
    dead = 0
    damaged = 0
    function OnDead(entity) dead = entity end
    function OnDamaged(entity) damaged = entity end
  )");
  NpcLuaApi api(env);

  const Entity npc = static_cast<Entity>(21);
  const Entity attacker = static_cast<Entity>(22);
  api.on_dead(npc, attacker);
  api.on_damaged(npc, attacker);

  EXPECT_EQ(21, env["dead"].get<int>());
  EXPECT_EQ(21, env["damaged"].get<int>());
}

TEST_F(LuaFixture, NpcHooksAreOptional) {
  NpcLuaApi api(env);

  EXPECT_NO_THROW(api.on_dead(static_cast<Entity>(1), static_cast<Entity>(2)));
  EXPECT_NO_THROW(api.on_damaged(static_cast<Entity>(1), static_cast<Entity>(2)));
}

// Each script file gets its own sol::environment, and that is the only thing
// keeping 573 scripts from stomping on each other's OnInit.
TEST_F(LuaFixture, EnvironmentsAreIsolatedFromEachOther) {
  sol::environment other(lua, sol::create, lua.globals());

  lua.script(R"(function OnInit() return true end)", env);
  lua.script(R"(function OnInit() return false end)", other);

  ExposedLuaApi first(env);
  ExposedLuaApi second(other);

  EXPECT_TRUE(first.on_init());
  EXPECT_FALSE(second.on_init());
}
