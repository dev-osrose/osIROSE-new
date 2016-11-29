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

#include "charpackets.h"
#include "components/inventory.h"

RoseCommon::SrvCharacterListReply::char_info::char_info(
    const std::string &name, uint8_t race /*= 0*/, uint16_t level /*= 0*/,
    uint16_t job /*= 0*/, uint32_t delete_time /*= 0*/,
    uint8_t platinum /*= 0*/, uint32_t face, uint32_t hair)
    : remain_sec_unitl_delete_(delete_time),
      level_(level),
      job_(job),
      race_(race),
      platinum_(platinum),
      name_(name),
      face_(face),
      hair_(hair) {}

RoseCommon::SrvCharacterListReply::equip_item::equip_item(
    uint16_t id /*= 0*/, uint16_t gem /*= 0*/, uint8_t socket /*= 0*/,
    uint8_t grade /*= 0*/)
    : id_(id), gem_op_(gem), socket_(socket), grade_(grade) {}

RoseCommon::SrvCharacterListReply::SrvCharacterListReply()
    : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY), character_count_(0) {}

RoseCommon::SrvCharacterListReply::~SrvCharacterListReply() {}

void RoseCommon::SrvCharacterListReply::addCharacter(
    const std::string &name, uint8_t race, uint16_t level, uint16_t job, uint32_t face, uint32_t hair,
    uint32_t delete_time /*= 0*/) {
  ++character_count_;
  char_info character(name, race, level, job, delete_time, false, face, hair);
  character_list_.push_back(character);
}

RoseCommon::SrvCharacterListReply::equipped_position RoseCommon::SrvCharacterListReply::getPosition(uint32_t slot) {
    using equipped_position = RoseCommon::SrvCharacterListReply::equipped_position;
    switch (slot) {
        case Inventory::GOGGLES:
            return equipped_position::EQUIP_GOGGLES;
        case Inventory::HELMET:
            return equipped_position::EQUIP_HELMET;
        case Inventory::ARMOR:
            return equipped_position::EQUIP_ARMOR;
        case Inventory::BACKPACK:
            return equipped_position::EQUIP_BACKPACK;
        case Inventory::GAUNTLET:
            return equipped_position::EQUIP_GAUNTLET;
        case Inventory::BOOTS:
            return equipped_position::EQUIP_BOOTS;
        case Inventory::WEAPON_R:
            return equipped_position::EQUIP_WEAPON_R;
        case Inventory::WEAPON_L:
            return equipped_position::EQUIP_WEAPON_L;
    }
    return equipped_position::EQUIP_WEAPON_R;
}

#include <iostream>
void RoseCommon::SrvCharacterListReply::addEquipItem(
    uint8_t char_id, uint8_t slot, uint16_t item_id /*= 0*/,
    uint16_t gem /*= 0*/, uint8_t socket /*= 0*/, uint8_t grade /*= 0*/) {
  if (char_id < character_count_ && slot < MAX_EQUIPPED_ITEMS) {
    std::cout << "adding item " << (int)item_id << " to slot " << (int)slot << std::endl;
    equip_item item = character_list_[char_id].items_[slot];
    item.id_ = item_id;
    item.gem_op_ = gem;
    item.socket_ = socket;
    item.grade_ = grade;
    character_list_[char_id].items_[slot] = item;
  }
}

void RoseCommon::SrvCharacterListReply::pack() {
  *this << character_count_;

  for (auto &character : character_list_) {
    *this << character.name_;
    *this << character.race_ << character.level_ << character.job_
          << character.remain_sec_unitl_delete_ << character.platinum_;
    *this << character.face_ << character.hair_;

    for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
        *this << character.items_[i].data;
    }
  }
}

