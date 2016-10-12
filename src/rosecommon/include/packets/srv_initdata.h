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

#ifndef __SRV_INITDATA_H__
#define __SRV_INITDATA_H__

#include "epackettype.h"
#include "crosepacket.h"

namespace RoseCommon {
	
class SrvInitDataReply : public CRosePacket {
 public:
  SrvInitDataReply(uint32_t rand_seed, uint16_t rand_index);

  virtual ~SrvInitDataReply();

  uint32_t rand_seed() const;
  uint16_t rand_index() const;

 protected:
  void pack();

 private:
  uint32_t rand_seed_;
  uint16_t rand_index_;
};

}

#endif