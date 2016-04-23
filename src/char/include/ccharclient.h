#ifndef __CCHARCLIENT_H__
#define __CCHARCLIENT_H__

#include "croseclient.h"
#include "crosepacket.h"
#include "packetclasses.h"

class CCharClient : public RoseCommon::CRoseClient {
 public:
  CCharClient();
  CCharClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();

  bool JoinServerReply(std::unique_ptr<RoseCommon::CliJoinServerReq> P);
  bool SendCharListReply();
  bool SendCharCreateReply(std::unique_ptr<RoseCommon::CliCreateCharReq> P);
  bool SendCharDeleteReply(std::unique_ptr<RoseCommon::CliDeleteCharReq> P);
  bool SendCharSelectReply(std::unique_ptr<RoseCommon::CliSelectCharReq> P);

  enum class eSTATE {
    DEFAULT,
    LOGGEDIN,
    TRANSFERING,
  };

  uint16_t access_rights_;
  eSTATE login_state_;
  uint32_t session_id_;
  uint32_t userid_;
  uint32_t channelid_;
};

#endif
