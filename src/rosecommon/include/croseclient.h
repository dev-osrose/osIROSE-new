#ifndef _CROSECLIENT_H_
#define _CROSECLIENT_H_

#include "crosecrypt.h"
#include "cnetwork_asio.h"
#include "crosepacket.h"

namespace RoseCommon {

class CRoseClient : public Core::CNetwork_Asio {
 public:
  CRoseClient();
  CRoseClient(tcp::socket &&_sock);
  virtual ~CRoseClient();

  virtual bool Send(CRosePacket &_buffer);
  virtual bool Send(std::unique_ptr<uint8_t[]> _buffer) override;

 protected:
  // Callback functions
  virtual bool OnConnect() override;
  virtual void OnConnected() override;
  virtual bool OnDisconnect() override;
  virtual void OnDisconnected() override;
  virtual bool OnReceive() override;
  virtual bool OnReceived() override;
  virtual bool OnSend(uint8_t* _buffer) override;
  virtual void OnSent() override;
  virtual bool HandlePacket(uint8_t* _buffer) override;

  PacketCodec crypt_;
};

}

#endif
