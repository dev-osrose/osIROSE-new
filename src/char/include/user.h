#pragma once

#include <memory>
#include "party.h"
#include "isc_client_status.h"

class User {
  public:
    User(uint32_t charId) : charId(charId) {}
    
    enum class Status {
      CONNECTING,
      CONNECTED,
      DISCONNECTED,
      SWITCHING,
      AFK,
      INVISIBLE
    };
  
  uint32_t get_charId() const noexcept { return charId; }
  
  Status get_status() const noexcept { return status; }
  void set_status(Status status) noexcept {
    this->status = status;
  }
  void set_status(RoseCommon::Packet::IscClientStatus::Status status) noexcept {
    switch (status) {
      case RoseCommon::Packet::IscClientStatus::Status::CONNECTED:
        this->status = Status::CONNECTED;
        break;
      case RoseCommon::Packet::IscClientStatus::Status::DISCONNECTED:
        this->status = Status::DISCONNECTED;
        break;
      case RoseCommon::Packet::IscClientStatus::Status::SWITCHING:
        this->status = Status::SWITCHING;
        break;
      case RoseCommon::Packet::IscClientStatus::Status::AFK:
        this->status = Status::AFK;
        break;
      case RoseCommon::Packet::IscClientStatus::Status::INVISIBLE:
        this->status = Status::INVISIBLE;
        break;
    }
  }
  
  std::shared_ptr<RoseCommon::PartyBase> get_party() const noexcept { return party; }
  void set_party(std::shared_ptr<Party> party) noexcept { this->party = party; }

  private:
    uint32_t charId;
    Status status = Status::CONNECTING;
    std::shared_ptr<Party> party;
    // std::shared_ptr<Guild>
    // std::vector<Chatroom>
    // whatever else
};
