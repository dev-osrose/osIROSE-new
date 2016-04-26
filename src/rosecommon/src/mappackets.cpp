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

RoseCommon::SrvSwitchServerReply::SrvSwitchServerReply(const std::string &ip,
                                                       uint16_t port,
                                                       uint32_t session_id,
                                                       uint32_t random_seed)
    : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER), port_(port), ip_(ip) {
  session_ids_[0] = session_id;
  session_ids_[1] = random_seed;
}

RoseCommon::SrvSwitchServerReply::~SrvSwitchServerReply() {}

uint32_t RoseCommon::SrvSwitchServerReply::port() const { return port_; }

uint32_t RoseCommon::SrvSwitchServerReply::session_id() const {
  return session_ids_[0];
}

uint32_t RoseCommon::SrvSwitchServerReply::random_seed() const {
  return session_ids_[1];
}

std::string RoseCommon::SrvSwitchServerReply::ip() const { return ip_; }

void RoseCommon::SrvSwitchServerReply::pack() {
  *this << port_ << session_ids_[0] << session_ids_[1] << ip_;
}

RoseCommon::CliLogoutReq::CliLogoutReq()
    : CRosePacket(ePacketType::PAKCS_LOGOUT_REQ) {}

RoseCommon::CliLogoutReq::CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE])
    : CRosePacket(buffer) {
  if (type() != ePacketType::PAKCS_LOGOUT_REQ)
    throw std::runtime_error("Not the right packet!");
}

RoseCommon::CliLogoutReq::~CliLogoutReq() {}

RoseCommon::SrvLogoutReply::SrvLogoutReply(uint16_t wait_time)
    : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY), wait_time_(wait_time) {}

RoseCommon::SrvLogoutReply::~SrvLogoutReply() {}

uint16_t RoseCommon::SrvLogoutReply::wait_time() const { return wait_time_; }

void RoseCommon::SrvLogoutReply::pack() { *this << wait_time_; }

RoseCommon::CliChangeMapReq::CliChangeMapReq()
    : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ) {}

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

RoseCommon::SrvReviveReply::SrvReviveReply(uint16_t mapid /*= 0*/)
    : CRosePacket(ePacketType::PAKWC_REVIVE_REPLY), mapid_(mapid) {}

void RoseCommon::SrvReviveReply::pack() { *this << mapid_; }

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

RoseCommon::SrvServerData::SrvServerData(uint8_t type)
    : CRosePacket(ePacketType::PAKWC_INIT_DATA), type_(type) {}

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
