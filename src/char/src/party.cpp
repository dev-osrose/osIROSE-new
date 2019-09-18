#include "party.h"

std::shared_ptr<Party> cache_fetch_party(uint32_t charId) {
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::Party partyTable{};
  Core::PartyMembers partyMembersTable{};
  
  auto res = conn(sqlpp::select(sqlpp::all_of(partyTable)).from(
    partyMembersTable.join(partyTable).on(partyMembersTable.id == partyTable.id))
                  .where(partyMembersTable.memberId == charId));
  
  if (res.empty()) {
    return {};
  }
  auto result = res.front();
  
  Party party;
  party.id = result.id;
  party.name = result.name;
  party.leader = result.leaderId;
  party.options = result.options;
  party.level = result.level;
  party.last_got_item_index = result.lastGotItemIndex;
  party.last_got_zuly_index = result.lastGotZulyIndex;
  party.last_got_etc_index = result.lastGotEtcIndex;
  
  // now we fetch all party members
  auto res2 = conn(sqlpp::select(partyMembersTable.memberId).from(partyMembersTable)
                   .where(partyMembersTable.id == party.id).order_by(partyMembersTable.rank.desc()));
  
  if (res.empty()) {
    return {};
  }
  
  for (const auto& r : res2) {
    party.members.push_back(r.memberId);
  }

  return {party};
}

void cache_create_party(std::shared_ptr<Party> party) {
  if (!party) return;
  
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::Party partyTable{};
  
  conn(sqlpp::insert_into(partyTable)
       .set(partyTable.name = party->name)
       .set(partyTable.leaderId = party->leader)
       .set(partyTable.options = party->options)
       .set(partyTable.level = party->level)
       .set(partyTable.lastGotItemIndex = party->last_got_item_index)
       .set(partyTable.lastGotEtcIndex = party->last_got_etc_index)
       .set(partyTable.lastGotZulyIndex = party->last_got_zuly_index));
}

void cache_write_party(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::Party partyTable{};
  
  conn(sqlpp::update(partyTable)
       .set(partyTable.name = party->name)
       .set(partyTable.leaderId = party->leader)
       .set(partyTable.options = party->options)
       .set(partyTable.level = party->level)
       .set(partyTable.lastGotItemIndex = party->last_got_item_index)
       .set(partyTable.lastGotEtcIndex = party->last_got_etc_index)
       .set(partyTable.lastGotZulyIndex = party->last_got_zuly_index)
       .where(partyTable.id == party->id));
}

void cache_write_party_members(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyMembers partyMembersTable{};

  conn(sqlpp::remove_from(partyMembersTable).where(partyMembersTable.id == party->id));
  auto insert = sqlpp::insert_into(partyMembersTable).columns(partyMembersTable.id, partyMembersTable.memberId);
  for (auto m : party-> members) {
    multi_insert.values.add(partyMembersTable.id = party->id, partyMembersTable.memberId = m);
  }
  conn(insert);
}

void cache_remove_party(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::Party partyTable{};
  
  conn(sqlpp::remove_from(partyTable).where(partyTable.id == party->id));
}
