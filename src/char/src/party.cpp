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
  
  for (auto& r : res2) {
    party.members.push_back(r.memberId);
  }

  return std::make_shared<Party>(party);
}

void cache_write_party(std::shared_ptr<Party> party) {
}

void cache_write_party_members(std::shared_ptr<Party> party) {
}

void cache_remove_party(std::shared_ptr<Party> party) {
}
