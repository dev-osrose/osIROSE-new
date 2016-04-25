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
#include "rosepackets.h"

class CMapClient : public RoseCommon::CRoseClient {
 public:
  CMapClient();
  CMapClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();
  
  bool JoinServerReply(std::unique_ptr<RoseCommon::CliJoinServerReq> P);
  
  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
  };

  uint16_t access_rights_;
  eSTATE login_state_;
  uint32_t session_id_;
  uint32_t userid_;
  uint32_t charid_;
};

#endif
