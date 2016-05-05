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
  SrvSwitchServerReply(const std::string &ip, uint16_t port,
                       uint32_t session_id, uint32_t random_seed);

  virtual ~SrvSwitchServerReply();

  uint16_t port() const;
  uint32_t session_id() const;
  uint32_t random_seed() const;
  std::string ip() const;

 protected:
  void pack();

 private:
  uint16_t port_;
  std::string ip_;
  uint32_t session_ids_[2];
};

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

class SrvInventoryData : public CRosePacket {
 public:
  SrvInventoryData(int64_t zuly)
      : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), zuly_(zuly){};

 protected:
  void pack() { *this << zuly_; };

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
  void pack() {  };

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
}

#endif