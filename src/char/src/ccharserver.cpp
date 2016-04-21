#include "ccharserver.h"
#include "ccharclient.h"
#include "ccharisc.h"
#include "epackettype.h"

using namespace RoseCommon;

CCharServer::CCharServer(bool _isc) : CRoseServer(_isc), client_count_(0), server_count_(0) {
}

CCharServer::~CCharServer() { Shutdown(); }

void CCharServer::OnAccepted(tcp::socket _sock) {
  if (_sock.is_open()) {
    // Do Something?
    std::string _address = _sock.remote_endpoint().address().to_string();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CCharClient* nClient = new CCharClient(std::move(_sock));
      nClient->SetId(client_count_++);
      logger_->notice("[{}] Client connected from: {}", nClient->GetId(),
                       _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CCharISC* nClient = new CCharISC(std::move(_sock));
      nClient->SetId(server_count_++);
      logger_->notice("Server connected from: {}", _address.c_str());
      isc_list_.push_front(nClient);
    }
  }
}
