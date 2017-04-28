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

#ifndef _CROSECLIENT_H_
#define _CROSECLIENT_H_

#include <spdlog/spdlog.h>
#include "logconsole.h"
#include "crosecrypt.h"
#include "inetwork.h"
#include "crosepacket.h"
#include "entitycomponents.h"
#include "crosesocket.h"

namespace RoseCommon {
class CRoseServer;

class CRoseClient : public CRoseSocket {
 friend class CRoseServer;
 public:
  CRoseClient();
  CRoseClient(Core::INetwork* _sock);
  virtual ~CRoseClient();

  virtual bool is_nearby(const CRoseClient* _otherClient) const;

  Entity getEntity() const { return entity_; }

 protected:
  Entity entity_;
};

}

#endif
