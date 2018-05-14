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

#ifndef __CMAPCLIENT_H__
#define __CMAPCLIENT_H__

#include "croseclient.h"
#include "crosepacket.h"
#include "mappackets.h"

#include "entitysystem.h"

#include <memory>

namespace RoseCommon {
class CliJoinServerReq;
}

class CMapClient : public RoseCommon::CRoseClient, public std::enable_shared_from_this<CMapClient> {
 public:
  CMapClient();
  CMapClient(std::unique_ptr<Core::INetwork> _sock, std::shared_ptr<EntitySystem> entitySystem);

  virtual ~CMapClient();

  virtual bool is_nearby(const CRoseClient* _otherClient) const override;

 protected:
  virtual bool HandlePacket(uint8_t* _buffer) override;
  virtual void OnDisconnected() override;

  void updateSession();

  bool LogoutReply();
  bool JoinServerReply(std::unique_ptr<RoseCommon::CliJoinServerReq> P);

  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
  };

  uint16_t access_rights_;
  eSTATE login_state_;
  uint32_t sessionId_;
  uint32_t userid_;
  uint32_t charid_;
  std::shared_ptr<EntitySystem> entitySystem_;
};

#endif
