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

#ifndef _CLOGINSERVER_H_
#define _CLOGINSERVER_H_

#include "croseserver.h"
#include "cmysql_database.h"

class CLoginServer : public RoseCommon::CRoseServer {
 public:
  CLoginServer(bool _isc = false);
  virtual ~CLoginServer();

  // This will give you the count of the clients in the list
  uint32_t GetClientCount() const {
    return (client_list_.empty() ? 0 : std::distance(std::begin(client_list_),
                                                     std::end(client_list_)));
  }

 protected:
  virtual void OnAccepted(Core::INetwork* _sock);
  uint32_t client_count_;
  uint32_t server_count_;
};

#endif
