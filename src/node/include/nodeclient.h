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

#ifndef _NODECLIENT_H_
#define _NODECLIENT_H_

#include "croseclient.h"
#include "srv_accept_reply.h"
#include "srv_srv_select_reply.h"
#include "srv_switch_server.h"

#include "cli_accept_req.h"
#include "cli_login_req.h"
#include "cli_join_server_req.h"

class NodeClient : public RoseCommon::CRoseClient {
 public:
  NodeClient();
  NodeClient(std::unique_ptr<Core::INetwork> _sock);

 protected:
  virtual bool onShutdown();
  virtual bool handlePacket(uint8_t* _buffer) override;
  virtual bool handleServerPacket(uint8_t* _buffer) override;

  // Packet Helper Functions
  bool serverAcceptReply(RoseCommon::Packet::SrvAcceptReply&& P);
  bool serverSelectReply(RoseCommon::Packet::SrvSrvSelectReply&& P);
  bool serverSwitchServer(RoseCommon::Packet::SrvSwitchServer&& P);
  
  bool clientAcceptReq(RoseCommon::Packet::CliAcceptReq&& P);
  bool clientLoginReq(RoseCommon::Packet::CliLoginReq&& P);
  bool clientJoinServerReq(RoseCommon::Packet::CliJoinServerReq&& P);

  uint32_t session_id_;
  std::unique_ptr<RoseCommon::CRosePacket> buffered_packet_;
};

#endif
