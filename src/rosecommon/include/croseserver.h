#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include <list>
#include <forward_list>
#include "cnetwork_asio.h"
#include "croseclient.h"
#include "croseisc.h"

class CRoseServer : public Core::CNetwork_Asio {
 public:
  CRoseServer(bool _iscServer = false);
  virtual ~CRoseServer();

  virtual bool Run();

  bool IsISCServer() { return isc_server_; }

  static std::forward_list<CRoseClient*>& GetClientList() {
    return client_list_;
  }
  static std::forward_list<CRoseISC*>& GetISCList() { return isc_list_; }
  static std::mutex& GetClientListMutex() { return client_list_mutex_; }
  static std::mutex& GetISCListMutex() { return isc_list_mutex_; }

 protected:
  // Callback functions
  virtual bool OnConnect();
  virtual void OnConnected();
  virtual bool OnListen();
  virtual void OnListening();
  virtual bool OnDisconnect();
  virtual void OnDisconnected();
  virtual bool OnAccept();
  virtual void OnAccepted(tcp::socket _sock);

  bool isc_server_;
  static std::forward_list<CRoseClient*> client_list_;
  static std::forward_list<CRoseISC*> isc_list_;
  static std::mutex client_list_mutex_;
  static std::mutex isc_list_mutex_;
};

#endif
