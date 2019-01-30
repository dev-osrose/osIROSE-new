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

#ifndef _CROSEISC_H_
#define _CROSEISC_H_

#include "croseclient.h"

namespace RoseCommon {

class CRoseISC : public CRoseClient {
 public:
  CRoseISC();
  CRoseISC(std::unique_ptr<Core::INetwork> _sock);
  virtual ~CRoseISC();

 protected:
  // Override the callback functions we will use only
  virtual void onConnected();
  virtual bool onShutdown();
  virtual bool onReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) override;
  virtual bool onSend(uint16_t socket_id_, uint8_t* _buffer) override;
  virtual bool handlePacket(uint8_t* _buffer) override;
};

}
#endif
