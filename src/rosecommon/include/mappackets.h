// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _MAPPACKETS_H_
#define _MAPPACKETS_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>

namespace RoseCommon {
#define MIN_SELL_TYPE 1
#define MAX_SELL_TYPE 11

//------------------------------------------------
//------------------------------------------------
  
class SrvSwitchServerReply : public CRosePacket {
 public:
  SrvSwitchServerReply(const std::string &ip, uint16_t port, uint32_t session_id, uint32_t random_seed)
      : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER),
        port_(port),
        ip_(ip) {
    session_ids_[0] = session_id;
    session_ids_[1] = random_seed;
  }

  virtual ~SrvSwitchServerReply() {}

  uint32_t port() const { return port_; }
  uint32_t session_id() const { return session_ids_[0]; }
  uint32_t random_seed() const { return session_ids_[1]; }
  std::string ip() const { return ip_; }

 protected:
  void pack() { *this << port_ << session_ids_[0] << session_ids_[1] << ip_; }

 private:
  uint16_t port_;
  std::string ip_;
  uint32_t session_ids_[2];
};

//------------------------------------------------
//------------------------------------------------

class CliLogoutReq : public CRosePacket {
 public:
  CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_LOGOUT_REQ)
      throw std::runtime_error("Not the right packet!");
  }
  CliLogoutReq() : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

  virtual ~CliLogoutReq() {}
};

//------------------------------------------------

class SrvLogoutReply : public CRosePacket {
 public:
  SrvLogoutReply(uint16_t wait_time)
      : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY), wait_time_(wait_time) {}

  virtual ~SrvLogoutReply() {}

  uint16_t wait_time() const { return wait_time_; }

 protected:
  void pack() { *this << wait_time_; }

 private:
  uint16_t wait_time_;
};

//------------------------------------------------
//------------------------------------------------

class CliChangeMapReq : public CRosePacket {
 public:
  CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_CHANGE_MAP_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> weight_rate_ >> position_z_;
  }
  CliChangeMapReq() : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ) {}

  virtual ~CliChangeMapReq() {}

  uint8_t weight_rate() const { return weight_rate_; }
  uint16_t position_z() const { return position_z_; }

 protected:
  void pack() { *this << weight_rate_ << position_z_; }

 private:
  uint8_t weight_rate_;
  uint16_t position_z_;  // this is not actually set
};

//------------------------------------------------

class SrvChangeMapReply : public CRosePacket {
 public:
  SrvChangeMapReply(uint16_t object_index, uint16_t current_hp,
                    uint16_t current_mp, uint16_t current_exp,
                    uint16_t penalize_exp, uint16_t world_time,
                    uint16_t team_number)
      : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY),
        object_index_(object_index),
        current_hp_(current_hp),
        current_mp_(current_mp),
        current_exp_(current_exp),
        penalize_exp_(penalize_exp),
        world_time_(world_time),
        team_number_(team_number) {
    for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
      zone_vars_.item_rate_[idx] = 0;
    }
  }

  virtual ~SrvChangeMapReply() {}

  uint16_t object_index() const { return object_index_; }
  uint16_t current_hp() const { return current_hp_; }
  uint16_t current_mp() const { return current_mp_; }

  void setItemRate(uint8_t type, uint8_t rate) {
    zone_vars_.item_rate_[type] = rate;
  }

 protected:
  void pack() {
    *this << object_index_ << current_hp_ << current_mp_ << current_exp_
          << penalize_exp_;
    {  // global_var
      *this << zone_vars_.craft_rate_ << zone_vars_.update_time_
            << zone_vars_.world_rate_ << zone_vars_.town_rate_;
      for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
        *this << zone_vars_.item_rate_[idx];
      }
      *this << zone_vars_.flags_;
    }  // global_var
    *this << world_time_ << team_number_;
  }

 private:
  struct global_var {
    uint16_t craft_rate_;
    uint32_t update_time_;
    uint16_t world_rate_;
    uint8_t town_rate_;
    uint8_t item_rate_[MAX_SELL_TYPE];
    uint32_t flags_;
  };

  uint16_t object_index_;
  uint16_t current_hp_;
  uint16_t current_mp_;
  uint64_t current_exp_;
  uint64_t penalize_exp_;
  uint32_t world_time_;
  uint32_t team_number_;
  global_var zone_vars_;
};

//------------------------------------------------
//------------------------------------------------

class CliChat : public CRosePacket {
 public:
  CliChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_NORMAL_CHAT)
      throw std::runtime_error("Not the right packet!");
    *this >> chat_;
  }
  CliChat(const std::string &chat = "")
      : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT),
        chat_(chat) {}
        
  std::string getMessage() const { return chat_; }

 protected:
  void pack() { *this << chat_; }

 private:
  std::string chat_;
};

//------------------------------------------------

class SrvChat : public CRosePacket {
 public:
  SrvChat(const std::string &chat = "", uint16_t charuid = 0)
      : CRosePacket(ePacketType::PAKWC_NORMAL_CHAT),
        chat_(chat),
        charuid_(charuid) {}

 protected:
  void pack() { *this << charuid_ << chat_; }

 private:
  std::string chat_;
  uint16_t charuid_;
};

//------------------------------------------------
//------------------------------------------------

class CliReviveReq : public CRosePacket {
 public:
  CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_REVIVE_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> type_;
  }
  CliReviveReq(uint8_t type = 0)
      : CRosePacket(ePacketType::PAKCS_REVIVE_REQ),
       type_(type) {}
       
  enum eType : uint8_t {
    REVIVE_POS,
    SAVE_POS,
    START_POS,
    CURRENT_POS
  };
  
  eType getType() const { return (eType)type_; }

 protected:
  void pack() { *this << type_; }

 private:
  uint8_t type_;
};

//------------------------------------------------

class SrvReviveReply : public CRosePacket {
 public:
  SrvReviveReply(uint16_t mapid = 0)
      : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY),
       mapid_(mapid) {}

 protected:
  void pack() { *this << mapid_; }

 private:
  uint16_t mapid_;
};

//------------------------------------------------
//------------------------------------------------

class SrvInitDataReply : public CRosePacket {
 public:
  SrvInitDataReply(uint32_t rand_seed, uint16_t rand_index)
      : CRosePacket(ePacketType::PAKWC_INIT_DATA),
        rand_seed_(rand_seed),
        rand_index_(rand_index) {}

  virtual ~SrvInitDataReply() {}

  uint32_t rand_seed() const { return rand_seed_; }
  uint16_t rand_index() const { return rand_index_; }

 protected:
  void pack() { *this << rand_seed_ << rand_index_; }

 private:
  uint32_t rand_seed_;
  uint16_t rand_index_;
};

//------------------------------------------------
//------------------------------------------------

class SrvServerData : public CRosePacket {
 public:
  SrvServerData(uint8_t type)
      : CRosePacket(ePacketType::PAKWC_INIT_DATA), type_(type) {}

  virtual ~SrvServerData() {}

  uint8_t type() const { return type_; }

  enum data_type : uint8_t { ECONOMY = 0, NPC };

 protected:
  void pack() {
    *this << type_;

    switch (type_) {
      case data_type::ECONOMY: {
        *this << enconmy_data_.counter_ << enconmy_data_.pop_base_
              << enconmy_data_.dev_base_;
        for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
          *this << enconmy_data_.consume_[idx];
        }
        *this << enconmy_data_.dev_ << enconmy_data_.pop_;
        for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
          *this << enconmy_data_.item_[idx];
        }
        break;
      }
      case data_type::NPC:
      default:
        break;
    }
  }

 private:
  struct Enconmy_Data {
    uint32_t counter_;
    uint16_t pop_base_;
    uint16_t dev_base_;
    uint16_t consume_[MAX_SELL_TYPE];
    uint16_t dev_;
    uint32_t pop_;
    uint32_t item_[MAX_SELL_TYPE];
  };
  uint8_t type_;
  Enconmy_Data enconmy_data_;
};

//------------------------------------------------
//------------------------------------------------
}

#endif