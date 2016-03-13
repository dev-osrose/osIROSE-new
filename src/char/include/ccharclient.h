#ifndef __CCHARCLIENT_H__
#define __CCHARCLIENT_H__

#include "croseclient.h"
#include "crosepacket.h"

class CCharClient : public RoseCommon::CRoseClient {
 public:
  CCharClient();
  CCharClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();

  bool JoinServerReply(RoseCommon::CRosePacket* P);
  bool SendCharListReply(RoseCommon::CRosePacket* P);
  bool SendCharCreateReply(RoseCommon::CRosePacket* P);
  bool SendCharDeleteReply(RoseCommon::CRosePacket* P);
  bool SendCharSelectReply(RoseCommon::CRosePacket* P);

  uint16_t access_rights_;
};

#endif
