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

#include "cmapserver.h"
#include "cmapisc.h"
#include "cmapclient.h"
#include "epackettype.h"

using namespace RoseCommon;

CMapClient::CMapClient() : CRoseClient(), access_rights_(0) {}

CMapClient::CMapClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0) {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
//    case ePacketType::PAKCS_JOIN_SERVER_REQ:
//      return JoinServerReply(pak);  // Allow client to connect
//    case ePacketType::PAKCS_CHAR_LIST_REQ:
//      return SendCharListReply(pak);
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }
