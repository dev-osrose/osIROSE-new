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

#include "srv_serverdata.h"

namespace RoseCommon {

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
