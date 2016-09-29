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
#include "entityComponents.h"

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

//-------------------------------------------------

class CliMouseCmd : public CRosePacket {
    public:
        CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]);
        CliMouseCmd();

        virtual ~CliMouseCmd();

        uint16_t targetId() const { return targetObjId_; }
        int32_t x() const { return destX_; }
        int32_t y() const { return destY_; }
        int16_t z() const { return posZ_; }

    private:
        uint16_t targetObjId_;
        int32_t destX_;
        int32_t destY_;
        int16_t posZ_;
};

class SrvMouseCmd : public CRosePacket {
    public:
        SrvMouseCmd(uint16_t sourceObjId, uint16_t destObjId, uint16_t srvDist,
                int32_t destX, int32_t destY, int16_t posZ);
        SrvMouseCmd();

        virtual ~SrvMouseCmd();

    protected:
        void pack();

    private:
        uint16_t sourceObjId_;
        uint16_t destObjId_;
        uint16_t srvDist_;
        int32_t destX_;
        int32_t destY_;
        int16_t posZ_;
};

//-----------------------------------------------

class CliStopMoving : public CRosePacket {
    public:
        CliStopMoving();
        CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]);

        virtual ~CliStopMoving();

        int32_t x() const { return posX_; }
        int32_t y() const { return posY_; }
        int16_t z() const { return posZ_; }

    private:
        int32_t posX_;
        int32_t posY_;
        int16_t posZ_;
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

    global_var() : craft_rate_(0),
    update_time_(0),
    world_rate_(0),
    town_rate_(0),
    flags_(0) {
      for (int i = 0; i < MAX_SELL_TYPE; ++i) {
        item_rate_[i] = 0;
      }
    }
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

    Enconmy_Data() : counter_(0),
    pop_base_(0),
    dev_base_(0),
    dev_(0),
    pop_(0) {
      for(int i =0; i < MAX_SELL_TYPE; ++i) {
        item_[i] = consume_[i] = 0;
      }
    }
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

class SrvSelectCharReply : public CRosePacket {
 public:
  SrvSelectCharReply(Entity character, uint32_t tag);

  virtual ~SrvSelectCharReply();

 protected:
  void pack();

 private:

  Entity entity_;
  uint32_t tag_;
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
};

//------------------------------------------------
//------------------------------------------------

class SrvBillingMsg : public CRosePacket {
 public:
  SrvBillingMsg() : CRosePacket(ePacketType::PAKWC_BILLING_MESSAGE){};

 protected:
  void pack() { *this << (uint16_t)0x1001 << (uint32_t)2; };
};

class CliStopReq : public CRosePacket {
 public:
  CliStopReq(uint8_t buffer[MAX_PACKET_SIZE]);
  virtual ~CliStopReq() {};

  float getX() const { return x; }
  float getY() const { return y; }

private:
  float x, y;
};

}

#endif
