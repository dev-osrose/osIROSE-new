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

#include "crosecrypt.h"
#include "cnetwork_asio.h"
#include "crosepacket.h"
#include "entityComponents.h"

namespace RoseCommon {

class CRoseClient : public Core::CNetwork_Asio {
 public:
  CRoseClient();
  CRoseClient(int* _sock);
  virtual ~CRoseClient();

  virtual bool Send(CRosePacket &_buffer);
  virtual bool Send(std::unique_ptr<uint8_t[]> _buffer) override;

  virtual bool IsNearby(const CRoseClient* _otherClient) const;

  Entity getEntity() const { return entity_; }

  virtual uint32_t GetObjId() const {
    return CRoseClient::GetId();
  }

 protected:

  virtual void OnAccepted(int*) {}
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

  Entity entity_;
};

}

#endif
