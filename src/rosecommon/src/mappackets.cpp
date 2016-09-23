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

#include "mappackets.h"

namespace RoseCommon {

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliLogoutReq::CliLogoutReq()
    : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

RoseCommon::CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_LOGOUT_REQ)
    throw std::runtime_error("Not the right packet!");
}

RoseCommon::CliLogoutReq::~CliLogoutReq() {}

//---------------------------------------------

RoseCommon::SrvLogoutReply::SrvLogoutReply(uint16_t wait_time)
    : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY), wait_time_(wait_time) {}

RoseCommon::SrvLogoutReply::~SrvLogoutReply() {}

uint16_t RoseCommon::SrvLogoutReply::wait_time() const { return wait_time_; }

void RoseCommon::SrvLogoutReply::pack() { *this << wait_time_; }
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliChangeMapReq::CliChangeMapReq()
    : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ), 
      weight_rate_(0),
      position_z_(0) {}

RoseCommon::CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_CHANGE_MAP_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> weight_rate_ >> position_z_;
}

RoseCommon::CliChangeMapReq::~CliChangeMapReq() {}

uint8_t RoseCommon::CliChangeMapReq::weight_rate() const {
  return weight_rate_;
}

uint16_t RoseCommon::CliChangeMapReq::position_z() const { return position_z_; }

void RoseCommon::CliChangeMapReq::pack() {
  *this << weight_rate_ << position_z_;
}
//---------------------------------------------
RoseCommon::SrvChangeMapReply::SrvChangeMapReply(
    uint16_t object_index, uint16_t current_hp, uint16_t current_mp,
    uint16_t current_exp, uint16_t penalize_exp, uint16_t world_time,
    uint16_t team_number)
    : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY),
      object_index_(object_index),
      current_hp_(current_hp),
      current_mp_(current_mp),
      current_exp_(current_exp),
      penalize_exp_(penalize_exp),
      world_time_(world_time),
      team_number_(team_number) {
  for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
    zone_vars_.item_rate_[idx] = 0;
  }
}

RoseCommon::SrvChangeMapReply::~SrvChangeMapReply() {}

uint16_t RoseCommon::SrvChangeMapReply::object_index() const {
  return object_index_;
}

uint16_t RoseCommon::SrvChangeMapReply::current_hp() const {
  return current_hp_;
}

uint16_t RoseCommon::SrvChangeMapReply::current_mp() const {
  return current_mp_;
}

void RoseCommon::SrvChangeMapReply::setItemRate(uint8_t type, uint8_t rate) {
  zone_vars_.item_rate_[type] = rate;
}

void RoseCommon::SrvChangeMapReply::pack() {
  *this << object_index_ << current_hp_ << current_mp_ << current_exp_
        << penalize_exp_;
  {  // global_var
    *this << zone_vars_.craft_rate_ << zone_vars_.update_time_
          << zone_vars_.world_rate_ << zone_vars_.town_rate_;
    for (int idx = 0; idx < MAX_SELL_TYPE; ++idx) {
      *this << zone_vars_.item_rate_[idx];
    }
    *this << zone_vars_.flags_;
  }  // global_var
  *this << world_time_ << team_number_;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliChat::CliChat(const std::string &chat /*= ""*/)
    : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT), chat_(chat) {}

RoseCommon::CliChat::CliChat(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_NORMAL_CHAT)
    throw std::runtime_error("Not the right packet!");
  *this >> chat_;
}

std::string RoseCommon::CliChat::getMessage() const { return chat_; }

void RoseCommon::CliChat::pack() { *this << chat_; }
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::CliReviveReq::CliReviveReq(uint8_t type /*= 0*/)
    : CRosePacket(ePacketType::PAKCS_REVIVE_REQ), type_(type) {}

RoseCommon::CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_REVIVE_REQ)
    throw std::runtime_error("Not the right packet!");
  *this >> type_;
}

RoseCommon::CliReviveReq::eType RoseCommon::CliReviveReq::getType() const {
  return (eType)type_;
}

void RoseCommon::CliReviveReq::pack() { *this << type_; }
//---------------------------------------------
RoseCommon::SrvReviveReply::SrvReviveReply(uint16_t mapid /*= 0*/)
    : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY), mapid_(mapid) {}

void RoseCommon::SrvReviveReply::pack() { *this << mapid_; }

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvInitDataReply::SrvInitDataReply(uint32_t rand_seed,
                                               uint16_t rand_index)
    : CRosePacket(ePacketType::PAKWC_INIT_DATA),
      rand_seed_(rand_seed),
      rand_index_(rand_index) {}

RoseCommon::SrvInitDataReply::~SrvInitDataReply() {}

uint32_t RoseCommon::SrvInitDataReply::rand_seed() const { return rand_seed_; }

uint16_t RoseCommon::SrvInitDataReply::rand_index() const {
  return rand_index_;
}

void RoseCommon::SrvInitDataReply::pack() {
  *this << rand_seed_ << rand_index_;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvServerData::SrvServerData(uint8_t type)
    : CRosePacket(ePacketType::PAKWC_GLOBAL_VARS), type_(type) {}

RoseCommon::SrvServerData::~SrvServerData() {}

uint8_t RoseCommon::SrvServerData::type() const { return type_; }

void RoseCommon::SrvServerData::pack() {
  *this << type_;

  switch (type_) {
    case data_type::ECONOMY: {
      *this << enconmy_data_.counter_ << enconmy_data_.pop_base_
            << enconmy_data_.dev_base_;
      for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
        *this << enconmy_data_.consume_[idx];
      }
      *this << enconmy_data_.dev_ << enconmy_data_.pop_;
      for (int idx = MIN_SELL_TYPE; idx < MAX_SELL_TYPE; ++idx) {
        *this << enconmy_data_.item_[idx];
      }
      break;
    }
    case data_type::NPC:
    default:
      break;
  }
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

SrvRemoveObject::SrvRemoveObject(uint16_t obj_id)
    : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT), obj_id_(obj_id) {}

void SrvRemoveObject::pack() { *this << obj_id_; }

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
RoseCommon::SrvSelectCharReply::SrvSelectCharReply()
    : CRosePacket(ePacketType::PAKWC_SELECT_CHAR_REPLY) {
  race_ = zone_ = revive_zone_ = position_start_[0] = position_start_[1] =
      unique_tag_ = 0;
  name_ = "";

  for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
    items_[i] = equip_item();
  }

  base_character_info_ = base_info();
  stats_ = character_stats();
  ext_stats_ = extended_stats();
  learned_skills_ = skills();
  hotbar_ = hotbar();
}

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
  equip_item item = equip_item(item_id, gem, socket, grade);
  items_[slot] = item;
}

void RoseCommon::SrvSelectCharReply::pack() {
  *this << race_ << zone_ << position_start_[0] << position_start_[1]
        << revive_zone_;

    for (int i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
      items_[i].serialize(*this);
    }

    base_character_info_.serialize( *this );
    stats_.serialize( *this );
    ext_stats_.serialize( *this );
    learned_skills_.serialize( *this );
    hotbar_.serialize( *this );

    *this << unique_tag_ << name_;

//    *this << base_character_info_ << stats_ << ext_stats_ << learned_skills_
//          << hotbar_ << unique_tag_ << name_;
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
  os << hp_ << mp_ << exp_ << level_ << stat_points_ << skill_points_
     << body_size_ << head_size_ << penalty_exp_ << fame1_ << fame2_;
  for (int i = 0; i < MAX_UNION_COUNT; ++i) {
    os << union_points_[i];
  }

  os << guild_id_ << guild_contribution_ << guild_position_ << pk_flags_
     << stamina_;

  for (int i = 0; i < MAX_BUFF_STATUS; ++i) {
    status_[i].serialize( os );
  }

  os << pat_hp_ << pat_cooldown_time_;
}

void RoseCommon::SrvSelectCharReply::extended_stats::deserialize(
    CRosePacket &os) {
  (void)os;
}

void RoseCommon::SrvSelectCharReply::skills::serialize(CRosePacket &os) const {
  for (int i = 0; i < MAX_SKILL_COUNT; ++i) {
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
  for (int i = 0; i < MAX_HOTBAR_ITEMS; ++i) {
    list_[i].serialize(os);
  }
}

void RoseCommon::SrvSelectCharReply::hotbar::deserialize(CRosePacket &os) {
  (void)os;
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

void SrvSelectCharReply::equip_item::serialize(CRosePacket &os) const {
  uint32_t data = ((grade_ & 0xF) << 20) | ((socket_ & 0x1) << 19) |
                  ((gem_op_ & 0x1FF) << 10) | (id_ & 0x3FF);
  os << data;
}

void SrvSelectCharReply::equip_item::deserialize(CRosePacket &os) { (void)os; }

CliStopReq::CliStopReq( uint8_t buffer[MAX_PACKET_SIZE] )     : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_STOP)
    throw std::runtime_error("Not the right packet!");
  *this >> x >> y;
}
}