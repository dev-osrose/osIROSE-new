#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "connection.h"
#include "cli_party_req.h"

struct Party {
  uint32_t id = 0;
  uint32_t xp = 0;
  std::string name;
  uint32_t leader = 0;
  uint8_t options = 0;
  uint8_t level = 1;
  uint32_t last_got_item_index = 0;
  uint32_t last_got_zuly_index = 0;
  uint32_t last_got_etc_index = 0;
  std::vector<uint32_t> members;
};

std::shared_ptr<Party> cache_fetch_party(uint32_t charId);
void cache_create_party(std::shared_ptr<Party> party);
void cache_write_party(std::shared_ptr<Party> party);
void cache_write_party_members(std::shared_ptr<Party> party);
void cache_remove_party(std::shared_ptr<Party> party);

class PartyCache {
  public:
    std::shared_ptr<Party> get_party(uint32_t charId) {
      if (cache.count(charId) != 0) {
        return cache.at(charId);
      }
      auto party = cache_fetch_party(charId);
      if (!party) { // no party!
        return {};
      }
      cache.insert({charId, party});

      for (auto m : party->members) {
        cache.insert({m, party});
      }
      return party;
    }
  
    std::shared_ptr<Party> create_party(uint32_t charId) {
      auto party = std::make_shared<Party>();
      party->members.push_back(charId);
      party->leader = charId;
      cache_create_party(party);
      cache_write_party_members(party);
      party = cache_fetch_party(charId);
      cache[charId] = party;
      return party;
    }
  
    void add_member_to_party(std::shared_ptr<Party> party, uint32_t member) {
      party->members.push_back(member);
      cache.insert({member, party});
    
      cache_write_party_members(party);
    }
  
    void remove_member_from_party(std::shared_ptr<Party> party, uint32_t member) {
      party->members.erase(std::remove(party->members.begin(), party->members.end(), member), party->members.end());
      cache.erase(member);
    
      if (party->members.empty()) {
        cache_remove_party(party);
      } else {
        cache_write_party_members(party);
      }
    }

    void remove_party(std::shared_ptr<Party> party) {
        for (auto member : party->members) {
            cache.erase(member);
        }
        cache_remove_party(party);
    }
  
  private:
    std::unordered_map<uint32_t, std::shared_ptr<Party>> cache;
};

class CCharServer;
class User;
void party_request(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, User& user);
