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

#ifndef _MAPPACKETS_H_
#define _MAPPACKETS_H_

#include "epackettype.h"
#include "crosepacket.h"
#include "packetfactory.h"

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_BILLING_MESSAGE, SrvBillingMsg)
class SrvBillingMsg : public CRosePacket {
 public:
  SrvBillingMsg() : CRosePacket(ePacketType::PAKWC_BILLING_MESSAGE) {
    write_uint16(0x1001);
    write_uint32(2);
  }
};

}

#endif
