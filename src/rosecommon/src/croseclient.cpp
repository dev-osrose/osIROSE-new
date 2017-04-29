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

#include <ctime>
#include "croseclient.h"
#include "epackettype.h"
#include "packets.h"

namespace RoseCommon {
//#define STRESS_TEST

CRoseClient::CRoseClient() : CRoseSocket() {
}

CRoseClient::CRoseClient(std::unique_ptr<Core::INetwork> _sock) : CRoseSocket(std::move(_sock)) {
  socket_->recv_data();
}

CRoseClient::~CRoseClient() {
}

bool CRoseClient::is_nearby(const CRoseClient* _otherClient) const {
  (void)_otherClient;
  logger_->trace("CRoseClient::IsNearby()");
  return false;
}

}
