#ifndef __CCHARSERVER_H__
#define __CCHARSERVER_H__

#include "croseserver.h"

class CCharServer : public CRoseServer {
 public:
  CCharServer(bool _isc = false);
  virtual ~CCharServer();

 protected:
  virtual void OnAccepted(tcp::socket _sock);
};

#endif
