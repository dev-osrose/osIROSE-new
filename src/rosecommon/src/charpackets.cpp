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

RoseCommon::CliCreateCharReq::CliCreateCharReq(const std::string &name,
                                               uint8_t race, uint8_t stone,
                                               uint8_t hair, uint8_t face,
                                               uint8_t weapon, uint16_t zone)
    : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ),
      race_(race),
      stone_(stone),
      hair_(hair),
      face_(face),
      weapon_(weapon),
      zone_(zone),
      name_(name) {}

RoseCommon::CliCreateCharReq::CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_CREATE_CHAR_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> race_ >> stone_ >> hair_ >> face_ >> weapon_ >> zone_ >> name_;
}

RoseCommon::CliCreateCharReq::~CliCreateCharReq() {}

std::string RoseCommon::CliCreateCharReq::name() const { return name_; }

uint8_t RoseCommon::CliCreateCharReq::race() const { return race_; }

uint8_t RoseCommon::CliCreateCharReq::stone() const { return stone_; }

uint8_t RoseCommon::CliCreateCharReq::hair() const { return hair_; }

uint8_t RoseCommon::CliCreateCharReq::face() const { return face_; }

uint8_t RoseCommon::CliCreateCharReq::weapon() const { return weapon_; }

uint16_t RoseCommon::CliCreateCharReq::zone() const { return zone_; }

void RoseCommon::CliCreateCharReq::pack() {
  *this << race_ << stone_ << hair_ << face_ << weapon_ << zone_ << name_;
}

RoseCommon::SrvCreateCharReply::SrvCreateCharReply(uint8_t result,
                                                   uint8_t platinum /*= 0*/)
    : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY),
      result_(result),
      platinum_(platinum) {}

RoseCommon::SrvCreateCharReply::~SrvCreateCharReply() {}

uint8_t RoseCommon::SrvCreateCharReply::result() const { return result_; }

uint8_t RoseCommon::SrvCreateCharReply::platinum() const { return platinum_; }

void RoseCommon::SrvCreateCharReply::pack() { *this << result_ << platinum_; }

RoseCommon::CliDeleteCharReq::CliDeleteCharReq(const std::string &name,
                                               uint8_t id, uint8_t del)
    : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ),
      char_id_(id),
      delete_(del),
      name_(name) {}

RoseCommon::CliDeleteCharReq::CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_DELETE_CHAR_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> char_id_ >> delete_ >> name_;
}

RoseCommon::CliDeleteCharReq::~CliDeleteCharReq() {}

uint8_t RoseCommon::CliDeleteCharReq::char_id() const { return char_id_; }

bool RoseCommon::CliDeleteCharReq::isDelete() const {
  return (delete_ != 0) ? true : false;
}

std::string RoseCommon::CliDeleteCharReq::name() const { return name_; }

void RoseCommon::CliDeleteCharReq::pack() { *this << char_id_ << delete_; }

RoseCommon::SrvDeleteCharReply::SrvDeleteCharReply(const std::string &name,
                                                   uint32_t remaining_time)
    : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY),
      remaining_time_(remaining_time),
      name_(name) {}

RoseCommon::SrvDeleteCharReply::~SrvDeleteCharReply() {}

uint32_t RoseCommon::SrvDeleteCharReply::remaining_time() const {
  return remaining_time_;
}

std::string RoseCommon::SrvDeleteCharReply::name() const { return name_; }

void RoseCommon::SrvDeleteCharReply::pack() {
  *this << remaining_time_ << name_;
}

RoseCommon::CliSelectCharReq::CliSelectCharReq(const std::string &name,
                                               uint8_t id, uint8_t run,
                                               uint8_t ride)
    : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ),
      char_id_(id),
      run_mode_(run),
      ride_mode_(ride),
      name_(name) {}

RoseCommon::CliSelectCharReq::CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_SELECT_CHAR_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> char_id_ >> run_mode_ >> ride_mode_ >> name_;
}

RoseCommon::CliSelectCharReq::~CliSelectCharReq() {}

uint8_t RoseCommon::CliSelectCharReq::char_id() const { return char_id_; }

uint8_t RoseCommon::CliSelectCharReq::run_mode() const { return run_mode_; }

uint8_t RoseCommon::CliSelectCharReq::ride_mode() const { return ride_mode_; }

std::string RoseCommon::CliSelectCharReq::name() const { return name_; }

void RoseCommon::CliSelectCharReq::pack() {
  *this << char_id_ << run_mode_ << ride_mode_ << name_;
}

RoseCommon::SrvSelectCharReply::SrvSelectCharReply()
    : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY) {}

RoseCommon::SrvSelectCharReply::~SrvSelectCharReply() {}

void RoseCommon::SrvSelectCharReply::setCharacter(const std::string &name,
                                                  uint8_t race, uint16_t zone,
                                                  float x, float y,
                                                  uint16_t revive_zone,
                                                  uint32_t utag) {
  name_ = name;
  race_ = race;
  zone_ = zone;
  position_start_[0] = x;
  position_start_[1] = y;
  revive_zone_ = revive_zone;
  unique_tag_ = utag;
}

void RoseCommon::SrvSelectCharReply::addEquipItem(uint8_t slot,
                                                  uint16_t item_id /*= 0*/,
                                                  uint16_t gem /*= 0*/,
                                                  uint8_t socket /*= 0*/,
                                                  uint8_t grade /*= 0*/) {
    equip_item item = items_[slot];
    item.id_ = item_id;
    item.gem_op_ = gem;
    item.socket_ = socket;
    item.grade_ = grade;
    items_[slot] = item;
}

void RoseCommon::SrvSelectCharReply::pack()
{
  *this << race_ << zone_ << position_start_[0] << position_start_[1]
    << revive_zone_;
  for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i)
  {
    for (int j = 0; j < 4; ++j) *this << items_[i].data[j];
  }
  *this << base_character_info_ << stats_ << ext_stats_ << learned_skills_ << hotbar_ << unique_tag_ << name_;
}

RoseCommon::SrvCharacterListReply::char_info::char_info(
    const std::string &name, uint8_t race /*= 0*/, uint16_t level /*= 0*/,
    uint16_t job /*= 0*/, uint32_t delete_time /*= 0*/,
    uint8_t platinum /*= 0*/)
    : remain_sec_unitl_delete_(delete_time),
      level_(level),
      job_(job),
      race_(race),
      platinum_(platinum),
      name_(name) {}

RoseCommon::SrvCharacterListReply::equip_item::equip_item(
    uint16_t id /*= 0*/, uint16_t gem /*= 0*/, uint8_t socket /*= 0*/,
    uint8_t grade /*= 0*/)
    : id_(id), gem_op_(gem), socket_(socket), grade_(grade) {}

RoseCommon::SrvCharacterListReply::SrvCharacterListReply()
    : CRosePacket(ePacketType::PAKCC_CHAR_LIST_REPLY), character_count_(0) {}

RoseCommon::SrvCharacterListReply::~SrvCharacterListReply() {}

void RoseCommon::SrvCharacterListReply::addCharacter(
    const std::string &name, uint8_t race, uint16_t level, uint16_t job,
    uint32_t delete_time /*= 0*/) {
  ++character_count_;
  char_info character(name, race, level, job, delete_time);
  character_list_.push_back(character);

  for (int idx = 0; idx < MAX_EQUIPPED_ITEMS; ++idx)
    addEquipItem(character_count_ - 1, idx);
}

void RoseCommon::SrvCharacterListReply::addEquipItem(
    uint8_t char_id, uint8_t slot, uint16_t item_id /*= 0*/,
    uint16_t gem /*= 0*/, uint8_t socket /*= 0*/, uint8_t grade /*= 0*/) {
  if (char_id < character_count_ && slot < MAX_EQUIPPED_ITEMS) {
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

    for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
      for (int j = 0; j < 4; ++j) *this << character.items_[i].data[j];
    }
  }
}

void RoseCommon::SrvSelectCharReply::base_info::serialize(
    CRosePacket &os) const {
  os << stone_ << face_ << hair_ << job_ << union_ << rank_ << fame_;
}

void RoseCommon::SrvSelectCharReply::base_info::deserialize(CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::character_stats::serialize(
    CRosePacket &os) const {
  os << str_ << dex_ << int_ << con_ << charm_ << sense_;
}

void RoseCommon::SrvSelectCharReply::character_stats::deserialize(
    CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::status_effects::serialize(
    CRosePacket &os) const {
  os << expired_seconds_ << value_ << unknown_;
}

void RoseCommon::SrvSelectCharReply::status_effects::deserialize(
    CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::extended_stats::serialize(
    CRosePacket &os) const {
  os << hp_ << mp_ << exp_ << level_ << stat_points_ << skill_points_ << body_size_ << head_size_ << penalty_exp_ << fame1_ << fame2_;
  for (int i = 0; i < MAX_UNION_COUNT; ++i)
  {
    os << union_points_[i];
  }
  
  os << guild_id_ << guild_contribution_ << guild_position_ << pk_flags_ << stamina_;
  
  for (int i = 0; i < MAX_BUFF_STATUS; ++i)
  {
    os << status_[i];
  }

  os << pat_hp_ << pat_cooldown_time_;
}

void RoseCommon::SrvSelectCharReply::extended_stats::deserialize(
    CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::skills::serialize(CRosePacket &os) const {
  for (int i = 0; i < MAX_SKILL_COUNT; ++i)
  {
    os << skill_id_[i];
  }
}

void RoseCommon::SrvSelectCharReply::skills::deserialize(CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::hotbar_item::serialize(
    CRosePacket &os) const {
  os << item_;
}

void RoseCommon::SrvSelectCharReply::hotbar_item::deserialize(CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::hotbar::serialize(CRosePacket &os) const {
  for (int i = 0; i < MAX_HOTBAR_ITEMS; ++i)
  {
    os << list_[i];
  }
}

void RoseCommon::SrvSelectCharReply::hotbar::deserialize(CRosePacket &os) {
  (void)os;
}
