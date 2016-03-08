#include "cloginserver.h"
#include "cloginisc.h"
#include "cloginclient.h"
#include "ePacketType.h"
#include "cdatabase_manager.h"

CLoginClient::CLoginClient() : CRoseClient(), access_rights_(0) {
  log_.SetIdentity("CLoginClient");
}

CLoginClient::CLoginClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0) {
  log_.SetIdentity("CLoginClient");
}

void CLoginClient::SendLoginReply(uint8_t Result) {
  CRosePacket* pak =
      new CRosePacket(ePacketType::PAKLC_LOGIN_REPLY, sizeof(pakLoginReply));
  pak->pLoginReply.Result = Result;
  pak->pLoginReply.Right = 0;
  pak->pLoginReply.Type = 0;

  if (Result == 0) {
    pak->pLoginReply.Right = access_rights_;

    // loop the server list here
    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
    for (auto& server : CLoginServer::GetISCList()) {
      if (server->GetType() == 1) {
        CLoginISC* svr = (CLoginISC*)server;

        // This if check is needed since the client actually looks for this.
        if (svr->IsTestServer())
          pak->Add<uint8_t>('@');
        else
          pak->Add<uint8_t>(' ');

        pak->AddString(svr->GetName().c_str(), true);
        pak->Add<uint32_t>(svr->GetId());
      }
    }
  }

  this->Send(pak);
}

bool CLoginClient::UserLogin(CRosePacket* P) {
  P->GetBytes(0, 32, password_);
  password_[32] = 0;  // Null term the string
  P->GetString(32, 16, (char*)username_);
  // IResult	*res = nullptr;
  std::unique_ptr<Core::IResult> res;
  std::string query = "CALL UserLogin('%s', '%s');";

  // TODO: Create string safe function to sanitize sql query input
  // TODO: Database class needs to become a singleton
  Core::CMySQL_Database& database = CDatabaseMgr::getInstance().GetDatabase();
  res = std::move(database.QStore(query));

  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      //	if(res)
      // 	{
      // 		// Already logged in
      // 		SendLoginReply( 4 );
      // 	}
      //
      // 	{
      // 		// Servers are under inspection
      // 		SendLoginReply( 1 );
      // 	}

      {  // Okay to login!!
        SendLoginReply(0);
      }

      // 	{
      // 		// Banned
      // 		SendLoginReply( 5 );
      // 	}
      //
      // 	{
      // 		// Incorrect Password
      // 		SendLoginReply( 3 );
      // 	}
      //
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
  // m_Log.icprintf( "Channel List\n" );

  uint32_t ServerID = P->pChannelListReq.lServerID;

  CRosePacket* pak = new CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY,
                                     sizeof(pakChannel_List));
  pak->pChannelList.lServerID = ServerID;
  pak->pChannelList.bServerCount = 0;

  pakChannelInfo channel;
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->GetType() == iscPacket::ServerReg_ServerType_CHAR &&
        server->GetId() == ServerID) {
      for (auto& obj : server->GetChannelList()) {
        tChannelInfo info = obj;
        {
          pak->pChannelList.bServerCount += 1;
          channel.ChannelID = info.ChannelID;
          channel.pad = 0;
          channel.Status = 0;

          pak->AddBytes((uint8_t*)&channel, sizeof(pakChannelInfo));
          pak->AddString(info.channelName.c_str(), true);
        }
      }
    }
  }

  this->Send(pak);

  return true;
}

bool CLoginClient::ServerSelect(CRosePacket* P) {
  uint32_t serverID = P->Get<uint32_t>(0);
  // uint8_t channelID = P->Get<uint8_t>( 4 );

  CRosePacket* pak = new CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY);
  pak->Add<uint8_t>(0);         // Not sure what this byte is for
  pak->Add<uint32_t>(GetId());  // Set this to client id
  pak->Add<uint32_t>(
      0);  // Set this to the crypt seed for the server we are connecting to
  std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
  for (auto& obj : CLoginServer::GetISCList()) {
    CLoginISC* server = (CLoginISC*)obj;
    if (server->GetType() == 1 && server->GetId() == serverID) {
      pak->AddString(server->GetIP().c_str(), true);
      pak->Add<uint16_t>(server->GetPort());

      // TODO: send char server the channel id we are connecting to (the client
      // may already do this)
    }
  }
  this->Send(pak);

  return true;
}

bool CLoginClient::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch (pak->Header.Command) {
    case ePacketType::PAKCS_CHANNEL_LIST_REQ:
      return ChannelList(pak);
    case ePacketType::PAKCS_SRV_SELECT_REQ:
      return ServerSelect(pak);
    case ePacketType::PAKCS_LOGIN_REQ:
      return UserLogin(pak);

    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CLoginClient::OnReceived() { return CRoseClient::OnReceived(); }
