#ifndef __CCHARISC_H__
#define __CCHARISC_H__

#include "croseisc.h"
#include "crosepacket.h"
#include "packetclasses.h"

class CCharISC : public RoseCommon::CRoseISC {
 public:
  CCharISC();
  CCharISC(tcp::socket _sock);

  bool IsLogin() const;
  void SetLogin(bool val);

 protected:
  bool ServerRegister(std::unique_ptr<RoseCommon::IscServerRegister> P);
  bool HandlePacket(uint8_t* _buffer);

  virtual void OnConnected();
  virtual bool OnShutdown() override;

  int32_t type_;
};

#endif
