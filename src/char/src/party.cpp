#include "party.h"
#include "user.h"
#include "ccharserver.h"
#include "logconsole.h"

std::shared_ptr<Party> cache_fetch_party(uint32_t charId) {
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable partyTable{};
  Core::PartyMembersTable partyMembersTable{};
  
  auto res = conn(sqlpp::select(sqlpp::all_of(partyTable)).from(
    partyMembersTable.join(partyTable).on(partyMembersTable.id == partyTable.id))
                  .where(partyMembersTable.memberId == charId));
  
  if (res.empty()) {
    return {};
  }
  auto& result = res.front();
  
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

  return std::make_shared<Party>(party);
}

void cache_create_party(std::shared_ptr<Party> party) {
  if (!party) return;
  
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable partyTable{};
  
  conn(sqlpp::insert_into(partyTable)
       .set(partyTable.name = party->name,
       partyTable.leaderId = party->leader,
       partyTable.options = party->options,
       partyTable.level = party->level,
       partyTable.lastGotItemIndex = party->last_got_item_index,
       partyTable.lastGotEtcIndex = party->last_got_etc_index,
       partyTable.lastGotZulyIndex = party->last_got_zuly_index));
}

void cache_write_party(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable partyTable{};
  
  conn(sqlpp::update(partyTable)
       .set(partyTable.name = party->name,
       partyTable.leaderId = party->leader,
       partyTable.options = party->options,
       partyTable.level = party->level,
       partyTable.lastGotItemIndex = party->last_got_item_index,
       partyTable.lastGotEtcIndex = party->last_got_etc_index,
       partyTable.lastGotZulyIndex = party->last_got_zuly_index)
       .where(partyTable.id == party->id));
}

void cache_write_party_members(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyMembersTable partyMembersTable{};

  conn(sqlpp::remove_from(partyMembersTable).where(partyMembersTable.id == party->id));
  auto insert = sqlpp::insert_into(partyMembersTable).columns(partyMembersTable.id, partyMembersTable.memberId);
  for (auto m : party-> members) {
    insert.values.add(partyMembersTable.id = party->id, partyMembersTable.memberId = static_cast<int>(m));
  }
  conn(insert);
}

void cache_remove_party(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable partyTable{};
  
  conn(sqlpp::remove_from(partyTable).where(partyTable.id == party->id));
}

void party_request(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, User& user) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("party_request({})", user.get_name());
  
    using namespace RoseCommon::Packet;
    switch (packet.get_request()) {
        case CliPartyReq::MAKE: // idXorTag == id
        {
            auto other = server.get_user(packet.get_idXorTag(), user.get_mapId());
            if (!other) {
              logger->warn("User ({}, {}) doesn't exist", packet.get_idXorTag(), user.get_mapId());
              return;
            }
            logger->debug("{} wants to make a party with {}", user.get_name(), other.value()->get_name());
            break;
        }
        case CliPartyReq::JOIN: // idXorTag == id
        {
            auto other = server.get_user(packet.get_idXorTag(), user.get_mapId());
            if (!other) {
              logger->warn("User ({}, {}) doesn't exist", packet.get_idXorTag(), user.get_mapId());
              return;
            }
            logger->debug("{} wants to join {}'s party", user.get_name(), other.value()->get_name());
            break;
        }
        case CliPartyReq::LEFT: // idXorTag == tag
        {
            logger->debug("{} left the party", user.get_name());
            break;
        }
        case CliPartyReq::CHANGE_OWNER: // idXorTag == tag
        {
            auto other = server.get_user(packet.get_idXorTag());
            if (!other) {
              logger->warn("User {} doesn't exist", packet.get_idXorTag());
              return;
            }
            logger->debug("{} wants to make {} the owner", user.get_name(), other.value()->get_name());
            break;
        }
        case CliPartyReq::KICK: // idXorTag == tag
        {
            auto other = server.get_user(packet.get_idXorTag());
            if (!other) {
              logger->warn("User {} doesn't exist", packet.get_idXorTag());
              return;
            }
            logger->debug("{} wants to kick {}", user.get_name(), other.value()->get_name());
            break;
        }
        default:
            logger->warn("Client {} sent a non valid request code {}", charId, packet.get_request());
    }
}

void party_reply(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, User& user) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("party_reply({})", user.get_name());
    
    auto* other;
    if (auto tmp = server.get_user(packet.get_idXorTag(), user.get_mapId()); !tmp) {
        logger->warn("Client {} replied to a party request of the non existing char {}", user.get_name(), packet.get_idXorTag());
        return;
    }
    
    using namespace RoseCommon::Packet;
    switch (packet.get_reply()) {
        case CliPartyReply::BUSY:
        case CliPartyReply::REJECT:
            logger->debug("{} refused {}'s party", other.value()->get_name(), user.get_name());
            break;
        case CliPartyReply::ACCEPT_MAKE:
        case CluPartyReply::ACCEPT_JOIN:
            logger->debug("{} accepted {}'s party", other.value()->get_name(), user.get_name());
            break;
    }
}
