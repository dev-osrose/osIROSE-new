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
#include "packetfactory.h"

namespace RoseCommon {

//------------------------------------------------
//------------------------------------------------

REGISTER_SEND_PACKET(ePacketType::PAKCC_CHAR_LIST_REPLY, SrvCharacterListReply)
class SrvCharacterListReply : public CRosePacket {
 public:
  SrvCharacterListReply();
  SrvCharacterListReply(uint8_t buffer[MAX_PACKET_SIZE]);

  virtual ~SrvCharacterListReply();

  void addCharacter(const std::string &name, uint8_t race, uint16_t level,
                    uint16_t job, uint32_t face, uint32_t hair, uint32_t delete_time = 0);

  void addEquipItem(uint8_t char_id, uint8_t slot, uint16_t item_id = 0,
                    uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0);

  enum equipped_position {
    EQUIP_HELMET = 0,
    EQUIP_ARMOR = 1,
    EQUIP_GAUNTLET,
    EQUIP_BOOTS,
    EQUIP_GOGGLES,
    EQUIP_BACKPACK,
    EQUIP_WEAPON_R,
    EQUIP_WEAPON_L,
    MAX_EQUIPPED_ITEMS
  };
  static equipped_position getPosition(uint32_t slot);

  uint8_t getCharacterCount() const {
    return character_count_;
  }

 private:
  struct char_info;
 public:

  const std::vector<char_info> &getCharacters() const {
    return character_list_;
  }

 protected:
  void pack();

 private:
  uint8_t character_count_;

  struct equip_item {
    union {
      PACK(struct {
        unsigned int id_ : 10;     // 0~1023
        unsigned int gem_op_ : 9;  // 0~512
        unsigned int socket_ : 1;  // 0~1
        unsigned int grade_ : 4;   // 0~15
      });
      uint32_t data;
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
    uint32_t face_;
    uint32_t hair_;

    char_info() {}
    char_info(const std::string &name, uint8_t race = 0, uint16_t level = 0,
              uint16_t job = 0, uint32_t delete_time = 0, uint8_t platinum = 0, uint32_t face = 1, uint32_t hair = 0);
  };
  std::vector<char_info> character_list_;
};

}

#endif
