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

#ifndef _CHARPACKETS_H_
#define _CHARPACKETS_H_

#include "epackettype.h"
#include "crosepacket.h"
#include <string>
#include <exception>
#include <vector>

namespace RoseCommon {

//------------------------------------------------
//------------------------------------------------
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

//------------------------------------------------

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

//------------------------------------------------
//------------------------------------------------
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

//------------------------------------------------

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

//------------------------------------------------
//------------------------------------------------
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

//------------------------------------------------

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

//------------------------------------------------
//------------------------------------------------
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

}

#endif