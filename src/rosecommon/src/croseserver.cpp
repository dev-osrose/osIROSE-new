#include <stdint.h>
#include "logconsole.h"
#include "croseserver.h"

std::forward_list<CRoseClient*> CRoseServer::client_list_;
std::forward_list<CRoseISC*> CRoseServer::isc_list_;
std::mutex CRoseServer::client_list_mutex_;
std::mutex CRoseServer::isc_list_mutex_;

CRoseServer::CRoseServer(bool _iscServer) : isc_server_(_iscServer) {
  log_.SetIdentity("CRoseServer");
  //  process_thread_ = std::thread([this]() { Run(); });
}

CRoseServer::~CRoseServer() {
  // std::lock_guard< std::mutex > lock( m_ClientListMutex );
  // for(uint32_t idx = 0; idx < m_ClientList.size(); ++idx)
  //	delete m_ClientList;
  if (IsISCServer() == false) {
    std::lock_guard<std::mutex> lock(client_list_mutex_);
    for (auto& client : client_list_) {
      client->Shutdown();
      delete client;
    }
    client_list_.clear();
  } else {
    std::lock_guard<std::mutex> lock(isc_list_mutex_);
    for (auto& client : isc_list_) {
      client->Shutdown();
      delete client;
    }
    isc_list_.clear();
  }
}

bool CRoseServer::OnConnect() { return true; }

void CRoseServer::OnConnected() {}

bool CRoseServer::OnListen() { return true; }

void CRoseServer::OnListening() {}

bool CRoseServer::OnDisconnect() { return true; }

void CRoseServer::OnDisconnected() {}

bool CRoseServer::OnAccept() { return true; }

void CRoseServer::OnAccepted(tcp::socket _sock) {
  if (_sock.is_open()) {
    // Do Something?
    // std::lock_guard<std::mutex> lock(m_ClientListMutex);
    std::string _address = _sock.remote_endpoint().address().to_string();
    // CRoseClient* nClient = nullptr;
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CRoseClient* nClient = new CRoseClient(std::move(_sock));
      log_.icprintf("Client connected from: %s\n", _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CRoseISC* nClient = new CRoseISC(std::move(_sock));
      log_.icprintf("Server connected from: %s\n", _address.c_str());
      isc_list_.push_front(nClient);
    }

    // m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
    // m_ClientList.push_back( nClient );
  }
}
