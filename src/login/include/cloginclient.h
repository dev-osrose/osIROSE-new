#ifndef _CLOGINCLIENT_H_
#define _CLOGINCLIENT_H_

#include "croseclient.h"
#include "crosepacket.h"

class CLoginClient : public RoseCommon::CRoseClient {
 public:
  CLoginClient();
  CLoginClient(tcp::socket _sock);

 protected:
  virtual bool HandlePacket(uint8_t* _buffer);
  virtual bool OnReceived();

  // Packet Helper Functions
  bool UserLogin(RoseCommon::CRosePacket* P);
  bool ChannelList(RoseCommon::CRosePacket* P);
  bool ServerSelect(RoseCommon::CRosePacket* P);

  void SendLoginReply(uint8_t Result);

  uint16_t access_rights_;
  uint8_t username_[17];
};

#endif
