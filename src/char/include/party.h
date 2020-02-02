#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "connection.h"
#include "cli_party_req.h"
#include "cli_party_reply.h"
#include "cli_party_rule.h"

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
  
  void set_xp_per_player() { options |= RoseCommon::Packet::CliPartyRule::EXP_PER_PLAYER; }
  void unset_xp_per_player() { options &= ~RoseCommon::Packet::CliPartyRule::EXP_PER_PLAYER; }
  bool is_xp_per_player() const noexcept { return options & RoseCommon::Packet::CliPartyRule::EXP_PER_PLAYER; }
  void set_item_in_order() { options |= RoseCommon::Packet::CliPartyRule::ITEM_IN_ORDER; }
  void unset_item_in_order() { options &= ~RoseCommon::Packet::CliPartyRule::ITEM_IN_ORDER; }
  bool is_item_in_order() const noexcept { return options & RoseCommon::Packet::CliPartyRule::ITEM_IN_ORDER; }
};

std::shared_ptr<Party> cache_fetch_party(uint32_t charId);
void cache_create_party(std::shared_ptr<Party> party);
void cache_write_party(std::shared_ptr<Party> party);
void cache_write_party_members(std::shared_ptr<Party> party);
void cache_remove_party(std::shared_ptr<Party> party);

class PartyManager {
  public:
    ~PartyManager();

    std::shared_ptr<Party> get_party(uint32_t charId);
    std::shared_ptr<Party> create_party(uint32_t charId);
    void add_member_to_party(std::shared_ptr<Party> party, uint32_t member);
    void remove_member_from_party(std::shared_ptr<Party> party, uint32_t member);
    void remove_party(std::shared_ptr<Party> party);
  
  private:
    std::unordered_map<uint32_t, std::shared_ptr<Party>> partys;
};

class CCharServer;
class User;
void party_request(const RoseCommon::Packet::CliPartyReq& packet, CCharServer& server, User& user);
void party_reply(const RoseCommon::Packet::CliPartyReply& packet, CCharServer& server, User& user);
