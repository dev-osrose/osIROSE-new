#include "cloginserver.h"
#include "cloginisc.h"
#include "cloginclient.h"
#include "epackettype.h"
#include "rosepackets.h"
#include "database.h"

using namespace RoseCommon;

CLoginClient::CLoginClient() : CRoseClient(), access_rights_(0), login_state_(eSTATE::DEFAULT) {
}

CLoginClient::CLoginClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0), login_state_(eSTATE::DEFAULT) {
}

void CLoginClient::SendLoginReply(uint8_t Result) {
  /* CRosePacket* pak = */
  /*     new CRosePacket(ePacketType::PAKLC_LOGIN_REPLY, sizeof(pakLoginReply)); */
  /* pak->pLoginReply.Result = Result; */
  /* pak->pLoginReply.Right = 0; */
  /* pak->pLoginReply.Type = 0; */

  if (Result == 0) {
    login_state_ = eSTATE::LOGGEDIN;
//    pak->pLoginReply.Right = access_rights_;

    // loop the server list here
    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
    /* for (auto& server : CLoginServer::GetISCList()) { */
      /* if (server->GetType() == 1) { */
        /* CLoginISC* svr = (CLoginISC*)server; */

        // This if check is needed since the client actually looks for this.
        /* if (svr->IsTestServer()) */
        /*   pak->Add<uint8_t>('@'); */
        /* else */
        /*   pak->Add<uint8_t>(' '); */

        /* pak->AddString(svr->GetName().c_str(), true); */
        /* pak->Add<uint32_t>(svr->GetId()); */
      /* } */
    /* } */
  }

  /* this->Send(pak); */
}

bool CLoginClient::UserLogin(CRosePacket* P) {
  if(login_state_ != eSTATE::DEFAULT)
  {
    logger_->warn("Client {} is attempting to login when already logged in.", GetId());
    return true;
  }
	(void)P;

  uint32_t serverCount = 0;

  CLoginServer::GetISCListMutex().lock();
  for (auto& server : CLoginServer::GetISCList()) {
    if (server->GetType() == 1) serverCount++;
  }
  CLoginServer::GetISCListMutex().unlock();

  logger_->debug("Found {} type 1 (CHAR) servers", serverCount);

  if (serverCount < 1)
  {
    // Servers are under inspection
    SendLoginReply( 1 );
    return true;
  }

  uint8_t password_[33];
  /* P->GetBytes(0, 32, password_); */
  password_[32] = 0;  // Null term the string

  std::string clientpass((char*)password_);
  /* P->GetString(32, 16, (char*)username_); */

  //todo(raven): make sure the username is safe to exec
  std::unique_ptr<Core::IResult> res;
  std::string query = "CALL UserLogin('";
  query.append( (char*)username_ ).append("');");

  Core::IDatabase &database = Core::databasePool.getDatabase();
  res = database.QStore(query);

  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      std::string pwd = "";
      res->getString("password", pwd);

      if (pwd == clientpass)
      {
        uint32_t onlineStatus = 0, accessRights = 0;
        res->getInt( "online", onlineStatus );
        if (res->getInt( "access", accessRights ))
        {
          access_rights_ = accessRights;
        }

        if(access_rights_ < 1)
        {
          // Banned
          SendLoginReply( 5 );
          return false;
        }

        if(onlineStatus == 0)
        {
          // Okay to login!!
          SendLoginReply( 0 );
        }
        else
        {
          // incorrect password
          SendLoginReply( 4 );
        }
      }
      else
      {
        // The user doesn't exist or server is down.
        SendLoginReply( 3 );
      }

      // 	{
      // 		// Server Full
      // 		SendLoginReply( 8 );
      //	}
    } else {
      // The user doesn't exist or server is down.
      SendLoginReply( 1 );
    }
  }
  return true;
}

bool CLoginClient::ChannelList(CRosePacket* P) {
  if(login_state_ != eSTATE::LOGGEDIN)
  {
    logger_->warn("Client {} is attempting to get channel list before logging in.", GetId());
    return true;
  }
  (void)P;
  /* uint32_t ServerID = P->pChannelListReq.lServerID; */

  /* CRosePacket* pak = new CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY, */
                                     /* sizeof(pakChannel_List)); */
  /* pak->pChannelList.lServerID = ServerID; */
  /* pak->pChannelList.bServerCount = 0; */
  /* pakChannelInfo channel; */
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  /* for (auto& obj : CLoginServer::GetISCList()) { */
  /*   CLoginISC* server = (CLoginISC*)obj; */
    /* if (server->GetType() == iscPacket::ServerReg_ServerType_CHAR && */
    /*     server->GetId() == ServerID) { */
      /* for (auto& obj : server->GetChannelList()) { */
      /*   tChannelInfo info = obj; */
        /* { */
          /* pak->pChannelList.bServerCount += 1; */
          /* channel.ChannelID = info.ChannelID; */
          /* channel.pad = 0; */
          /* channel.Status = 0; */

          /* pak->AddBytes((uint8_t*)&channel, sizeof(pakChannelInfo)); */
          /* pak->AddString(info.channelName.c_str(), true); */
        /* } */
      /* } */
    /* } */
  /* } */

  /* this->Send(pak); */
  logger_->trace("Client {}: Channel List end.", GetId());
  return true;
}

bool CLoginClient::ServerSelect(CRosePacket* P) {
  if(login_state_ != eSTATE::LOGGEDIN)
  {
    logger_->warn("Client {} is attempting to select a server before logging in.", GetId());
    return true;
  }
  (void)P;
//  uint32_t serverID = P->Get<uint32_t>(0);
  //uint8_t channelID = P->Get<uint8_t>( 4 );
  login_state_ = eSTATE::TRANSFERING;

//  CRosePacket* pak = new CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY);

  // 0 = Good to go
  // 1 = Failed
  // 2 = Full
  // 3 = Invalid channel
  // 4 = Channel not active
  /* pak->Add<uint8_t>( 0 );         // Server select result */
  /* pak->Add<uint32_t>( GetId() );  // Set this to the client id */
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
//  for (auto& obj : CLoginServer::GetISCList()) {
//    CLoginISC* server = (CLoginISC*)obj;
//    if (server->GetType() == 1 && server->GetId() == serverID) {
//      pak->Add<uint32_t>(0);  // Set this to the crypt seed for the server we are connecting to
//      pak->AddString(server->GetIP().c_str(), true);
//      pak->Add<uint16_t>(server->GetPort());

      // TODO: send char server the channel id we are connecting to (the client
      // may already do this)
//      break;
//    }
//  }
//  this->Send(pak);
  logger_->trace("Client {}: Server Select end.", GetId());
  return true;
}

bool CLoginClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
//    case ePacketType::PAKCS_CHANNEL_LIST_REQ:
//      return ChannelList( getPacket<ePacketType::PAKCS_CHANNEL_LIST_REQ>(_buffer) );
//    case ePacketType::PAKCS_SRV_SELECT_REQ:
//      return ServerSelect( getPacket<ePacketType::PAKCS_SRV_SELECT_REQ>(_buffer) );
//    case ePacketType::PAKCS_LOGIN_REQ:
//      return UserLogin( getPacket<ePacketType::PAKCS_LOGIN_REQ>(_buffer) );

    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CLoginClient::OnReceived() { return CRoseClient::OnReceived(); }
