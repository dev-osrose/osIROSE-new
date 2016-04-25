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

/*!
 * \file packetclasses.h
 *
 * \author L3nn0x RavenX8
 * \date april 2016
 *
 * This file contains all packet subclasses and the logic needed
 */
#ifndef _PACKETCLASSES_H_
#define _PACKETCLASSES_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>
#include "iscpackets.pb.h"

#include "loginpackets.h"
#include "charpackets.h"

namespace RoseCommon {

//---------------------------------------------------
// Server Recv packets
//---------------------------------------------------
class CliAlive : public CRosePacket {
 public:
  CliAlive(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_ALIVE)
      throw std::runtime_error("Not the right packet!");
  }
  CliAlive() : CRosePacket(ePacketType::PAKCS_ALIVE) {}

  virtual ~CliAlive() {}
};

//------------------------------------------------

class CliAcceptReq : public CRosePacket {
 public:
  CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_ACCEPT_REQ)
      throw std::runtime_error("Not the right packet!");
  }
  CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

  virtual ~CliAcceptReq() {}
};

//------------------------------------------------

class CliScreenShotReq : public CRosePacket {
 public:
  CliScreenShotReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> count_;
  }
  CliScreenShotReq(uint16_t count = 1)
      : CRosePacket(ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ), count_(count) {}

  virtual ~CliScreenShotReq() {}

 private:
  uint16_t count_;
};

//------------------------------------------------

class CliJoinServerReq : public CRosePacket {
 public:
  CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_JOIN_SERVER_REQ)
      throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> session_id_ >> pass;
    password_ = std::string(pass, 32);
  }
  CliJoinServerReq(uint32_t id, const std::string &pass)
      : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ),
        session_id_(id),
        password_(pass) {}

  virtual ~CliJoinServerReq() {}

  std::string password() const { return password_; }
  uint32_t session_id() const { return session_id_; }

 protected:
  void pack() { *this << session_id_ << password_.c_str(); }

 private:
  uint32_t session_id_;
  std::string password_;
};

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

//-----------------------------------------------
// Send Packets
//-----------------------------------------------

class SrvAcceptClient : public CRosePacket {
 public:
  SrvAcceptClient(uint32_t rand_value, uint8_t result = 0x02)
      : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY),
        result_(result),
        rand_value_(rand_value) {}

 protected:
  void pack() { *this << result_ << rand_value_; }

 private:
  uint8_t result_;
  uint32_t rand_value_;
};

//------------------------------------------------

class SrvScreenShotReply : public CRosePacket {
 public:
  SrvScreenShotReply()
      : CRosePacket(ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY),
        year_(2016),
        month_(4),
        day_(20),
        hour_(5),
        min_(52) {}

 protected:
  void pack() { *this << year_ << month_ << day_ << hour_ << min_; }

 private:
  uint16_t year_;
  uint8_t month_;
  uint8_t day_;
  uint8_t hour_;
  uint8_t min_;
};

//------------------------------------------------

class SrvJoinServerReply : public CRosePacket {
 public:
  SrvJoinServerReply(uint8_t result, uint32_t id, uint32_t pay_flag = 0)
      : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY),
        result_(result),
        id_(id),
        pay_flag_(pay_flag) {}

  virtual ~SrvJoinServerReply() {}

  uint8_t result() const { return result_; }
  uint32_t id() const { return id_; }
  uint32_t payflag() const { return pay_flag_; }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    TIME_OUT,
    INVALID_PASSWORD,
    ALREADY_LOGGEDIN
  };

 protected:
  void pack() { *this << result_ << id_ << pay_flag_; }

 private:
  uint8_t result_;
  uint32_t id_;
  uint32_t pay_flag_;
};

//-------------------------------------------

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

#define MIN_SELL_TYPE 1
#define MAX_SELL_TYPE 11
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

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
class IscServerRegister : public CRosePacket {
 public:
  IscServerRegister(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::ISC_SERVER_REGISTER)
      throw std::runtime_error("Not the right packet!");
    uint32_t _size = size();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    for (uint32_t i = 0; i < _size; ++i) *this >> data[i];

    if (server_reg_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");
  }

  IscServerRegister(const std::string &name, const std::string &ip, int32_t id,
                    int32_t port, int32_t type, int32_t right)
      : CRosePacket(ePacketType::ISC_SERVER_REGISTER) {
    server_reg_.set_name(name);
    server_reg_.set_addr(ip);
    server_reg_.set_port(port);
    server_reg_.set_type((iscPacket::ServerType)type);
    server_reg_.set_accright( right );
    server_reg_.set_id( id );
  }

  iscPacket::ServerReg server_reg() const { return server_reg_; }

 protected:
  void pack() {
    int _size = server_reg_.ByteSize();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    if (server_reg_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");

    for (int i = 0; i < _size; ++i) *this << data[i];
  }

 private:
  iscPacket::ServerReg server_reg_;
};

//------------------------------------------------

class IscServerShutdown : public CRosePacket {
 public:
  IscServerShutdown(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::ISC_SHUTDOWN)
      throw std::runtime_error("Not the right packet!");
    uint32_t _size = size();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    for (uint32_t i = 0; i < _size; ++i) *this >> data[i];

    if (server_shutdown_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");
  }

  IscServerShutdown(int32_t id, int32_t type = 0)
      : CRosePacket(ePacketType::ISC_SHUTDOWN) {
    server_shutdown_.set_id(id);
    server_shutdown_.set_type((iscPacket::ServerType)type);
  }

  iscPacket::ServerShutdown server_reg() const { return server_shutdown_; }

 protected:
  void pack() {
    int _size = server_shutdown_.ByteSize();
    auto data = std::unique_ptr<uint8_t[]>(new uint8_t[_size]);
    memset(data.get(), 0, _size);
    if (server_shutdown_.SerializeToArray(data.get(), _size) == false)
      throw std::runtime_error("Couldn't serialize the data");

    for (int i = 0; i < _size; ++i) *this << data[i];
  }

 private:
  iscPacket::ServerShutdown server_shutdown_;
};

//------------------------------------------------
}

#endif /* !_PACKETCLASSES_H_ */
