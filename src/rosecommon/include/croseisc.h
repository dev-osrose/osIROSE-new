#ifndef _CROSEISC_H_
#define _CROSEISC_H_

#include "croseclient.h"

class CRoseISC : public CRoseClient {
 public:
  CRoseISC();
  CRoseISC(tcp::socket _sock);
  virtual ~CRoseISC();

 protected:
  // Override the callback functions we will use only
  virtual void OnConnected() override;
  virtual bool OnDisconnect() override;
  virtual void OnDisconnected() override;
  virtual bool OnReceived() override;
  virtual bool OnSend(uint8_t* _buffer) override;
  virtual void OnSent() override;
  virtual bool HandlePacket(uint8_t* _buffer) override;
};

#endif
