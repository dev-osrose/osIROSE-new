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

#include "cloginclient.h"
#include "cloginserver.h"
#include "cloginisc.h"
#include "croseisc.h"
#include "packetfactory.h"
#include "isccommon.h"
#include "isc_server_register.h"
#include "cli_login_req.h"
#include "connection.h"
#include "connectionpool.h"
#include "croseserver.h"
#include "cli_channel_list_req.h"
#include "cli_srv_select_req.h"
#include "srv_srv_select_reply.h"
#include "epackettype.h"
#include "srv_channel_list_reply.h"
#include "config.h"

using namespace RoseCommon;

CLoginClient::CLoginClient()
    : CRoseClient(),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      userid_(0),
      session_id_(0),
      server_(nullptr) {}

CLoginClient::CLoginClient(CLoginServer* server, std::unique_ptr<Core::INetwork> _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      userid_(0),
      session_id_(0),
      server_(server) {}

void CLoginClient::sendLoginReply(Packet::SrvLoginReply::Result Result) {
  logger_->debug("sendLoginReply({})", Result);
  auto packet = Packet::SrvLoginReply::create(Result, 0, 0);

  if (Result == Packet::SrvLoginReply::OK) {
    login_state_ = eSTATE::LOGGEDIN;
    packet.set_right(access_rights_);

    // loop the server list here
    std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
    for (auto& server : server_->GetISCList()) {
      if (server->get_type() == Isc::ServerType::CHAR) {
        CLoginISC* svr = static_cast<CLoginISC*>(server.get());
        if (!svr) {
            continue;
        }

        // This if check is needed since the client actually looks for this.
        auto info = Packet::SrvLoginReply::ServerInfo();
        info.set_name(svr->getName());
        info.set_id(svr->get_id() + 1);
        info.set_test(svr->isTestServer() ? '@' : ' ');
        packet.add_serversInfo(info);
      }
    }
  }

  send(packet);
}

bool CLoginClient::userLogin(RoseCommon::Packet::CliLoginReq&& P) {
  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  get_id());
    return true;
  }
  uint32_t serverCount = 0;

  server_->GetISCListMutex().lock();
  for (auto& server : server_->GetISCList()) {
    if (server && server->get_type() == Isc::ServerType::CHAR) serverCount++;
  }
  server_->GetISCListMutex().unlock();

  logger_->debug("Found {} type 1 (CHAR) servers", serverCount);

  if (serverCount < 1) {
    // Servers are under inspection
    sendLoginReply(Packet::SrvLoginReply::FAILED);
    return true;
  }

  username_ = Core::escapeData(P.get_username());
  std::string clientpass = Core::escapeData(P.get_password());

  logger_->debug("Client sent '{}' as the password", clientpass);

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::AccountTable table{};
  Core::SessionTable session{};
  try {
    const auto res = conn(sqlpp::select(table.id, table.password, table.access, table.active, table.online, table.loginCount)
              .from(table).where(table.accountType == "user" and table.username == username_
                  and table.password == sqlpp::verbatim<sqlpp::varchar>(fmt::format("SHA2(CONCAT('{}', salt), 256)", clientpass))));

        if (!res.empty()) {
            const auto &row = res.front();
            const auto ses = conn(sqlpp::select(session.id).from(session).where(session.userid == row.id));
            if (!row.access.is_null())
                access_rights_ = row.access;

            if (access_rights_ < 1) {
                // Banned
                sendLoginReply(Packet::SrvLoginReply::NO_RIGHT_TO_CONNECT);
                return false;
            }

            if (!row.online && ses.empty()) {
                // Okay to login!!
                userid_ = row.id;
                session_id_ = std::time(nullptr);
                conn(sqlpp::update(table).set(table.online = 1,
                                              table.loginCount = row.loginCount + 1,
                                              table.lastip = get_address(),
                                              table.lasttime = std::chrono::system_clock::now())
                     .where(table.id == userid_));
                this->set_name(username_);
                sendLoginReply(Packet::SrvLoginReply::OK);
            } else {
                // Online already
                sendLoginReply(Packet::SrvLoginReply::ALREADY_LOGGEDIN);
            }
        } else {
            if (!conn(sqlpp::select(table.id).from(table).where(table.username == username_)).empty())
                sendLoginReply(Packet::SrvLoginReply::INVALID_PASSWORD);
            else {
                // The user doesn't exist or server is down.
                auto& config = Core::Config::getInstance();
                if (config.loginServer().createAccountOnFail) {
                  logger_->debug("Creating account");
                    std::string query = fmt::format("CALL create_account('{}', '{}');", username_, clientpass);
                    conn->execute(query);
                }
                sendLoginReply(Packet::SrvLoginReply::UNKNOWN_ACCOUNT);
            }
        }
  } catch (const sqlpp::exception &e) {
    logger_->error("Error while accessing the database: {}", e.what());
        sendLoginReply(Packet::SrvLoginReply::FAILED);
  }
  return true;
}

void CLoginClient::onDisconnected() {
  Core::SessionTable session{};
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  auto res = conn(sqlpp::select(session.id).from(session)
                  .where(session.id == session_id_));
  if (res.empty()) {
    Core::AccountTable account{};
    conn(sqlpp::update(account).set(account.online = 0)
         .where(account.id == userid_));
  }
}

bool CLoginClient::channelList(RoseCommon::Packet::CliChannelListReq&& P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to get channel list before logging in.",
        get_id());
    return true;
  }
  uint32_t ServerID = P.get_serverId()-1;

  auto packet = Packet::SrvChannelListReply::create(ServerID+1);
  std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
  for (auto& obj : server_->GetISCList()) {
    CLoginISC* server = static_cast<CLoginISC*>(obj.get());
    if (!server) {
        continue;
    }
    if (server->get_type() == Isc::ServerType::CHAR &&
        server->get_id() == ServerID) {
      for (tChannelInfo& info : server->getChannelList()) {
        auto channel = Packet::SrvChannelListReply::ChannelInfo();
        channel.set_id(info.ChannelID + 1);
        channel.set_name(info.channelName);
        channel.set_lowAge(0);
        channel.set_highAge(0);
        channel.set_capacity(0);
        packet.add_channels(channel);
      }
    }
  }

  this->send(packet);
  logger_->trace("Client {}: Channel List end.", get_id());
  return true;
}

bool CLoginClient::serverSelect(RoseCommon::Packet::CliSrvSelectReq&& P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to select a server before logging in.",
        get_id());
    return true;
  }
  uint32_t serverID = P.get_serverId()-1;
  uint8_t channelID = P.get_channelId()-1;
  login_state_ = eSTATE::TRANSFERING;

  // 0 = Good to go
  // 1 = Failed
  // 2 = Full
  // 3 = Invalid channel
  // 4 = Channel not active
  std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
  for (auto& obj : server_->GetISCList()) {
    CLoginISC* server = static_cast<CLoginISC*>(obj.get());
    if (server->get_type() == Isc::ServerType::CHAR &&
        server->get_id() == serverID) {
      Core::SessionTable session{};
      auto conn = Core::connectionPool.getConnection(Core::osirose);
      conn(sqlpp::insert_into(session).set(
                    session.id = session_id_,
                    session.userid = userid_,
                    session.channelid = channelID,
                    session.time = std::chrono::system_clock::now()));

      auto packet = Packet::SrvSrvSelectReply::create(
          Packet::SrvSrvSelectReply::OK, session_id_, 0,
          server->get_address(), server->get_port());
      this->send(packet);
      break;
    }
  }
  logger_->trace("Client {}: Server Select end.", get_id());
  return true;
}

bool CLoginClient::handlePacket(uint8_t* _buffer) {
  logger_->trace("CLoginClient::handlePacket start");
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_CHANNEL_LIST_REQ:
      return channelList(
          Packet::CliChannelListReq::create(_buffer));
    case ePacketType::PAKCS_SRV_SELECT_REQ:
      return serverSelect(
          Packet::CliSrvSelectReq::create(_buffer));
    case ePacketType::PAKCS_LOGIN_REQ:
      return userLogin(Packet::CliLoginReq::create(_buffer));

    default:
      return CRoseClient::handlePacket(_buffer);
  }
  logger_->trace("CLoginClient::handlePacket end");
  return true;
}
