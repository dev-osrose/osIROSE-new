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

#ifndef __SRV_SERVERDATA_H__
#define __SRV_SERVERDATA_H__

#include "epackettype.h"
#include "crosepacket.h"

namespace RoseCommon {
	
class SrvServerData : public CRosePacket {
 public:
  SrvServerData(uint8_t type);

  virtual ~SrvServerData();

  uint8_t type() const;

  enum data_type : uint8_t { ECONOMY = 0, NPC };

 protected:
  void pack();

 private:
  struct Enconmy_Data {
    uint32_t counter_;
    uint16_t pop_base_;
    uint16_t dev_base_;
    uint16_t consume_[MAX_SELL_TYPE];
    uint16_t dev_;
    uint32_t pop_;
    uint32_t item_[MAX_SELL_TYPE];

    Enconmy_Data() : counter_(0),
    pop_base_(0),
    dev_base_(0),
    dev_(0),
    pop_(0) {
      for(int i =0; i < MAX_SELL_TYPE; ++i) {
        item_[i] = consume_[i] = 0;
      }
    }
  };
  uint8_t type_;
  Enconmy_Data enconmy_data_;
};

}

#endif