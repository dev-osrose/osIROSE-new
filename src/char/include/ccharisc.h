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

#ifndef __CCHARISC_H__
#define __CCHARISC_H__

#include "croseisc.h"
#include "cli_login_req.h"
#include "isc_server_register.h"
#include "isc_transfer.h"
#include "isc_transfer_char.h"

class CCharServer;

class CCharISC : public RoseCommon::CRoseISC {
 public:
  CCharISC();
  CCharISC(CCharServer* server, std::unique_ptr<Core::INetwork> _sock);

  bool isLogin() const;
  void setLogin(bool val);

 protected:
  bool serverAuth(RoseCommon::Packet::CliLoginReq&& P);
  bool serverRegister(RoseCommon::Packet::IscServerRegister&& P);
  bool handlePacket(uint8_t* _buffer) override;
  bool transfer_packet(RoseCommon::Packet::IscTransfer&& P);
  bool transfer_char_packet(RoseCommon::Packet::IscTransferChar&& P);

  virtual void onConnected() override;
  virtual bool onShutdown() override;
  
  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
    REGISTERED,
  };

  eSTATE state_;

 private:
  CCharServer *server_;
};

#endif
