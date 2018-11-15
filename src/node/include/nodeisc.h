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

#ifndef _NODEISC_H_
#define _NODEISC_H_

#include <forward_list>
#include "croseisc.h"

#include "isc_serverregister.h"
#include "isc_shutdown.h"

namespace Core {
class INetwork;
}

class NodeISC : public RoseCommon::CRoseISC {
 public:
  NodeISC();
  NodeISC(std::unique_ptr<Core::INetwork> _sock);

  std::forward_list<RoseCommon::tChannelInfo> getChannelList() const {
    return channel_list_;
  }

 protected:
  bool handlePacket(uint8_t* _buffer) override;
  bool serverRegister(RoseCommon::IscServerRegister&& P);
  bool serverShutdown(RoseCommon::IscShutdown&& P);

  std::string server_name_;
  uint32_t channel_count_;
  uint32_t min_right_;
  bool test_server_;

  std::forward_list<RoseCommon::tChannelInfo> channel_list_;
};

#endif
