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

#ifndef __CMAPISC_H__
#define __CMAPISC_H__

#include "croseisc.h"
#include "isc_server_register.h"
#include <vector>

class CMapServer;

class CMapISC : public RoseCommon::CRoseISC {
 public:
  CMapISC();
  CMapISC(CMapServer* server, std::unique_ptr<Core::INetwork> _sock);

  bool isChar() const;

  void add_maps(const std::vector<uint16_t>& maps);

 protected:
  bool serverRegister(RoseCommon::Packet::IscServerRegister&& P);
  bool handlePacket(uint8_t* _buffer) override;

  virtual void onConnected() override;
  virtual bool onShutdown() override;

 private:
  CMapServer *server_;
  std::vector<uint16_t> maps;
};

#endif
