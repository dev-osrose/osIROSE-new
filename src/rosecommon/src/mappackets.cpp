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
#include "srv_changemapreply.h"
#include "srv_serverdata.h"

namespace RoseCommon {

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

}
