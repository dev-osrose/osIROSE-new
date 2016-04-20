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
  bool SendCharCreateReply(RoseCommon::CRosePacket* P);
  bool SendCharDeleteReply(RoseCommon::CRosePacket* P);
  bool SendCharSelectReply(RoseCommon::CRosePacket* P);

  uint16_t access_rights_;
};

#endif
