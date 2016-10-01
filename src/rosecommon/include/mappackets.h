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
#include "dataconsts.h"
#include <string>
#include <exception>
#include <vector>
#include "entityComponents.h"
#include "cli_chat.h"
#include "cli_stop.h"
#include "cli_logout.h"
#include "cli_revive.h"
#include "cli_changemap.h"
#include "srv_chat.h"
#include "src_revive.h"
#include "srv_logout.h"
#include "srv_initdata.h"
#include "srv_changemap.h"
#include "srv_serverdata.h"
#include "srv_removeobject.h"

namespace RoseCommon {

class SrvSelectCharReply : public CRosePacket {
 public:
  SrvSelectCharReply(Entity character);

  virtual ~SrvSelectCharReply();

 protected:
  void pack();

 private:

  Entity entity_;
  uint32_t tag_;
};

class CliMouseCmd : public CRosePacket {
    public:
        CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]);
        CliMouseCmd();

        virtual ~CliMouseCmd();

        uint16_t targetId() const { return targetId_; }
        int32_t x() const { return x_; }
        int32_t y() const { return y_; }
        uint16_t z() const { return z_; }

    private:
        uint16_t targetId_;
        int32_t x_;
        int32_t y_;
        uint16_t z_;
};

class SrvMouseCmd : public CRosePacket {
    public:
        SrvMouseCmd(uint16_t sourceId, uint16_t destId, uint16_t dist,
                int32_t x, int32_t y, int16_t z);
        SrvMouseCmd();

        virtual ~SrvMouseCmd();

    protected:
        void pack();

    private:
        uint16_t sourceId_;
        uint16_t destId_;
        uint16_t dist_;
        int32_t x_;
        int32_t y_;
        int16_t z_;
};

class CliStopMoving : public CRosePacket {
    public:
        CliStopMoving();
        CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]);

        virtual ~CliStopMoving();

        int32_t x() const { return x_; }
        int32_t y() const { return y_; }
        int16_t z() const { return z_; }

    private:
        int32_t x_;
        int32_t y_;
        int16_t z_;
};

//------------------------------------------------
//------------------------------------------------

class SrvInventoryData : public CRosePacket {
 public:
  SrvInventoryData(Entity entity)
      : CRosePacket(ePacketType::PAKWC_INVENTORY_DATA), entity_(entity) {};

 protected:
  void pack() {
    *this << entity_.component<AdvancedInfo>()->zuly_;
    for (int i = 0; i < 140; ++i) {
      *this << (uint16_t)0 << (uint32_t)0;
    }
  };

 private:
  Entity entity_;
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



//------------------------------------------------
//------------------------------------------------

class SrvPlayerChar : public CRosePacket {
public:
    SrvPlayerChar(Entity entity)
        : CRosePacket(ePacketType::PAKWC_PLAYER_CHAR), entity_(entity) {}

    virtual ~SrvPlayerChar() {}

protected:
    void pack() override
    {
        auto pos = entity_.component<Position>();
        auto dest = entity_.component<Destination>();
        auto advanced = entity_.component<AdvancedInfo>();
        auto basic = entity_.component<BasicInfo>();
        auto info = entity_.component<CharacterInfo>();
        auto graphics = entity_.component<CharacterGraphics>();
        auto items = entity_.component<EquippedItems>();
        auto riding = entity_.component<RidingItems>();
        auto bullets = entity_.component<BulletItems>();
        float destX = pos->x_, destY = pos->y_;
        if (dest) {
            destX = dest->x_;
            destY = dest->y_;
        }

        *this << basic->tag_
              << (float)pos->x_ << (float)pos->y_ << (float)destX << (float)destY
              << (uint32_t)0 // command
              << basic->targetId_
              << advanced->moveMode_
              << advanced->hp_
              << basic->teamId_
              << info->pkFlag_ // statusFlag ?
              << graphics->race_
              << (uint16_t)1 // runSpeed
              << (uint16_t)1 // attackSpeed
              << (uint8_t)0; // weightRate
        for (auto &it : items->items_)
            *this << (ISerialize&)it;
        for (auto &it : bullets->items_)
            it.bulletPartialSerialize(*this);
        *this << info->job_
              << basic->level_;
        for (auto &it : riding->items_)
            *this << (ISerialize&)it;
        *this << pos->z_
              << (uint64_t)0; //subFlag
    }

private:
    Entity entity_;
};

}

#endif
