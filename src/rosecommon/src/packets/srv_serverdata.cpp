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
    : CRosePacket(ePacketType::PAKWC_GLOBAL_VARS) {
  write_uint8(type);
  switch (type) {
    case data_type::ECONOMY:
      write_uint32(0); // economy counter
      write_uint16(0); // economy pop base
      write_uint16(0); // economy dev base
      for (int i = MIN_SELL_TYPE; i < MAX_SELL_TYPE; ++i) {
        write_uint16(0); // economy consume
      }
      write_uint16(0); // economy dev
      write_uint32(0); // economy pop
      for (int i = MIN_SELL_TYPE; i < MAX_SELL_TYPE; ++i) {
        write_uint32(0); // economy item
      }
      break;
    case data_type::NPC:
    default:
      break;
  }
}
}
