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

#ifndef __SRV_CHANGEMAP_H__
#define __SRV_CHANGEMAP_H__

#include "epackettype.h"
#include "crosepacket.h"
#include "packetfactory.h"

namespace RoseCommon
{
  REGISTER_SEND_PACKET(ePacketType::PAKWC_CHANGE_MAP_REPLY, SrvChangeMapReply)

  class SrvChangeMapReply : public CRosePacket {
  public:
    SrvChangeMapReply(uint16_t object_index, uint16_t current_hp,
                      uint16_t current_mp, uint16_t current_exp,
                      uint16_t penalize_exp, uint16_t world_time,
                      uint16_t team_number);

    virtual ~SrvChangeMapReply();

    uint16_t object_index() const;
    uint16_t current_hp() const;
    uint16_t current_mp() const;

    void setItemRate(uint8_t type, uint8_t rate);

  protected:
    void pack();

  private:
    struct global_var {
      uint16_t craft_rate_;
      uint32_t update_time_;
      uint16_t world_rate_;
      uint8_t town_rate_;
      uint8_t item_rate_[MAX_SELL_TYPE];
      uint32_t flags_;

      global_var() : craft_rate_( 0 ),
                     update_time_( 0 ),
                     world_rate_( 0 ),
                     town_rate_( 0 ),
                     flags_( 0 ) {
        for ( int i = 0; i < MAX_SELL_TYPE; ++i ) {
          item_rate_[i] = 0;
        }
      }
    };

    uint16_t object_index_;
    uint16_t current_hp_;
    uint16_t current_mp_;
    uint64_t current_exp_;
    uint64_t penalize_exp_;
    uint32_t world_time_;
    uint32_t team_number_;
    global_var zone_vars_;
  };
}

#endif
