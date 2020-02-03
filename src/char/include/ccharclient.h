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

#ifndef __CCHARCLIENT_H__
#define __CCHARCLIENT_H__

#include <memory>

#include "croseclient.h"

#include "cli_create_char_req.h"
#include "cli_delete_char_req.h"
#include "cli_join_server_req.h"
#include "cli_select_char_req.h"

class CCharServer;

class CCharClient : public RoseCommon::CRoseClient, public std::enable_shared_from_this<CCharClient> {
 public:
  CCharClient();
  CCharClient(CCharServer *server, std::unique_ptr<Core::INetwork> _sock);
  ~CCharClient();

  uint32_t sessionId() const { return sessionId_; }

  void send_packet(const RoseCommon::CRosePacket& packet) { CRoseClient::send(packet); } 

 protected:
  virtual bool handlePacket(uint8_t* _buffer) override;

  bool joinServerReply(RoseCommon::Packet::CliJoinServerReq&& P);
  bool sendCharListReply();
  bool sendCharCreateReply(RoseCommon::Packet::CliCreateCharReq&& P);
  bool sendCharDeleteReply(RoseCommon::Packet::CliDeleteCharReq&& P);
  bool sendCharSelectReply(RoseCommon::Packet::CliSelectCharReq&& P);
 
  virtual void onDisconnected() override;

 protected:
  void updateSession();

  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
    TRANSFERING,
  };

  uint16_t accessRights_;
  eSTATE loginState_;
  uint32_t sessionId_;
  uint32_t userId_;
  uint32_t channelId_;
  uint32_t charId_;

  std::vector<uint32_t> characterRealId_;
  CCharServer *server_;
};

#endif
