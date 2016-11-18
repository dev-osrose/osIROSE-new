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

REGISTER_SEND_PACKET(ePacketType::PAKWC_QUEST_DATA, SrvQuestData)
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

REGISTER_SEND_PACKET(ePacketType::PAKWC_BILLING_MESSAGE, SrvBillingMsg)
class SrvBillingMsg : public CRosePacket {
 public:
  SrvBillingMsg() : CRosePacket(ePacketType::PAKWC_BILLING_MESSAGE){};

 protected:
  void pack() { *this << (uint16_t)0x1001 << (uint32_t)2; };
};

}

#endif
