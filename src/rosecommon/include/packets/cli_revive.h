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

#ifndef __CLI_REVIVE_H__
#define __CLI_REVIVE_H__

#include "epackettype.h"
#include "crosepacket.h"

namespace RoseCommon {
	
class CliReviveReq : public CRosePacket {
 public:
  CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]);
  CliReviveReq(uint8_t type = 0);

  enum eType : uint8_t { REVIVE_POS, SAVE_POS, START_POS, CURRENT_POS };

  eType getType() const;

 protected:
  void pack();

 private:
  uint8_t type_;
};
	
}

#endif