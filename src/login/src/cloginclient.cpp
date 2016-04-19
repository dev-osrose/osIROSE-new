#include "cloginserver.h"
#include "cloginisc.h"
#include "cloginclient.h"
#include "epackettype.h"
#include "rosepackets.h"
#include "database.h"
#include "packetclasses.h"
#include "iscpackets.pb.h"

using namespace RoseCommon;

CLoginClient::CLoginClient()
    : CRoseClient(), access_rights_(0), login_state_(eSTATE::DEFAULT) {}

CLoginClient::CLoginClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT) {}

void CLoginClient::SendLoginReply(uint8_t Result) {
  auto packet = makePacket<ePacketType::PAKLC_LOGIN_REPLY>(Result, 0, 0);

  if (Result == 0) {
    login_state_ = eSTATE::LOGGEDIN;
    packet->setRight(access_rights_);

    // loop the server list here
    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
    for (auto& server : CLoginServer::GetISCList())
      if (server->GetType() == 1) {
        CLoginISC* svr = (CLoginISC*)server;

        // This if check is needed since the client actually looks for this.
        packet->addServer(svr->GetName(), svr->GetId(), svr->IsTestServer());
      }
  }

  this->Send(*packet);
}

bool CLoginClient::UserLogin(std::unique_ptr<RoseCommon::CliLoginReq> P) {
  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  GetId());
    return true;
  }
  uint32_t serverCount = 0;

  CLoginServer::GetISCListMutex().lock();
  for (auto& server : CLoginServer::GetISCList()) {
    if (server->GetType() == 1) serverCount++;
  }
  CLoginServer::GetISCListMutex().unlock();

  logger_->debug("Found {} type 1 (CHAR) servers", serverCount);

  if (serverCount < 1) {
    // Servers are under inspection
    SendLoginReply(1);
    return true;
  }

  username_ = P->username();
  std::string clientpass = P->password();

  // todo(raven): make sure the username is safe to exec
  std::unique_ptr<Core::IResult> res;
  std::string query = "CALL UserLogin('";
  query.append(username_).append("');");

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);

  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      std::string pwd = "";
      res->getString("password", pwd);

      if (pwd == clientpass) {
        uint32_t onlineStatus = 0, accessRights = 0;
        res->getInt("online", onlineStatus);
        if (res->getInt("access", accessRights)) {
          access_rights_ = accessRights;
        }

        if (access_rights_ < 1) {
          // Banned
          SendLoginReply(5);
          return false;
        }

        if (onlineStatus == 0) {
          // Okay to login!!
          SendLoginReply(0);
        } else {
          // incorrect password
          SendLoginReply(4);
        }
      } else {
        // The user doesn't exist or server is down.
        SendLoginReply(3);
      }

      // 	{
      // 		// Server Full
      // 		SendLoginReply( 8 );
      //	}
    } else {
      // The user doesn't exist or server is down.
      SendLoginReply(1);
    }
  }
  return true;
}

bool CLoginClient::ChannelList(std::unique_ptr<RoseCommon::CliChannelReq> P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to get channel list before logging in.",
        GetId());
    return true;
  }
  uint32_t ServerID = P->server_id();

  auto packet = makePacket<ePacketType::PAKLC_CHANNEL_LIST_REPLY>(ServerID);
  /* pakChannelInfo channel; */
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->GetType() == iscPacket::ServerReg_ServerType_CHAR &&
        server->GetId() == ServerID) {
      for (auto& obj : server->GetChannelList()) {
        tChannelInfo info = obj;
        { packet->addChannel(info.channelName, info.ChannelID, 0); }
      }
    }
  }

  this->Send(*packet);
  logger_->trace("Client {}: Channel List end.", GetId());
  return true;
}

bool CLoginClient::ServerSelect(
    std::unique_ptr<RoseCommon::CliServerSelectReq> P) {
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to select a server before logging in.",
        GetId());
    return true;
  }
  uint32_t serverID = P->server_id();
  //uint8_t channelID = P->channel_id();
  login_state_ = eSTATE::TRANSFERING;

  // 0 = Good to go
  // 1 = Failed
  // 2 = Full
  // 3 = Invalid channel
  // 4 = Channel not active
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->GetType() == 1 && server->GetId() == serverID) {
      auto packet = makePacket<ePacketType::PAKLC_SRV_SELECT_REPLY>(
          server->GetIP(), GetId(), 0, server->GetPort());
      this->Send(*packet);
      // TODO: send char server the channel id we are connecting to (the client
      // may already do this)
      break;
    }
  }
  logger_->trace("Client {}: Server Select end.", GetId());
  return true;
}

bool CLoginClient::HandlePacket(uint8_t* _buffer) {
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
  return true;
}

bool CLoginClient::OnReceived() { return CRoseClient::OnReceived(); }
