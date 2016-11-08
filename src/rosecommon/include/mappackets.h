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
#include "packetfactory.h"
#include "packets.h"

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_SELECT_CHAR_REPLY, SrvSelectCharReply)
class SrvSelectCharReply : public CRosePacket {
 public:
  SrvSelectCharReply(Entity character);

  virtual ~SrvSelectCharReply();

 protected:
  void pack();

 private:

  Entity entity_;
};

class CliStopMoving : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_STOP_MOVING, CliStopMoving> {
    public:
        CliStopMoving();
        CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]);

        virtual ~CliStopMoving();

        float x() const { return x_; }
        float y() const { return y_; }
        int16_t z() const { return z_; }

    private:
        float x_;
        float y_;
        int16_t z_;
};

REGISTER_SEND_PACKET(ePacketType::PAKWC_STOP_MOVING, SrvStopMoving)
class SrvStopMoving : public CRosePacket {
    public:
        SrvStopMoving() : CRosePacket(ePacketType::PAKWC_STOP) {}
        SrvStopMoving(Entity entity) : CRosePacket(ePacketType::PAKWC_STOP), entity_(entity) {}

        virtual ~SrvStopMoving() {}

    protected:
        void pack() {
            auto basic = entity_.component<BasicInfo>();
            auto pos = entity_.component<Position>();
            *this << basic->id_;
            *this << pos->x_ << pos->y_ << pos->z_;
        }

    private:
        Entity entity_;
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
    auto inventory = entity_.component<Inventory>();
    for (auto &it : inventory->items_)
        *this << (ISerialize&)it;
    }

 private:
  Entity entity_;
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
        auto equipped = entity_.component<EquippedItems>();
        auto riding = entity_.component<RidingItems>();
        auto bullets = entity_.component<BulletItems>();
        float destX = pos->x_, destY = pos->y_;
        if (dest) {
            destX = dest->x_;
            destY = dest->y_;
        }

        *this << basic->id_
              << pos->x_ << pos->y_ << destX << destY
              << basic->command_
              << basic->targetId_
              << advanced->moveMode_
              << advanced->hp_
              << basic->teamId_
              << info->statusFlag_
              << graphics->race_
              << advanced->runSpeed_
              << advanced->atkSpeed_
              << advanced->weightRate_;

        for (auto &it : equipped->items_)
            it.partialSerialize(*this);

        for (auto &it : bullets->items_)
            it.bulletPartialSerialize(*this);

        *this << info->job_
              << basic->level_;

        for (auto &it : riding->items_)
            it.partialSerialize(*this);

        *this << pos->z_
              << (uint32_t)0; //subFlag

        *this << basic->name_; //szUserID

        *this << basic->name_; //Avatar name
    }

private:
    Entity entity_;
};

}

#endif
