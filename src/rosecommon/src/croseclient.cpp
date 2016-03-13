#include <ctime>
#include "croseclient.h"
#include "epackettype.h"


namespace RoseCommon {
//#define STRESS_TEST

CRoseClient::CRoseClient() : CNetwork_Asio(), crypt_() {
  log_.SetIdentity("CRoseClient");
  ResetBuffer();
}

CRoseClient::CRoseClient(tcp::socket _sock) : CNetwork_Asio(), crypt_() {
  SetSocket(std::move(_sock));
  log_.SetIdentity("CRoseClient");
  Recv();
  ResetBuffer();
}

CRoseClient::~CRoseClient() { Shutdown(); }

bool CRoseClient::Send(CRosePacket* _buffer) {
  //  std::unique_ptr<uint8_t*> buf( ((uint8_t*)_buffer) );
  return CNetwork_Asio::Send(std::unique_ptr<uint8_t>((uint8_t*)_buffer));
}

bool CRoseClient::Send(std::unique_ptr<uint8_t> _buffer) {
  return CNetwork_Asio::Send(std::move(_buffer));
}

// Callback functions
bool CRoseClient::OnConnect() { return true; }

void CRoseClient::OnConnected() { CNetwork_Asio::OnConnected(); }

bool CRoseClient::OnDisconnect() { return true; }

void CRoseClient::OnDisconnected() {}

bool CRoseClient::OnReceive() { return true; }

bool CRoseClient::OnReceived() {
  bool rtnVal = true;
  if (packet_size_ == 6) {
#ifndef STRESS_TEST
    packet_size_ = crypt_.decodeClientHeader((unsigned char*)&buffer_);
#else
    packet_size_ = buffer_[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      log_.eicprintf("Client sent incorrect blockheader\n");
      ResetBuffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef STRESS_TEST
  if (!crypt_.decodeClientBody((unsigned char*)&buffer_)) {
    // ERROR!!!
    log_.eicprintf("Client sent illegal block\n");
    ResetBuffer();
    return false;
  }
#endif

  CRosePacket* pak = (CRosePacket*)&buffer_;
  log_.oicprintf("Received a packet on CRoseClient: Header[%i, 0x%X]\n",
                 pak->Header.Size, pak->Header.Command);
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();

  return rtnVal;
}

bool CRoseClient::OnSend(uint8_t* _buffer) {
  (void)_buffer;
#ifndef STRESS_TEST
  crypt_.encodeServerPacket(_buffer);
#endif
  return true;
}
void CRoseClient::OnSent() {}

bool CRoseClient::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch ((ePacketType)pak->Header.Command) {
    case ePacketType::PAKCS_ALIVE: {
//      log_.icprintf("Got keep alive packet\n");
#ifdef STRESS_TEST
      CRosePacket* pak =
          new CRosePacket(ePacketType::PAKCS_ALIVE, sizeof(sPacketHeader));
      Send(pak);
#endif
      break;
    }
#ifdef STRESS_TEST
    case (ePacketType)0x6F6D: {
      CRosePacket* pak = new CRosePacket(0x6F6D, 8);
      memcpy(pak->Buffer, _buffer, 8);
      Send(pak);
      break;
    }
#endif
    case ePacketType::PAKCS_ACCEPT_REQ: {
      // Encryption stuff
      CRosePacket* pak = new CRosePacket(0x7ff, sizeof(pakEncryptionRequest));
      pak->pEncryptReq.Unknown = 0x02;
      pak->pEncryptReq.RandValue = static_cast<uint32_t>(std::time(nullptr));
      Send(pak);
      break;
    }
    default: {
      log_.eicprintf("Unknown Packet Type: 0x%X\n", pak->Header.Command);
      return false;
    }
  }
  return true;
}

}
