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
#include "packetclasses.h"
#include "mappackets.h"
#include "crosepacket.h"
#include "packets.h"
#include "entitySystem.h"
#include <atomic>

class CMapClient : public RoseCommon::CRoseClient {
 public:
  CMapClient();
  CMapClient(tcp::socket _sock, std::shared_ptr<EntitySystem> entitySystem);

  virtual ~CMapClient() = default;

  virtual bool IsNearby(const CRoseClient* _otherClient) const override;

  void canBeDeleted() { canBeDeleted_.store(true); }

 protected:
  virtual bool HandlePacket(uint8_t* _buffer) override;
  virtual bool OnReceived() override;
  virtual void OnDisconnected() override;
  virtual bool OnShutdown() override;

  bool LogoutReply();
  bool JoinServerReply( std::unique_ptr<RoseCommon::CliJoinServerReq> P );

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

  std::atomic<bool> canBeDeleted_;
};

#endif
