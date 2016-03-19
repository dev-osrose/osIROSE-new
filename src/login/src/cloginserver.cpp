#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "epackettype.h"
#include "config.h"
#include "database.h"

CLoginServer::CLoginServer(bool _isc) : CRoseServer(_isc), client_count_(0), server_count_(0) {
}

CLoginServer::~CLoginServer() { Shutdown(); }

void CLoginServer::OnAccepted(tcp::socket _sock) {
  if (_sock.is_open()) {
    // Do Something?
    std::string _address = _sock.remote_endpoint().address().to_string();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CLoginClient* nClient = new CLoginClient(std::move(_sock));
      nClient->SetId(client_count_++);
      logger_->notice("[%d] Client connected from: %s\n", nClient->GetId(),
                    _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CLoginISC* nClient = new CLoginISC(std::move(_sock));
      nClient->SetId(server_count_++);
      logger_->notice("Server connected from: %s\n", _address.c_str());
      isc_list_.push_front(nClient);
    }
  }
}
