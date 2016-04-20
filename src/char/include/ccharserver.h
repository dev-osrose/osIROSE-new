#ifndef __CCHARSERVER_H__
#define __CCHARSERVER_H__

#include "croseserver.h"

class CCharServer : public RoseCommon::CRoseServer {
 public:
  CCharServer(bool _isc = false);
  virtual ~CCharServer();

 protected:
  virtual void OnAccepted(tcp::socket _sock);
  uint32_t client_count_;
  uint32_t server_count_;
};

#endif
