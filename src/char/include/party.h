#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "connection.h"

struct Party {
  uint32_t id;
  uint32_t xp = 0;
  std::string name;
  uint32_t leader;
  uint8_t options;
  uint8_t level;
  uint32_t last_got_item_index;
  uint32_t last_got_zuly_index;
  uint32_t last_got_etc_index;
  std::vector<uint32_t> members;
};

std::shared_ptr<Party> cache_fetch_party(uint32_t charId);
void cache_write_party(std::shared_ptr<Party> party);
void cache_write_party_members(std::shared_ptr<Party> party);
void cache_remove_party(std::shared_ptr<Party> party);

class PartyCache {
  public:
    std::shared_ptr<Party> get_party(uint32_t charId) {
      if (cache.contains(charId)) {
        return cache.at(charId);
      }
      auto [it, inserted] = cache.insert({charId, cache_fetch_party(charId)});
      
      if (!inserted) {
        return *it;
      }
      const auto& party = *it;
      for (auto m : party->members) {
        cache.insert({m, party});
      }
      return party;
    }
  
  void add_member_to_party(std::shared_ptr<Party> party, uint32_t member) {
    party->members.push_back(member);
    cache.insert({member, party});
    
    cache_write_party_members(party);
  }
  
  void remove_member_from_party(std::shared_ptr<Party>, uint32_t member) {
    party->members.erase(std::remove(party->members.begin(), party->members.end(), member), party->members.end());
    cache.erase(member);
    
    if (party->members.empty()) {
      cache_remove_party(party);
    } else {
      cache_write_party_members(party);
    }
  }
  
  private:
    std::unordered_map<uint32_t, std::shared_ptr<Party>> cache;
};
