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

class CliLogoutReq : public CRosePacket {
 public:
  CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliLogoutReq();

  virtual ~CliLogoutReq();
};

//------------------------------------------------

class SrvLogoutReply : public CRosePacket {
 public:
  SrvLogoutReply(uint16_t wait_time);

  virtual ~SrvLogoutReply();

  uint16_t wait_time() const;

 protected:
  void pack();

 private:
  uint16_t wait_time_;
};

//------------------------------------------------
//------------------------------------------------

class CliChangeMapReq : public CRosePacket {
 public:
  CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliChangeMapReq();

  virtual ~CliChangeMapReq();

  uint8_t weight_rate() const;
  uint16_t position_z() const;

 protected:
  void pack();

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
                    uint16_t team_number);

  virtual ~SrvChangeMapReply();

  uint16_t object_index() const;
  uint16_t current_hp() const;
  uint16_t current_mp() const;

  void setItemRate(uint8_t type, uint8_t rate);

 protected:
  void pack();

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
  CliChat(uint8_t buffer[MAX_PACKET_SIZE]);
  CliChat(const std::string &chat = "");

  std::string getMessage() const;

 protected:
  void pack();

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
  CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliReviveReq(uint8_t type = 0);

  enum eType : uint8_t { REVIVE_POS, SAVE_POS, START_POS, CURRENT_POS };

  eType getType() const;

 protected:
  void pack();

 private:
  uint8_t type_;
};

//------------------------------------------------

class SrvReviveReply : public CRosePacket {
 public:
  SrvReviveReply(uint16_t mapid = 0);

 protected:
  void pack();

 private:
  uint16_t mapid_;
};

//------------------------------------------------
//------------------------------------------------

class SrvInitDataReply : public CRosePacket {
 public:
  SrvInitDataReply(uint32_t rand_seed, uint16_t rand_index);

  virtual ~SrvInitDataReply();

  uint32_t rand_seed() const;
  uint16_t rand_index() const;

 protected:
  void pack();

 private:
  uint32_t rand_seed_;
  uint16_t rand_index_;
};

//------------------------------------------------
//------------------------------------------------

class SrvServerData : public CRosePacket {
 public:
  SrvServerData(uint8_t type);

  virtual ~SrvServerData();

  uint8_t type() const;

  enum data_type : uint8_t { ECONOMY = 0, NPC };

 protected:
  void pack();

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

class SrvRemoveObject : public CRosePacket {
 public:
  SrvRemoveObject(uint16_t obj_id);

 protected:
  void pack();

 private:
  uint16_t obj_id_;
};

//------------------------------------------------
//------------------------------------------------
#define MAX_UNION_COUNT 10
#define MAX_BUFF_STATUS 40
#define MAX_SKILL_COUNT 120
#define MAX_HOTBAR_ITEMS 32

PACK(class SrvSelectCharReply : public CRosePacket {
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

  struct equip_item : public ISerialize {
    union {
      struct {
        unsigned int id_ : 10;     // 0~1023
        unsigned int gem_op_ : 9;  // 0~512
        unsigned int socket_ : 1;  // 0~1
        unsigned int grade_ : 4;   // 0~15
      };
      unsigned char data[4];
    };

    equip_item(uint16_t id = 0, uint16_t gem = 0, uint8_t socket = 0,
               uint8_t grade = 0)
               : id_(id), gem_op_(gem), socket_(socket), grade_(grade) {}
  protected:
    virtual void serialize( CRosePacket &os ) const override;
    virtual void deserialize( CRosePacket &os ) override;
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
    
    status_effects() : expired_seconds_(0), value_(0), unknown_(0) {}

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
    
    extended_stats() : hp_(100), mp_(100), level_(1), stat_points_(0), 
                       skill_points_(0), body_size_(1), head_size_(1),
                       exp_(0), penalty_exp_(0), fame1_(0), fame2_(0),
                       union_points_(), guild_id_(0), guild_position_(0),
                       pk_flags_(0), stamina_(1000), status_(), pat_hp_(100),
                       pat_cooldown_time_(0) {
      for (int i = 0; i < MAX_UNION_COUNT; ++i)
      {
        union_points_[i] = 0;
      }
      for (int i = 0; i < MAX_BUFF_STATUS; ++i)
      {
        status_[i] = status_effects();
      }
    }

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct skills : public ISerialize {
    uint16_t skill_id_[MAX_SKILL_COUNT];

    skills() {
      for (int i = 0; i < MAX_SKILL_COUNT; ++i)
      {
        skill_id_[i] = 0;
      }
    }

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
      unsigned short item_;
    };
    
    hotbar_item() : item_(0) {}

   protected:
    virtual void serialize(CRosePacket &os) const override;
    virtual void deserialize(CRosePacket &os) override;
  };

  struct hotbar : public ISerialize {
    hotbar_item list_[MAX_HOTBAR_ITEMS];

    hotbar() {
      for (int i = 0; i < MAX_HOTBAR_ITEMS; ++i)
      {
        list_[i] = hotbar_item();
      }
    }

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

class SrvInventoryData : public CRosePacket {
 public:
  SrvInventoryData(int64_t zuly)
      : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), zuly_(zuly){};

 protected:
  void pack() { 
    *this << zuly_; 
    for (int i = 0; i < 140; ++i) {
      *this << (uint16_t)0 << (uint32_t)0;
    }
  };

 private:
  int64_t zuly_;
  //todo: item list goes here
};

//------------------------------------------------
//------------------------------------------------

class SrvQuestData : public CRosePacket {
 public:
  SrvQuestData() : CRosePacket(ePacketType::PAKWC_QUEST_DATA){};

 protected:
  void pack() {
    //Quest data
    for (int i = 0; i < 5; ++i)
    {
      *this << (uint16_t)0;
    }
    for (int i = 0; i < 3; ++i)
    {
      *this << (uint16_t)0;
    }
    for (int i = 0; i < 7; ++i)
    {
      *this << (uint16_t)0;
    }
    for (int i = 0; i < 10; ++i)
    {
      *this << (uint16_t)0;
    }

    for (int i = 0; i < 10; ++i)
    {
      *this << (uint16_t)0 << (uint32_t)0;

      for (int i = 0; i < 10; ++i)
      {
        *this << (uint16_t)0;
      }
      *this << (uint32_t)0;

      for (int i = 0; i < 6; ++i)
      {
        *this << (uint16_t)0 << (uint32_t)0;
      }
    }

    for (int i = 0; i < 16; ++i)
    {
      *this << (uint32_t)0;
    }

    // Wish list
    for (int i = 0; i < 30; ++i)
    {
      *this << (uint16_t)0 << (uint32_t)0;
    }
  };

  //Quest data
  //Wish List
};)

//------------------------------------------------
//------------------------------------------------

class SrvBillingMsg : public CRosePacket {
 public:
  SrvBillingMsg() : CRosePacket(ePacketType::PAKWC_BILLING_MESSAGE){};

 protected:
  void pack() { *this << (uint16_t)0x1001 << (uint32_t)2; };
};
}

#endif