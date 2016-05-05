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
#include "iserialize.h"

namespace RoseCommon {

//------------------------------------------------
//------------------------------------------------
class CliCreateCharReq : public CRosePacket {
 public:
  CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliCreateCharReq(const std::string &name, uint8_t race, uint8_t stone,
                   uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone);

  virtual ~CliCreateCharReq();

  std::string name() const;
  uint8_t race() const;
  uint8_t stone() const;
  uint8_t hair() const;
  uint8_t face() const;
  uint8_t weapon() const;
  uint16_t zone() const;

 protected:
  void pack();

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
  SrvCreateCharReply(uint8_t result, uint8_t platinum = 0);

  virtual ~SrvCreateCharReply();

  uint8_t result() const;
  uint8_t platinum() const;

  enum eResult : uint8_t {
    OK = 0,
    FAILED,
    NAME_TAKEN,
    INVALID_VALUE,
    TOO_MANY_CHARS,
    BLOCKED
  };

 protected:
  void pack();

 private:
  uint8_t result_;
  uint8_t platinum_;
};

//------------------------------------------------
//------------------------------------------------
class CliDeleteCharReq : public CRosePacket {
 public:
  CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliDeleteCharReq(const std::string &name, uint8_t id, uint8_t del);

  virtual ~CliDeleteCharReq();

  uint8_t char_id() const;
  bool isDelete() const;
  std::string name() const;

 protected:
  void pack();

 private:
  uint8_t char_id_;
  uint8_t delete_;
  std::string name_;
};

//------------------------------------------------

class SrvDeleteCharReply : public CRosePacket {
 public:
  SrvDeleteCharReply(const std::string &name, uint32_t remaining_time);

  virtual ~SrvDeleteCharReply();

  uint32_t remaining_time() const;
  std::string name() const;

 protected:
  void pack();

 private:
  uint32_t remaining_time_;
  std::string name_;
};

//------------------------------------------------
//------------------------------------------------

class SrvCharacterListReply : public CRosePacket {
 public:
  SrvCharacterListReply();

  virtual ~SrvCharacterListReply();

  void addCharacter(const std::string &name, uint8_t race, uint16_t level,
                    uint16_t job, uint32_t delete_time = 0);

  void addEquipItem(uint8_t char_id, uint8_t slot, uint16_t item_id = 0,
                    uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0);

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
  void pack();

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
               uint8_t grade = 0);
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
              uint16_t job = 0, uint32_t delete_time = 0, uint8_t platinum = 0);
  };
  std::vector<char_info> character_list_;
};

//------------------------------------------------
//------------------------------------------------
class CliSelectCharReq : public CRosePacket {
 public:
  CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliSelectCharReq(const std::string &name, uint8_t id, uint8_t run,
                   uint8_t ride);

  virtual ~CliSelectCharReq();

  uint8_t char_id() const;
  uint8_t run_mode() const;
  uint8_t ride_mode() const;
  std::string name() const;

 protected:
  void pack();

 private:
  uint8_t char_id_;
  uint8_t run_mode_;
  uint8_t ride_mode_;
  std::string name_;
};

//------------------------------------------------
#define MAX_UNION_COUNT 10
#define MAX_BUFF_STATUS 40
#define MAX_SKILL_COUNT 120
#define MAX_HOTBAR_ITEMS 32

class SrvSelectCharReply : public CRosePacket {
 public:
  SrvSelectCharReply();

  virtual ~SrvSelectCharReply();

  void setCharacter(const std::string &name, uint8_t race, uint16_t zone,
                    float x, float y, uint16_t revive_zone, uint32_t utag);

  void addEquipItem(uint8_t slot, uint16_t item_id = 0,
                    uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0);

 protected:
  void pack();

 private:
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

  struct base_info : public ISerialize {
    uint16_t job_;
    uint8_t stone_;
    uint8_t face_;
    //------
    uint8_t hair_;
    uint8_t union_;
    uint8_t rank_;
    uint8_t fame_;

    base_info(uint8_t stone = 0, uint8_t face = 0, uint8_t hair = 0, uint16_t job = 0, uint8_t _union = 0, uint8_t rank = 0, uint8_t fame = 0)
        : job_(job),
          stone_(stone),
          face_(face),
          hair_(hair),
          union_(_union),
          rank_(rank),
          fame_(fame) {}

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct character_stats : public ISerialize {
    uint16_t str_;
    uint16_t dex_;
    uint16_t int_;
    uint16_t con_;
    uint16_t charm_;
    uint16_t sense_;

    character_stats(uint16_t str = 10, uint16_t dex = 10, uint16_t _int = 10,
                    uint16_t con = 10, uint16_t charm = 10, uint16_t sense = 10)
        : str_(str),
          dex_(dex),
          int_(_int),
          con_(con),
          charm_(charm),
          sense_(sense) {}

   protected:
    virtual void serialize(CRosePacket &os) const override;

    virtual void deserialize(CRosePacket &os) override;
  };

  struct status_effects : public ISerialize {
    uint32_t expired_seconds_;
    uint16_t value_;
    uint16_t unknown_;

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct extended_stats : public ISerialize {
    uint16_t hp_;
    uint16_t mp_;
    uint16_t level_;
    uint16_t stat_points_;
    uint16_t skill_points_;
    uint8_t body_size_;
    uint8_t head_size_;
    uint64_t exp_;
    uint64_t penalty_exp_;

    uint16_t fame1_;
    uint16_t fame2_;
    uint16_t union_points_[MAX_UNION_COUNT];

    uint32_t guild_id_;
    uint16_t guild_contribution_;
    uint8_t guild_position_;

    uint16_t pk_flags_;
    uint16_t stamina_;

    status_effects status_[MAX_BUFF_STATUS];

    uint16_t pat_hp_;
    uint32_t pat_cooldown_time_;

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct skills : public ISerialize {
    uint16_t skill_id_[MAX_SKILL_COUNT] {};

    skills() {}

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  enum hotbar_item_type {
    inv_item = 1,
    command_item,
    skill_item,
    emote_item,
    dialog_item,
    clanskill_item,
  };

  struct hotbar_item : public ISerialize {
    union {
      struct {
        unsigned short type_ : 5;
        unsigned short slot_id_ : 11;
      };
      uint16_t item_;
    };
    
    hotbar_item() : item_(0) {}

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct hotbar : public ISerialize {
    hotbar_item list_[MAX_HOTBAR_ITEMS] {};

    hotbar() { }

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  uint8_t race_;
  uint16_t zone_;
  uint16_t revive_zone_;
  float position_start_[2];  // x and y;

  equip_item items_[MAX_EQUIPPED_ITEMS];
  base_info base_character_info_;
  character_stats stats_;
  extended_stats ext_stats_;
  skills learned_skills_;
  hotbar hotbar_;

  uint32_t unique_tag_;
  std::string name_;
};

//------------------------------------------------
//------------------------------------------------
}

#endif