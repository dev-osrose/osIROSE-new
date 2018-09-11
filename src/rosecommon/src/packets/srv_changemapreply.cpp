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

#include "srv_changemapreply.h"

namespace RoseCommon {

RoseCommon::SrvChangeMapReply::SrvChangeMapReply(
    uint16_t object_index, uint16_t current_hp, uint16_t current_mp,
    uint16_t current_exp, uint16_t penalize_exp, uint16_t world_time,
    uint16_t team_number)
    : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY) {
  write_uint16(object_index);
  write_uint16(current_hp);
  write_uint16(current_mp);
  write_uint16(current_exp);
  write_uint16(penalize_exp);
  { // global vars
  write_uint16(0); // craft rate
  write_uint32(0); // update time
  write_uint16(0); // world rate
  write_uint8(0); // town rate
  for (int i = 0; i < MAX_SELL_TYPE; ++i) {
    write_uint8(0);
  } // global vars
  write_uint32(0); // flags
  }
  write_uint16(world_time);
  write_uint16(team_number);
}
