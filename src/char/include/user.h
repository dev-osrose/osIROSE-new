#pragma once

#include <memory>
#include <string>
#include "party.h"
#include "isc_client_status.h"

class CCharClient;

class User {
  public:
    User(std::weak_ptr<CCharClient> client, const std::string& name, uint32_t charId, uint16_t mapId) :
        client(client), name(name), charId(charId), mapId(mapId) {}
    
    enum class Status {
      CONNECTING,
      CONNECTED,
      DISCONNECTED,
      SWITCHING,
      AFK,
      INVISIBLE
    };
  
  uint32_t get_charId() const noexcept { return charId; }
  uint16_t get_mapId() const noexcept { return mapId; }

  void set_mapId(uint16_t mapId) { this->mapId = mapId; }
  
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

  const std::weak_ptr<CCharClient> get_client() const noexcept { return client; }

  const std::string& get_name() const noexcept { return name; }
  
  std::shared_ptr<Party> get_party() const noexcept { return party; }
  void set_party(std::shared_ptr<Party> party) noexcept {
    this->requested_party.reset();
    this->party = party;
  }
  void set_requested_party(std::shared_ptr<Party> party) noexcept {
    this->requested_party = party;
    this->party.reset();
  }
  std::shared_ptr<Party> get_requested_party() const noexcept { return requested_party; }
  
  uint16_t get_entityId() const noexcept { return entityId; }
  void set_entityId(uint16_t entityId) { this->entityId = entityId; }

  private:
    std::weak_ptr<CCharClient> client;
    std::string name;
    uint32_t charId;
    uint16_t mapId;
    uint16_t entityId = 0;
    Status status = Status::CONNECTING;
    std::shared_ptr<Party> requested_party;
    std::shared_ptr<Party> party;
    // std::shared_ptr<Guild>
    // std::vector<Chatroom>
    // whatever else
};
