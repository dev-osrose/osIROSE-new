#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include <list>
#include <forward_list>
#include "cnetwork_asio.h"
#include "croseclient.h"
#include "croseisc.h"

namespace RoseCommon {

class CRoseServer : public Core::CNetwork_Asio {
 public:
  CRoseServer(bool _iscServer = false);
  virtual ~CRoseServer();

  bool IsISCServer() { return isc_server_; }

  static std::forward_list<CRoseClient*>& GetClientList() {
    return client_list_;
  }
  static std::forward_list<CRoseISC*>& GetISCList() { return isc_list_; }
  static std::mutex& GetClientListMutex() { return client_list_mutex_; }
  static std::mutex& GetISCListMutex() { return isc_list_mutex_; }

 protected:
  // Callback functions
  virtual bool OnConnect() override;
  virtual void OnConnected() override;
  virtual bool OnListen() override;
  virtual void OnListening() override;
  virtual bool OnDisconnect() override;
  virtual void OnDisconnected() override;
  virtual bool OnAccept() override;
  virtual void OnAccepted(tcp::socket _sock) override;

  bool isc_server_;
  static std::forward_list<CRoseClient*> client_list_;
  static std::forward_list<CRoseISC*> isc_list_;
  static std::mutex client_list_mutex_;
  static std::mutex isc_list_mutex_;
};

}

#endif
