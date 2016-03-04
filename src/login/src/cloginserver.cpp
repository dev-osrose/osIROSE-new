#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "ePacketType.h"

CLoginServer::CLoginServer(bool _isc) : CRoseServer(_isc), client_count_(0) {
  if (true == _isc)
    log_.SetIdentity("CLoginISCServer");
  else
    log_.SetIdentity("CLoginServer");

  /*  process_thread_ = std::thread([this]() {
      active_ = true;
      while( IsActive() )
      {
      if (IsISCServer() == false) {
        std::lock_guard<std::mutex> lock(client_list_mutex_);
        for (auto& client : client_list_) {
          if(client->IsActive() == false) {
            client->Shutdown();
            delete client;
            client_list_.remove(client);
            //client = client_list_.before_begin();
            break;
          }
        }
      } else {
        std::lock_guard<std::mutex> lock(isc_list_mutex_);
        for (auto& client : isc_list_) {
          if(client->IsActive() == false) {
            client->Shutdown();
            delete client;
            isc_list_.remove(client);
            //client = isc_list_.before_begin();
            break;
          }
        }
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
      }

      return 0;
    });*/
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
      log_.icprintf("[%d] Client connected from: %s\n", nClient->GetId(),
                    _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CLoginISC* nClient = new CLoginISC(std::move(_sock));
      log_.icprintf("Server connected from: %s\n", _address.c_str());
      isc_list_.push_front(nClient);
    }
  }
}

// bool CLoginServer::HandlePacket( uint8_t* _buffer )
//{
//	CPacket* pak = (CPacket*)_buffer;
//	switch ( pak->Header.Command )
//	{
//	default:
//	{
//		CRoseServer::HandlePacket( _buffer );
//		return false;
//	}
//	}
//	return true;
//}
