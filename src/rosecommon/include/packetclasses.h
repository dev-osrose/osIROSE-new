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

class CliAcceptReq : public CRosePacket {
 public:
  CliAcceptReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_ACCEPT_REQ)
      throw std::runtime_error("Not the right packet!");
  }
  CliAcceptReq() : CRosePacket(ePacketType::PAKCS_ACCEPT_REQ) {}

  virtual ~CliAcceptReq() {}
};

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

class CliLoginReq : public CRosePacket {
 public:
  CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_LOGIN_REQ)
      throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> pass >> username_;
    password_ = std::string(pass, 32);
  }
  CliLoginReq(const std::string &user, const std::string &pass)
      : CRosePacket(ePacketType::PAKCS_LOGIN_REQ),
        password_(pass),
        username_(user) {}

  virtual ~CliLoginReq() {}

  std::string password() const { return password_; }
  std::string username() const { return username_; }

 protected:
  void pack() { *this << password_.c_str() << username_; }

 private:
  std::string password_;
  std::string username_;
};

class CliChannelReq : public CRosePacket {
 public:
  CliChannelReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_CHANNEL_LIST_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> server_id_;
  }
  CliChannelReq(uint32_t server_id)
      : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ),
        server_id_(server_id) {}

  virtual ~CliChannelReq() {}

  uint32_t server_id() const { return server_id_; }

 protected:
  void pack() { *this << server_id_; }

 private:
  uint32_t server_id_;
};

class CliServerSelectReq : public CRosePacket {
 public:
  CliServerSelectReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SRV_SELECT_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> server_id_ >> channel_id_;
  }

  CliServerSelectReq(uint32_t server_id, uint8_t channel_id)
      : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ),
        server_id_(server_id),
        channel_id_(channel_id) {}

  virtual ~CliServerSelectReq() {}

  uint32_t server_id() const { return server_id_; }
  uint8_t channel_id() const { return channel_id_; }

 protected:
  void pack() { *this << server_id_ << channel_id_; }

 private:
  uint32_t server_id_;
  uint8_t channel_id_;
};

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

class CliCreateCharReq : public CRosePacket {
 public:
  CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_CREATE_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> race_ >> stone_ >> hair_ >> face_ >> weapon_ >> zone_ >> name_;
  }
  CliCreateCharReq(const std::string &name, uint8_t race, uint8_t stone,
                   uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone)
      : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ),
        race_(race),
        stone_(stone),
        hair_(hair),
        face_(face),
        weapon_(weapon),
        zone_(zone),
        name_(name) {}

  virtual ~CliCreateCharReq() {}

  std::string name() const { return name_; }
  uint8_t race() const { return race_; }
  uint8_t stone() const { return stone_; }
  uint8_t hair() const { return hair_; }
  uint8_t face() const { return face_; }
  uint8_t weapon() const { return weapon_; }
  uint16_t zone() const { return zone_; }

 protected:
  void pack() {
    *this << race_ << stone_ << hair_ << face_ << weapon_ << zone_ << name_;
  }

 private:
  uint8_t race_;
  uint8_t stone_;
  uint8_t hair_;
  uint8_t face_;
  uint8_t weapon_;
  uint16_t zone_;
  std::string name_;
};

class CliDeleteCharReq : public CRosePacket {
 public:
  CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_DELETE_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> char_id_ >> delete_ >> name_;
  }
  CliDeleteCharReq(const std::string &name, uint8_t id, uint8_t del)
      : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ),
        char_id_(id),
        delete_(del),
        name_(name) {}

  virtual ~CliDeleteCharReq() {}

  uint8_t char_id() const { return char_id_; }
  bool isDelete() const { return (delete_ != 0) ? true : false; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << char_id_ << delete_; }

 private:
  uint8_t char_id_;
  uint8_t delete_;
  std::string name_;
};

class CliSelectCharReq : public CRosePacket {
 public:
  CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_SELECT_CHAR_REQ)
      throw std::runtime_error("Not the right packet!");
    *this >> char_id_ >> run_mode_ >> ride_mode_ >> name_;
  }
  CliSelectCharReq(const std::string &name, uint8_t id, uint8_t run,
                   uint8_t ride)
      : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ),
        char_id_(id),
        run_mode_(run),
        ride_mode_(ride),
        name_(name) {}

  virtual ~CliSelectCharReq() {}

  uint8_t char_id() const { return char_id_; }
  uint8_t run_mode() const { return run_mode_; }
  uint8_t ride_mode() const { return ride_mode_; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << char_id_ << run_mode_ << ride_mode_ << name_; }

 private:
  uint8_t char_id_;
  uint8_t run_mode_;
  uint8_t ride_mode_;
  std::string name_;
};

class CliLogoutReq : public CRosePacket {
 public:
  CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    if (type() != ePacketType::PAKCS_LOGOUT_REQ)
      throw std::runtime_error("Not the right packet!");
  }
  CliLogoutReq() : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

  virtual ~CliLogoutReq() {}
};

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

class SrvLoginReply : public CRosePacket {
 public:
  SrvLoginReply(uint8_t result, uint16_t right, uint16_t type)
      : CRosePacket(ePacketType::PAKLC_LOGIN_REPLY),
        right_(right),
        type_(type),
        result_(result) {}

  virtual ~SrvLoginReply() {}

  uint8_t &result() { return result_; }
  uint16_t &right() { return right_; }
  uint16_t &type() { return type_; }

  void setRight(uint16_t right) { right_ = right; }
  void addServer(const std::string &name, uint32_t id, bool isTest = false) {
    info channel(name, id, isTest);
    channel_list_.push_back(channel);
  }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    UNKNOWN_ACCOUNT,
    INVALID_PASSWORD,
    ALREADY_LOGGEDIN,
    REFUSED_ACCOUNT,
    NEED_CHARGE,
    NO_RIGHT_TO_CONNECT,
    TOO_MANY_USERS,
    NO_NAME,
    INVALID_VERSION,
    OUTSIDE_REGION
  };

 protected:
  void pack() {
    *this << result_ << right_ << type_;

    for (auto &server : channel_list_) {
      char pad = ' ';
      if (server.test_ == true) pad = '@';
      *this << pad << server.name_ << server.channel_id_;
    }
  }

 private:
  uint16_t right_;
  uint16_t type_;
  uint8_t result_;

  struct info {
    std::string name_;
    uint32_t channel_id_;
    bool test_;

    info(const std::string &name, uint32_t id, bool isTest = false)
        : name_(name), channel_id_(id), test_(isTest) {}
  };

  std::vector<info> channel_list_;
};

class SrvChannelReply : public CRosePacket {
 public:
  SrvChannelReply(uint32_t serverid)
      : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY),
        server_id_(serverid),
        channel_count_(0) {}

  virtual ~SrvChannelReply() {}

  uint8_t server_id() const { return server_id_; }
  uint16_t channel_count() const { return channel_count_; }

  void addChannel(const std::string &name, uint8_t id,
                  uint16_t user_capacity_percentage, uint8_t low_age = 0,
                  uint8_t high_age = 0) {
    ++channel_count_;
    info channel(name, id, user_capacity_percentage, low_age, high_age);
    channel_list_.push_back(channel);
  }

 protected:
  void pack() {
    *this << server_id_ << channel_count_;

    for (auto &server : channel_list_)
      *this << server.channel_id_ << server.low_age_ << server.high_age_
            << server.capacity_ << server.name_;
  }

 private:
  uint32_t server_id_;
  uint8_t channel_count_;

  struct info {
    std::string name_;
    uint8_t channel_id_;
    uint8_t low_age_;
    uint8_t high_age_;
    uint16_t capacity_;

    info(const std::string &name, uint8_t id, uint16_t user_capacity_percentage,
         uint8_t low_age = 0, uint8_t high_age = 0)
        : name_(name),
          channel_id_(id),
          low_age_(low_age),
          high_age_(high_age),
          capacity_(user_capacity_percentage) {}
  };

  std::vector<info> channel_list_;
};

class SrvServerSelectReply : public CRosePacket {
 public:
  SrvServerSelectReply(const std::string &ip, uint32_t session_id,
                       uint32_t crypt_val, uint16_t port)
      : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY),
        session_id_(session_id),
        crypt_val_(crypt_val),
        port_(port),
        result_(0),
        ip_(ip) {}

  virtual ~SrvServerSelectReply() {}

  uint32_t session_id() const { return session_id_; }
  uint32_t crypt_val() const { return crypt_val_; }
  uint16_t port() const { return port_; }
  uint8_t result() const { return result_; }
  std::string ip() const { return ip_; }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    FULL,
    INVALID_CHANNEL,
    CHANNEL_NOT_ACTIVE,
    INVALID_AGE
  };

 protected:
  void pack() { *this << result_ << session_id_ << crypt_val_ << ip_ << port_; }

 private:
  uint32_t session_id_;
  uint32_t crypt_val_;
  uint16_t port_;
  uint8_t result_;
  std::string ip_;
};

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

class SrvCharacterListReply : public CRosePacket {
 public:
  SrvCharacterListReply()
      : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY), character_count_(0) {}

  virtual ~SrvCharacterListReply() {}

  void addCharacter(const std::string &name, uint8_t race, uint16_t level,
                    uint16_t job, uint32_t delete_time = 0) {
    ++character_count_;
    char_info character(name, race, level, job, delete_time);
    character_list_.push_back(character);

    for (int idx = 0; idx < MAX_EQUIPPED_ITEMS; ++idx)
      addEquipItem(character_count_ - 1, idx);
  }

  void addEquipItem(uint8_t char_id, uint8_t slot, uint16_t item_id = 0,
                    uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0) {
    if (char_id < character_count_ && slot < MAX_EQUIPPED_ITEMS) {
      equip_item item = character_list_[char_id].items_[slot];
      item.id_ = item_id;
      item.gem_op_ = gem;
      item.socket_ = socket;
      item.grade_ = grade;
      character_list_[char_id].items_[slot] = item;
    }
  }

  enum equipped_position {
    EQUIP_FACE = 0,
    EQUIP_HAIR = 1,
    EQUIP_HELMET = 2,
    EQUIP_ARMOR = 3,
    EQUIP_GAUNTLET = 4,
    EQUIP_BOOTS = 5,
    EQUIP_GOGGLES = 6,
    EQUIP_FACE_ITEM = EQUIP_GOGGLES,
    EQUIP_BACKPACK = 7,
    EQUIP_WEAPON_R = 8,
    EQUIP_WEAPON_L = 9,
    MAX_EQUIPPED_ITEMS
  };

 protected:
  void pack() {
    *this << character_count_;

    for (auto &character : character_list_) {
      *this << character.name_;
      *this << character.race_ << character.level_ << character.job_
            << character.remain_sec_unitl_delete_ << character.platinum_;

      for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
        for (int j = 0; j < 4; ++j) *this << character.items_[i].data[j];
      }
    }
  }

 private:
  uint8_t character_count_;

  struct equip_item {
    union {
      struct {
        unsigned int id_ : 10;     // 0~1023
        unsigned int gem_op_ : 9;  // 0~512
        unsigned int socket_ : 1;  // 0~1
        unsigned int grade_ : 4;   // 0~15
      };
      uint8_t data[4];
    };

    equip_item(uint16_t id = 0, uint16_t gem = 0, uint8_t socket = 0,
               uint8_t grade = 0)
        : id_(id), gem_op_(gem), socket_(socket), grade_(grade) {}
  };

  struct char_info {
    uint32_t remain_sec_unitl_delete_;
    uint16_t level_;
    uint16_t job_;
    uint8_t race_;
    uint8_t platinum_;
    equip_item items_[MAX_EQUIPPED_ITEMS];
    std::string name_;

    char_info(const std::string &name, uint8_t race = 0, uint16_t level = 0,
              uint16_t job = 0, uint32_t delete_time = 0, uint8_t platinum = 0)
        : remain_sec_unitl_delete_(delete_time),
          level_(level),
          job_(job),
          race_(race),
          platinum_(platinum),
          name_(name) {}
  };
  std::vector<char_info> character_list_;
};

class SrvCreateCharReply : public CRosePacket {
 public:
  SrvCreateCharReply(uint8_t result, uint8_t platinum = 0)
      : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY),
        result_(result),
        platinum_(platinum) {}

  virtual ~SrvCreateCharReply() {}

  uint8_t result() const { return result_; }
  uint8_t platinum() const { return platinum_; }

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    NAME_TAKEN,
    INVALID_VALUE,
    TOO_MANY_CHARS,
    BLOCKED
  };

 protected:
  void pack() { *this << result_ << platinum_; }

 private:
  uint8_t result_;
  uint8_t platinum_;
};

class SrvDeleteCharReply : public CRosePacket {
 public:
  SrvDeleteCharReply(const std::string &name, uint32_t remaining_time)
      : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY),
        remaining_time_(remaining_time),
        name_(name) {}

  virtual ~SrvDeleteCharReply() {}

  uint32_t remaining_time() const { return remaining_time_; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << remaining_time_ << name_; }

 private:
  uint32_t remaining_time_;
  std::string name_;
};

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

class SrvSelectCharReply : public CRosePacket {
 public:
  SrvSelectCharReply(const std::string &name, uint32_t remaining_time)
      : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY),
        remaining_time_(remaining_time),
        name_(name) {}

  virtual ~SrvSelectCharReply() {}

  uint32_t remaining_time() const { return remaining_time_; }
  std::string name() const { return name_; }

 protected:
  void pack() { *this << remaining_time_ << name_; }

 private:
  uint32_t remaining_time_;
  std::string name_;
};

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
}

#endif /* !_PACKETCLASSES_H_ */
