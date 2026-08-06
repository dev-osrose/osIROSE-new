#include "gtest/gtest.h"

#include "party.h"
#include "user.h"

#include <memory>

// Party::options is a bitfield shared with the client, and User owns a small
// state machine around party membership. Both are pure data with no I/O, and
// both encode invariants that are currently only visible by reading the code.

namespace {
constexpr auto EXP_PER_PLAYER = RoseCommon::Packet::CliPartyRule::EXP_PER_PLAYER;
constexpr auto ITEM_IN_ORDER = RoseCommon::Packet::CliPartyRule::ITEM_IN_ORDER;

User make_user() {
  // The weak_ptr is never locked by anything under test.
  return User(std::weak_ptr<CCharClient>{}, "tester", 42, 7);
}
}  // namespace

TEST(PartyOptions, DefaultsToAllFlagsClear) {
  Party party;
  EXPECT_EQ(0, party.options);
  EXPECT_FALSE(party.is_xp_per_player());
  EXPECT_FALSE(party.is_item_in_order());
}

TEST(PartyOptions, XpPerPlayerRoundTrips) {
  Party party;

  party.set_xp_per_player();
  EXPECT_TRUE(party.is_xp_per_player());

  party.unset_xp_per_player();
  EXPECT_FALSE(party.is_xp_per_player());
}

TEST(PartyOptions, ItemInOrderRoundTrips) {
  Party party;

  party.set_item_in_order();
  EXPECT_TRUE(party.is_item_in_order());

  party.unset_item_in_order();
  EXPECT_FALSE(party.is_item_in_order());
}

// The two flags share one byte, so a wrong mask would make them clobber each
// other. EXP_PER_PLAYER is bit 0 and ITEM_IN_ORDER is bit 7.
TEST(PartyOptions, FlagsAreIndependent) {
  Party party;

  party.set_xp_per_player();
  party.set_item_in_order();
  EXPECT_TRUE(party.is_xp_per_player());
  EXPECT_TRUE(party.is_item_in_order());

  party.unset_xp_per_player();
  EXPECT_FALSE(party.is_xp_per_player());
  EXPECT_TRUE(party.is_item_in_order()) << "clearing one flag cleared the other";

  party.set_xp_per_player();
  party.unset_item_in_order();
  EXPECT_TRUE(party.is_xp_per_player());
  EXPECT_FALSE(party.is_item_in_order());
}

TEST(PartyOptions, SettingIsIdempotent) {
  Party party;

  party.set_xp_per_player();
  const uint8_t after_first = party.options;
  party.set_xp_per_player();

  EXPECT_EQ(after_first, party.options);
}

TEST(PartyOptions, UsesTheWireFlagValues) {
  Party party;

  party.set_xp_per_player();
  EXPECT_EQ(EXP_PER_PLAYER, party.options);

  party.unset_xp_per_player();
  party.set_item_in_order();
  EXPECT_EQ(ITEM_IN_ORDER, party.options);
}

// Unrelated bits must survive, since options is sent to and from the client.
TEST(PartyOptions, PreservesUnknownBits) {
  Party party;
  party.options = 0b0011'0000;

  party.set_xp_per_player();
  party.unset_item_in_order();

  EXPECT_EQ(0b0011'0000, party.options & 0b0011'0000);
}

TEST(UserState, StartsConnectingWithNoParty) {
  auto user = make_user();

  EXPECT_EQ(User::Status::CONNECTING, user.get_status());
  EXPECT_EQ(42u, user.get_charId());
  EXPECT_EQ(7u, user.get_mapId());
  EXPECT_EQ("tester", user.get_name());
  EXPECT_EQ(nullptr, user.get_party());
  EXPECT_EQ(nullptr, user.get_requested_party());
}

TEST(UserState, StatusRoundTrips) {
  auto user = make_user();

  user.set_status(User::Status::CONNECTED);
  EXPECT_EQ(User::Status::CONNECTED, user.get_status());

  user.set_status(User::Status::AFK);
  EXPECT_EQ(User::Status::AFK, user.get_status());
}

// The ISC overload translates the cross-server status enum into the local one.
TEST(UserState, TranslatesIscStatus) {
  using Isc = RoseCommon::Packet::IscClientStatus::Status;
  auto user = make_user();

  user.set_status(Isc::CONNECTED);
  EXPECT_EQ(User::Status::CONNECTED, user.get_status());

  user.set_status(Isc::SWITCHING);
  EXPECT_EQ(User::Status::SWITCHING, user.get_status());

  user.set_status(Isc::INVISIBLE);
  EXPECT_EQ(User::Status::INVISIBLE, user.get_status());

  user.set_status(Isc::DISCONNECTED);
  EXPECT_EQ(User::Status::DISCONNECTED, user.get_status());

  user.set_status(Isc::AFK);
  EXPECT_EQ(User::Status::AFK, user.get_status());
}

// A user is either in a party or waiting on an invite, never both. Each setter
// clears the other field.
TEST(UserState, JoiningAPartyClearsThePendingInvite) {
  auto user = make_user();
  auto invited = std::make_shared<Party>();
  auto joined = std::make_shared<Party>();

  user.set_requested_party(invited);
  EXPECT_EQ(invited, user.get_requested_party());
  EXPECT_EQ(nullptr, user.get_party());

  user.set_party(joined);
  EXPECT_EQ(joined, user.get_party());
  EXPECT_EQ(nullptr, user.get_requested_party()) << "pending invite outlived joining";
}

TEST(UserState, BeingInvitedClearsCurrentParty) {
  auto user = make_user();
  auto joined = std::make_shared<Party>();
  auto invited = std::make_shared<Party>();

  user.set_party(joined);
  EXPECT_EQ(joined, user.get_party());

  user.set_requested_party(invited);
  EXPECT_EQ(invited, user.get_requested_party());
  EXPECT_EQ(nullptr, user.get_party()) << "membership outlived a new invite";
}

TEST(UserState, LeavingAPartyClearsBothFields) {
  auto user = make_user();

  user.set_party(std::make_shared<Party>());
  user.set_party(nullptr);

  EXPECT_EQ(nullptr, user.get_party());
  EXPECT_EQ(nullptr, user.get_requested_party());
}

TEST(UserState, MapIdIsMutableForServerTransfers) {
  auto user = make_user();

  user.set_mapId(99);
  EXPECT_EQ(99u, user.get_mapId());
  EXPECT_EQ(42u, user.get_charId()) << "charId must not move with the map";
}
