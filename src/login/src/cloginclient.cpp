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

#include "cloginserver.h"
#include "cloginisc.h"
#include "cloginclient.h"
#include "database.h"

using namespace RoseCommon;

CLoginClient::CLoginClient()
    : CRoseClient(),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      userid_(0),
      session_id_(0) {}

CLoginClient::CLoginClient(Core::INetwork* _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      userid_(0),
      session_id_(0) {}

void CLoginClient::SendLoginReply(uint8_t Result) {
  logger_->debug("SendLoginReply({})", Result);
  auto packet = makePacket<ePacketType::PAKLC_LOGIN_REPLY>(Result, 0, 0);

  if (Result == 0) {
    login_state_ = eSTATE::LOGGEDIN;
    packet->setRight(access_rights_);

    // loop the server list here
    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
    for (auto& server : CLoginServer::GetISCList())
      if (server->get_type() == iscPacket::ServerType::CHAR) {
        CLoginISC* svr = (CLoginISC*)server;

        // This if check is needed since the client actually looks for this.
        packet->addServer(svr->GetName(), svr->get_id() + 1,
                          svr->IsTestServer());
      }
  }

  this->send(*packet);
}

bool CLoginClient::UserLogin(std::unique_ptr<RoseCommon::CliLoginReq> P) {
  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  get_id());
    return true;
  }
  uint32_t serverCount = 0;

  CLoginServer::GetISCListMutex().lock();
  for (auto& server : CLoginServer::GetISCList()) {
    if (server->get_type() == iscPacket::ServerType::CHAR) serverCount++;
  }
  CLoginServer::GetISCListMutex().unlock();

  logger_->debug("Found {} type 1 (CHAR) servers", serverCount);

  if (serverCount < 1) {
    // Servers are under inspection
    SendLoginReply(SrvLoginReply::FAILED);
    return true;
  }

  username_ = Core::CMySQL_Database::escapeData(P->username());
  std::string clientpass = Core::CMySQL_Database::escapeData(P->password());

  std::unique_ptr<Core::IResult> res;
  std::string query = fmt::format("CALL user_login('{0}', '{1}');", username_.c_str(), clientpass.c_str());

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);

  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
        uint32_t onlineStatus = 0, accessRights = 0;
        res->getInt("online", onlineStatus);
        if (res->getInt("access", accessRights)) {
          access_rights_ = accessRights;
        }

        if (access_rights_ < 1) {
          // Banned
          SendLoginReply(SrvLoginReply::NO_RIGHT_TO_CONNECT);
          return false;
        }

        if (onlineStatus == 0) {
          // Okay to login!!
          res->getInt("id", userid_);
          session_id_ = std::time(nullptr);
          SendLoginReply(SrvLoginReply::OK);
        } else {
          // Online already
          SendLoginReply(SrvLoginReply::ALREADY_LOGGEDIN);
        }
    } else {
        if ((res = database.QStore(fmt::format("CALL user_exists('{}');", username_.c_str()))) && res->size())
            SendLoginReply(SrvLoginReply::INVALID_PASSWORD);
        else if (res)
          // The user doesn't exist or server is down.
          SendLoginReply(SrvLoginReply::UNKNOWN_ACCOUNT);
        else
            SendLoginReply(SrvLoginReply::FAILED);
    }
  } else {
    SendLoginReply(SrvLoginReply::FAILED);
  }
  return true;
}

bool CLoginClient::ChannelList(std::unique_ptr<RoseCommon::CliChannelListReq> P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to get channel list before logging in.",
        get_id());
    return true;
  }
  uint32_t ServerID = P->serverId()-1;

  auto packet = makePacket<ePacketType::PAKLC_CHANNEL_LIST_REPLY>(ServerID+1);
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->get_type() == iscPacket::ServerType::CHAR &&
        server->get_id() == ServerID) {
      for (auto& obj : server->GetChannelList()) {
        tChannelInfo info = obj;
        { packet->addChannel(info.channelName, info.ChannelID+1, 0); }
      }
    }
  }

  this->send(*packet);
  logger_->trace("Client {}: Channel List end.", get_id());
  return true;
}

bool CLoginClient::ServerSelect(
    std::unique_ptr<RoseCommon::CliSrvSelectReq> P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to select a server before logging in.",
        get_id());
    return true;
  }
  uint32_t serverID = P->serverId()-1;
  uint8_t channelID = P->channelId()-1;
  login_state_ = eSTATE::TRANSFERING;

  // 0 = Good to go
  // 1 = Failed
  // 2 = Full
  // 3 = Invalid channel
  // 4 = Channel not active
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->get_type() == iscPacket::ServerType::CHAR &&
        server->get_id() == serverID) {
      std::string query = fmt::format("CALL create_session({}, {}, {});",
                                      session_id_, userid_, channelID);

      Core::IDatabase& database = Core::databasePool.getDatabase();
      database.QExecute(query);

      auto packet = makePacket<ePacketType::PAKLC_SRV_SELECT_REPLY>(
          SrvSrvSelectReply::OK, session_id_, 0, server->get_address(), server->get_port());
      this->send(*packet);
      break;
    }
  }
  logger_->trace("Client {}: Server Select end.", get_id());
  return true;
}

bool CLoginClient::HandlePacket(uint8_t* _buffer) {
  logger_->trace("CLoginClient::HandlePacket start");
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_CHANNEL_LIST_REQ:
      return ChannelList(
          getPacket<ePacketType::PAKCS_CHANNEL_LIST_REQ>(_buffer));
    case ePacketType::PAKCS_SRV_SELECT_REQ:
      return ServerSelect(
          getPacket<ePacketType::PAKCS_SRV_SELECT_REQ>(_buffer));
    case ePacketType::PAKCS_LOGIN_REQ:
      return UserLogin(getPacket<ePacketType::PAKCS_LOGIN_REQ>(_buffer));

    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  logger_->trace("CLoginClient::HandlePacket end");
  return true;
}
