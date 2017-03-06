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
#include "entityComponents.h"

namespace RoseCommon {
class CRoseServer;

class CRoseClient {
 friend class CRoseServer;
 public:
  CRoseClient();
  CRoseClient(Core::INetwork* _sock);
  virtual ~CRoseClient();

  virtual bool Send(CRosePacket &_buffer);
  virtual bool Send(std::unique_ptr<uint8_t[]> _buffer);

  virtual bool IsNearby(const CRoseClient* _otherClient) const;

  Entity getEntity() const { return entity_; }

  virtual uint32_t GetObjId() const {
    return socket_->GetId();
  }

  bool IsActive() const { return socket_->IsActive(); }
  bool GetId() const { return socket_->GetId(); }
  virtual void SetId(uint32_t _val) { socket_->SetId(_val); }
  std::chrono::steady_clock::time_point GetLastUpdateTime() const { return socket_->GetLastUpdateTime(); }
  virtual bool Shutdown(bool _final = false) { return socket_->Shutdown(_final); }

  std::shared_ptr<spdlog::logger> logger_;

 protected:

  virtual void OnAccepted(Core::INetwork*) {}
  // Callback functions
  virtual bool OnConnect() ;
  virtual void OnConnected() ;
  virtual bool OnDisconnect() ;
  virtual void OnDisconnected() ;
  virtual bool OnReceive() ;
  virtual bool OnReceived() ;
  virtual bool OnSend(uint8_t* _buffer) ;
  virtual void OnSent() ;
  virtual bool HandlePacket(uint8_t* _buffer) ;

  PacketCodec crypt_;
  Core::INetwork* socket_;
  Entity entity_;
};

}

#endif
