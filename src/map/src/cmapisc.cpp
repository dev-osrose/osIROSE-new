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
#include "crosepacket.h"
#include "config.h"
#include "isc_serverregister.h"
#include "isc_shutdown.h"
#include "platform_defines.h"

using namespace RoseCommon;

CMapISC::CMapISC() : CRoseISC() {
//  socket_->set_type(iscPacket::ServerType::MAP_MASTER);
}

CMapISC::CMapISC(std::unique_ptr<Core::INetwork> _sock)
    : CRoseISC(std::move(_sock)) {
  socket_->set_type(to_underlying(Isc::ServerType::MAP_MASTER));
  socket_->registerOnConnected(std::bind(&CMapISC::OnConnected, this));
  socket_->registerOnShutdown(std::bind(&CMapISC::OnShutdown, this));
}

bool CMapISC::IsChar() const { return socket_->get_type() == Isc::ServerType::CHAR; }

bool CMapISC::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(
          getPacket<ePacketType::ISC_SERVER_REGISTER>(_buffer));
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return true;
    default: {
      CRoseISC::HandlePacket(_buffer);
      return false;
    }
  }
  return true;
}

bool CMapISC::ServerRegister(
    std::unique_ptr<RoseCommon::IscServerRegister> P) {
  logger_->trace("CMapISC::ServerRegister(CRosePacket* P)");

//  int16_t _type = 0;
//  _type = pMapServer.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

//  iscPacket::ServerReg pServerReg;
//  std::string name, ip;
//  int32_t port = 0, type = 0, right = 0;

//  if (_type == iscPacket::ServerType::NODE) {
//    // This is a node and we need to figure out something to do with this
//  } else if (_type == iscPacket::ServerType::MAP_MASTER) {
//    name = pMapServer.name();
//    ip = pMapServer.addr();
//    port = pMapServer.port();
//    type = pMapServer.type();
//    right = pMapServer.accright();
//  } else if (_type == iscPacket::ServerType::MAP_WORKER) {
//    name = pMapServer.name();
//    ip = pMapServer.addr();
//    port = pMapServer.port();
//    type = pMapServer.type();
//    right = pMapServer.accright();
//    this->set_type(_type);
//  }

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                RoseCommon::Isc::serverTypeName(P->serverType()),
                  P->name(), P->addr(),
                  P->port());
  return false;
}

void CMapISC::OnConnected() {
  Core::Config& config = Core::Config::getInstance();
  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(
      RoseCommon::Isc::ServerType::MAP_MASTER,
      config.mapServer().channelName, config.serverData().ip,
      config.mapServer().clientPort,
      config.mapServer().accessLevel,
      get_id());

  logger_->trace("Sending a packet on CMapISC: Header[{0}, 0x{1:x}]",
                 packet->size(), static_cast<uint16_t>(packet->type()));
  send(*packet);

  if (socket_->process_thread_.joinable() == false) {
    socket_->process_thread_ = std::thread([this]() {
		  while (is_active() == true && IsChar() == true) {
			  std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
			  int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
				  update - get_update_time())
				  .count();
			  if (dt > (1000 * 60) * 1)  // wait 1 minute before pinging
			  {
				  logger_->trace("Sending ISC_ALIVE");
				  auto packet = std::make_unique<CRosePacket>( ePacketType::ISC_ALIVE );
				  send(*packet);
			  }
			  std::this_thread::sleep_for(
				  std::chrono::milliseconds(1000));
		  }
		  return 0;
	  });
  }
}

bool CMapISC::OnShutdown() {
	logger_->trace("CCharISC::OnDisconnected()");
	bool result = true;

	if (is_active() == true) {
		if (get_type() == RoseCommon::Isc::ServerType::CHAR) {
			if (socket_->reconnect() == true) {
				logger_->info("Reconnected to character server.");
				result = false;
			}
		}
		else {
			auto packet = makePacket<ePacketType::ISC_SHUTDOWN>();
			std::lock_guard<std::mutex> lock(CMapServer::GetISCListMutex());
			for (auto& server : CMapServer::GetISCList()) {
				CMapISC* svr = static_cast<CMapISC*>(server.get());
                if (!svr) {
                    continue;
                }
                if (svr->get_type() == RoseCommon::Isc::ServerType::CHAR) {
					svr->send(*packet);
					break;
				}
			}
		}
	}
	return result;
}
