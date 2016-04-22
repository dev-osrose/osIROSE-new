#ifndef _CLOGINCLIENT_H_
#define _CLOGINCLIENT_H_

#include "croseclient.h"
#include "rosepackets.h"

class CLoginClient : public RoseCommon::CRoseClient {
 public:
  CLoginClient();
  CLoginClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();

  // Packet Helper Functions
  bool UserLogin(std::unique_ptr<RoseCommon::CliLoginReq> P);
  bool ChannelList(std::unique_ptr<RoseCommon::CliChannelReq> P);
  bool ServerSelect(std::unique_ptr<RoseCommon::CliServerSelectReq> P);

  void SendLoginReply(uint8_t Result);

  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
    TRANSFERING,
  };

  uint16_t access_rights_;
  std::string username_;
  eSTATE login_state_;
  uint32_t userid_;
  uint32_t session_id_;
};

#endif
