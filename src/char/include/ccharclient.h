#ifndef __CCHARCLIENT_H__
#define __CCHARCLIENT_H__

#include "croseclient.h"
#include "crosepacket.h"

class CCharClient : public CRoseClient {
 public:
  CCharClient();
  CCharClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();

  bool JoinServerReply(CRosePacket* P);
  bool SendCharListReply(CRosePacket* P);
  bool SendCharCreateReply(CRosePacket* P);
  bool SendCharDeleteReply(CRosePacket* P);
  bool SendCharSelectReply(CRosePacket* P);

  uint16_t access_rights_;
};

#endif
