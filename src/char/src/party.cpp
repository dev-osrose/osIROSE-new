#include "party.h"
#include "user.h"
#include "ccharserver.h"
#include "logconsole.h"

#include "srv_party_req.h"
#include "srv_party_reply.h"
#include "cli_party_reply.h"

#include <unordered_set>

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
  
  if (res2.empty()) {
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

  auto res = conn(sqlpp::select(partyTable.id).from(partyTable)
          .where(partyTable.leaderId == party->leader));

  if (res.empty()) {
      return;
  }

  party->id = res.front().id;
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
    insert.values.add(partyMembersTable.id = party->id, partyMembersTable.memberId = m);
  }
  conn(insert);
}

void cache_remove_party(std::shared_ptr<Party> party) {
  if (!party) return;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::PartyTable partyTable{};
  
  conn(sqlpp::remove_from(partyTable).where(partyTable.id == party->id));
}

std::shared_ptr<Party> PartyManager::get_party(uint32_t charId) {
    if (partys.count(charId) != 0) {
        return partys.at(charId);
    }
    auto party = cache_fetch_party(charId);
    if (!party) { // no party!
        return {};
    }
    partys.insert({charId, party});

    for (auto m : party->members) {
        partys.insert({m, party});
    }
    return party;
}

std::shared_ptr<Party> PartyManager::create_party(uint32_t charId) {
    auto party = std::make_shared<Party>();
    party->members.push_back(charId);
    party->leader = charId;
    cache_create_party(party);
    cache_write_party_members(party);
    party->members.push_back(charId);
    partys[charId] = party;
    return party;
}

void PartyManager::add_member_to_party(std::shared_ptr<Party> party, uint32_t member) {
    party->members.push_back(member);
    partys.insert({member, party});
    
    cache_write_party_members(party);
}

void PartyManager::remove_member_from_party(std::shared_ptr<Party> party, uint32_t member) {
    party->members.erase(std::remove(party->members.begin(), party->members.end(), member), party->members.end());
    partys.erase(member);
    
    if (party->members.empty()) {
        cache_remove_party(party);
    } else {
        cache_write_party_members(party);
    }
}

void PartyManager::remove_party(std::shared_ptr<Party> party) {
    for (auto member : party->members) {
        partys.erase(member);
    }
    cache_remove_party(party);
}

PartyManager::~PartyManager() {
    std::unordered_set<std::shared_ptr<Party>> tmp;
    for (const auto& [_, party] : partys) {
        tmp.insert(party);
    }
    for (const auto& party : tmp) {
        cache_remove_party(party);
    }
}

void party_request(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, User& user) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("party_request({})", user.get_name());
  
    using namespace RoseCommon::Packet;
    switch (packet.get_type()) {
        case CliPartyReq::CREATE: // idXorTag == id
        {
            auto other = server.get_user(packet.get_target(), user.get_mapId());
            if (!other) {
              logger->warn("User ({}, {}) doesn't exist", packet.get_target(), user.get_mapId());
              server.send_char(user, SrvPartyReply::create(SrvPartyReply::NOT_FOUND, packet.get_target()));
              return;
            }
            logger->debug("{} wants to make a party with {}", user.get_name(), other.value()->get_name());
            if (user.get_party()) {
                logger->error("{} wants to make a party but is already in a party", user.get_name());
                server.send_char(user, SrvPartyReply::create(SrvPartyReply::REJECT_JOIN, user.get_entityId()));
                return;
            }
            if (user.get_requested_party()) {
                logger->warn("user {} got another pending party request", user.get_name());
                server.send_char(user, SrvPartyReply::create(SrvPartyReply::BUSY, user.get_entityId()));
                return;
            }
            if (other.value()->get_requested_party()) {
                logger->warn("user {} got another pending party request", other.value()->get_name());
                server.send_char(user, SrvPartyReply::create(SrvPartyReply::BUSY, other.value()->get_entityId()));
                return;
            }
            if (other.value()->get_party()) {
                logger->debug("{} wants to make a party with {} which is already in a party", user.get_name(), other.value()->get_name());
                server.send_char(user, SrvPartyReply::create(SrvPartyReply::REJECT_JOIN, other.value()->get_entityId()));  
                return;
            }
            // TODO: check for level difference
            other.value()->set_requested_party(server.create_party(user));
            server.send_char(*other.value(), SrvPartyReq::create(SrvPartyReq::CREATE, user.get_entityId(), user.get_name()));
            break;
        }
        case CliPartyReq::JOIN: // idXorTag == id
        {
            auto other = server.get_user(packet.get_target(), user.get_mapId());
            if (!other) {
              logger->warn("User ({}, {}) doesn't exist", packet.get_target(), user.get_mapId());
              server.send_char(user, SrvPartyReply::create(SrvPartyReply::NOT_FOUND, packet.get_target()));
              return;
            }
            logger->debug("{} wants to join {}'s party", user.get_name(), other.value()->get_name());
            break;
        }
        case CliPartyReq::LEAVE: // idXorTag == tag
        {
            logger->debug("{} left the party", user.get_name());
            break;
        }
        case CliPartyReq::CHANGE_OWNER: // idXorTag == tag
        {
            auto other = server.get_user(packet.get_target());
            if (!other) {
              logger->warn("User {} doesn't exist", packet.get_target());
              server.send_char(user, SrvPartyReply::create(SrvPartyReply::NOT_FOUND, packet.get_target()));
              return;
            }
            logger->debug("{} wants to make {} the owner", user.get_name(), other.value()->get_name());
            break;
        }
        case CliPartyReq::BAN: // idXorTag == tag
        {
            auto other = server.get_user(packet.get_target());
            if (!other) {
              logger->warn("User {} doesn't exist", packet.get_target());
              server.send_char(user, SrvPartyReply::create(SrvPartyReply::NOT_FOUND, packet.get_target()));
              return;
            }
            logger->debug("{} wants to kick {}", user.get_name(), other.value()->get_name());
            break;
        }
        default:
            logger->warn("Client {} sent a non valid request code {}", user.get_name(), packet.get_target());
    }
}

void party_reply(const RoseCommon::Packet::CliPartyReply& packet, CCharServer& server, User& user) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("party_reply({})", user.get_name());
    
    auto tmp = server.get_user(packet.get_target(), user.get_mapId());
    if (!tmp) {
        logger->warn("Client {} replied to a party request of the non existing char {}", user.get_name(), packet.get_target());
        server.send_char(user, RoseCommon::Packet::SrvPartyReply::create(RoseCommon::Packet::SrvPartyReply::NOT_FOUND, packet.get_target()));
        return;
    }
    User*const other = tmp.value();
    if (!other->get_party()) {
        logger->warn("no party found for character {}", other->get_name());
        return;
    }
    
    using namespace RoseCommon::Packet;
    switch (packet.get_type()) {
        case CliPartyReply::BUSY:
            logger->debug("{} is too busy to accept {}'s party", user.get_name(), other->get_name());
            server.send_char(*other, SrvPartyReply::create(SrvPartyReply::BUSY, user.get_entityId()));
            break;
        case CliPartyReply::REJECT_JOIN:
            logger->debug("{} refused {}'s party", user.get_name(), other->get_name());
            server.send_char(*other, SrvPartyReply::create(SrvPartyReply::REJECT_JOIN, user.get_entityId()));
            break;
        case CliPartyReply::ACCEPT_CREATE:
        case CliPartyReply::ACCEPT_JOIN:
            logger->debug("{} accepted {}'s party", user.get_name(), other->get_name());
            if (user.get_requested_party() != other->get_party()) {
                logger->warn("{} tried to answer to a different party request from {}", user.get_name(), other->get_name());
                user.set_requested_party({}); // we reset the party request
                return;
            }
            server.send_char(*other, SrvPartyReply::create(SrvPartyReply::ACCEPT_JOIN, user.get_entityId()));
            server.add_user_to_party(user, other->get_party());
            break;
        default:
            logger->debug("{} replied {} to {}", user.get_name(), packet.get_type(), other->get_name());
            break;
    }
}
